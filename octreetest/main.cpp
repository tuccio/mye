#include <iostream>

#include <mye/core/Camera.h>
#include <mye/core/ColorRGBA.h>
#include <mye/core/Octree.h>
#include <mye/core/ResourceTypeManager.h>
#include <mye/core/Utils.h>

#include <mye/d3d11/DX11ConstantBuffer.h>
#include <mye/d3d11/DX11ShaderManager.h>
#include <mye/d3d11/DX11Window.h>
#include <mye/d3d11/DX11PixelShader.h>
#include <mye/d3d11/DX11VertexShader.h>
#include <mye/d3d11/DX11VertexBuffer.h>

#include <mye/math/Geometry.h>

#include <mye/win/MouseKeyboardInput.h>
#include <mye/win/Utils.h>

using namespace mye::core;
using namespace mye::math;
using namespace mye::dx11;
using namespace mye::win;

struct CameraRatioAdjuster :
	public IWindow::Listener
{

	Camera *camera;

	CameraRatioAdjuster(void)
	{
		camera = nullptr;
	}

	void OnResize(IWindow *window, const Vector2i &size)
	{
		if (camera)
		{
			camera->SetClipAspectRatio((float) size.x() / size.y());
		}
	}

};

void CompileShaders(void);

template <typename T>
void CreateOctree(std::list<DX11VertexBuffer> &list,
				  DX11Window &window,
				  OctreeTraverser<T> &traverser);

void CreateAABBt(std::list<DX11VertexBuffer> &list,
				DX11Window &window,const AABBt &AABBt);

void CreateQuad(std::list<DX11VertexBuffer> &list,
				DX11Window &window,
				const Vector3f &a,
				const Vector3f &b,
				const Vector3f &c,
				const Vector3f &d);

void DrawQuadList(std::list<DX11VertexBuffer> &list,
				  DX11Window &window)
{
	
	for (DX11VertexBuffer& b : list)
	{
		b.Bind();
		window.GetDevice().GetImmediateContext()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINESTRIP);
		window.GetDevice().GetImmediateContext()->Draw(5, 0);
	}

}

int main(int argc, char *argv[])
{

	float size = 1024;
	int n = 5000;

	float removeProbability = 0.0f;

	std::vector<Vector3f> points(n);
	std::list<int> removePoints;
	
	Octree<Vector3f> octree(
		Vector3f(0),
		Vector3f(size),
		256);

	for (int i = 0; i < n; i++)
	{

		Vector3f p(rand(), rand(), rand());
		p = p * (size / RAND_MAX) - Vector3f(size * 0.5f);
		octree.Insert(p);
		points[i] = p;

		if ((float) rand() / RAND_MAX > 1.0f - removeProbability)
		{
			removePoints.push_back(i);
		}

	}

	for (auto i : removePoints)
	{
		static int k = 0;
		octree.Remove(points[i - k]);
		points.erase(points.begin() + (i - k));
		k++;
	}

	MouseKeyboardInput input;
	input.Init();

	ResourceTypeManager resourceTypeManager;

	DX11Device device;
	DX11ShaderManager shaderManager(device);
	DX11Window window(device);
	SendQuitOnClose quitOnClose;
	CameraRatioAdjuster cameraRatioAdjuster;

	CompileShaders();

	DX11VertexShader *vs = ResourceTypeManager::GetSingleton().
		GetResource("DX11Shader", "VertexShader.hlsl").Cast<DX11VertexShader>();

	DX11VertexShader *ps = ResourceTypeManager::GetSingleton().
		GetResource("DX11Shader", "PixelShader.hlsl").Cast<DX11VertexShader>();

	vs->Use();
	ps->Use();

	Camera camera;

	camera.LookAt(
		Vector3f(0, 0, - (3 * size)),
		Vector3f(0, 1, 0),
		Vector3f(0));

	camera.SetFarClipDistance(3 * size);

	cameraRatioAdjuster.camera = &camera;

	window.Create();
	window.SetCaption("Octree test");

	if (!window.Init())
	{
		ShowErrorBox();
	}
	
	window.AddListener(&quitOnClose);
	window.AddListener(&cameraRatioAdjuster);
	window.Show();

	DX11ConstantBuffer mvpBuffer(nullptr, "", nullptr, device);
	mvpBuffer.Create(sizeof(float) * 16, Matrix4f(1.0f).Data());

	DX11ConstantBuffer colorBuffer(nullptr, "", nullptr, device);
	colorBuffer.Create(sizeof(float) * 4);

	DX11VertexBuffer verticesBuffer(nullptr, "", nullptr, device);

	VertexDeclaration vDecl;
	vDecl.AddAttribute(VertexDeclaration::VDA_POSITION, VertexDeclaration::VDAT_FLOAT4);

	if (points.size() > 0 &&
		!verticesBuffer.Create(&points[0], points.size(), vDecl))
	{
		ShowErrorBox("D3D11 Error");
		return 1;
	}

	OctreeTraverser<Vector3f> traverser(octree);
	std::list<DX11VertexBuffer> list;
	CreateOctree<Vector3f>(list, window, traverser);

	Transformf cameraTransform = Transformf::Identity();

	bool drawOnlyOnePoint = false;
	int pointToDraw = -1;

	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	do
	{

		input.GetMouse()->SetWheelDelta(0);

		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		window.ClearColorBuffer();
		window.ClearDepthBuffer();

		mvpBuffer.Bind(PIPELINE_VERTEX_SHADER, 0);		

		mvpBuffer.SetData(
			(camera.GetProjectionMatrix() *
			camera.GetViewMatrix() *
			cameraTransform.GetSRTMatrix()).Data());

		colorBuffer.Bind(PIPELINE_PIXEL_SHADER, 0);

		static ColorRGBA octreeColor(1.0f, 0.0f, 0.2f, 1.0f);
		colorBuffer.SetData(octreeColor.Data());

		if (input.GetKeyboard()->IsPressed(MYE_VK_W))
		{
			camera.SetPosition(camera.GetPosition() + Vector3f(0, 0, size * 0.01f));
		}

		if (input.GetKeyboard()->IsPressed(MYE_VK_S))
		{
			camera.SetPosition(camera.GetPosition() + Vector3f(0, 0, - size * 0.01f));
		}

		if (input.GetKeyboard()->IsPressed(MYE_VK_D))
		{
			Quaternionf q = cameraTransform.GetOrientation();
			cameraTransform.SetOrientation(Quaternionf(Vector3f(0, 1, 0), 1.5f) * q);
		}

		if (input.GetKeyboard()->IsPressed(MYE_VK_A))
		{
			Quaternionf q = cameraTransform.GetOrientation();
			cameraTransform.SetOrientation(Quaternionf(Vector3f(0, 1, 0), -1.5f) * q);
		}		

		if (input.GetKeyboard()->IsPressed(MYE_VK_E))
		{
			camera.SetPosition(camera.GetPosition() + Vector3f(0, size * 0.01f, 0));
		}

		if (input.GetKeyboard()->IsPressed(MYE_VK_Q))
		{
			camera.SetPosition(camera.GetPosition() + Vector3f(0, - size * 0.01f, 0));
		}

		if (float delta = input.GetMouse()->GetWheelDelta())
		{
			Quaternionf q = cameraTransform.GetOrientation();
			cameraTransform.SetOrientation(Quaternionf(Vector3f(1, 0, 0), delta * 1.5f) * q);
		}

		if (input.GetKeyboard()->IsPressed(MYE_VK_1))
		{

			drawOnlyOnePoint = true;
			pointToDraw++;

			if (pointToDraw == n)
			{
				pointToDraw = 0;
			}

			std::cout << pointToDraw << std::endl;

		}

		if (input.GetKeyboard()->IsPressed(MYE_VK_2))
		{
			drawOnlyOnePoint = false;
		}

		/* Draw Octree */

		DrawQuadList(list, window);

		/* Draw Points */

		if (points.size() > 0)
		{

			static ColorRGBA pointsColor(0.85f, 0.8f, 0.25f, 1.0f);
			colorBuffer.SetData(pointsColor.Data());

			verticesBuffer.Bind();

			device.GetImmediateContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

			if (drawOnlyOnePoint)
			{
				device.GetImmediateContext()->Draw(1, pointToDraw);
			}
			else
			{
				device.GetImmediateContext()->Draw(points.size(), 0);
			}

		}

		window.GetSwapChain()->Present(1, 0);

	}
	while (msg.message != WM_QUIT);

	return 0;

}

void CompileShaders(void)
{

	Parameters params;

	std::vector<D3D11_INPUT_ELEMENT_DESC> vDesc(1);

	vDesc[0].SemanticName         = "POSITION";
	vDesc[0].SemanticIndex        = 0;	
	vDesc[0].Format               = DXGI_FORMAT_R32G32B32_FLOAT;
	vDesc[0].InputSlot            = 0;
	vDesc[0].AlignedByteOffset    = 0;
	vDesc[0].InputSlotClass       = D3D11_INPUT_PER_VERTEX_DATA;
	vDesc[0].InstanceDataStepRate = 0;

	params["type"] = "vertex";
	params["inputLayoutVector"] = PointerToString(static_cast<void*>(&vDesc));

	ResourceHandle hShader = ResourceTypeManager::GetSingleton().CreateResource(
		"DX11Shader",
		"VertexShader.hlsl",
		nullptr,
		&params);

	DX11VertexShader *vertexShader = static_cast<DX11VertexShader*>(hShader.get());

	if (!hShader->Load())
	{

		const std::string &compileError = vertexShader->GetCompileError();

		if (!compileError.empty())
		{
			MessageBox(nullptr,
				compileError.c_str(),
				"Vertex shader compile error",
				MB_OK);
		}

	}

	params.clear();
	params["type"] = "pixel";

	hShader = ResourceTypeManager::GetSingleton().CreateResource(
		"DX11Shader",
		"PixelShader.hlsl",
		nullptr,
		&params);

	DX11PixelShader *pixelShader = static_cast<DX11PixelShader*>(hShader.get());

	if (!hShader->Load())
	{

		MessageBox(nullptr,
			pixelShader->GetCompileError().c_str(),
			"Pixel shader compile error",
			MB_OK);

	}

	vertexShader->Use();
	pixelShader->Use();

}

template <typename T>
void CreateOctree(std::list<DX11VertexBuffer>& list,
				  DX11Window &window,
				  OctreeTraverser<T> &traverser)
{

	OctreeNode *node = traverser.GetCurrent();

	if (node)
	{

		CreateAABBt(list, window, traverser.GetBounds());

		if (!node->IsLeaf())
		{

			OctreeInternalNode *internalNode = static_cast<OctreeInternalNode*>(node);

			for (int i = 0; i < 8; i++)
			{
				if (traverser.MoveToChild(OctreeInternalNode::Children(i)))
				{
					CreateOctree<Vector3f>(list, window, traverser);
					traverser.MoveToParent();
				}
			}

		}

	}

}

void CreateAABBt(std::list<DX11VertexBuffer> &list,
				DX11Window &window,
				const AABBt &AABBt)
{

	auto corners = AABBt.GetCorners();

	CreateQuad(list,
		window,
		corners[AABBt::LEFT_BOTTOM_NEAR],
		corners[AABBt::RIGHT_BOTTOM_NEAR],
		corners[AABBt::RIGHT_TOP_NEAR],
		corners[AABBt::LEFT_TOP_NEAR]);

	CreateQuad(list,
		window,
		corners[AABBt::LEFT_BOTTOM_FAR],
		corners[AABBt::RIGHT_BOTTOM_FAR],
		corners[AABBt::RIGHT_TOP_FAR],
		corners[AABBt::LEFT_TOP_FAR]);

	CreateQuad(list,
		window,
		corners[AABBt::LEFT_TOP_NEAR],
		corners[AABBt::RIGHT_TOP_NEAR],
		corners[AABBt::RIGHT_TOP_FAR],
		corners[AABBt::LEFT_TOP_FAR]);

	CreateQuad(list,
		window,
		corners[AABBt::RIGHT_BOTTOM_NEAR],
		corners[AABBt::RIGHT_TOP_NEAR],
		corners[AABBt::RIGHT_TOP_FAR],
		corners[AABBt::RIGHT_BOTTOM_FAR]);

	CreateQuad(list,
		window,
		corners[AABBt::LEFT_BOTTOM_NEAR],
		corners[AABBt::LEFT_TOP_NEAR],
		corners[AABBt::LEFT_TOP_FAR],
		corners[AABBt::LEFT_BOTTOM_FAR]);

}

void CreateQuad(std::list<DX11VertexBuffer> &list,
				DX11Window &window,
				const Vector3f &a,
				const Vector3f &b,
				const Vector3f &c,
				const Vector3f &d)
{

	VertexDeclaration vDecl;
	vDecl.AddAttribute(VertexDeclaration::VDA_POSITION, VertexDeclaration::VDAT_FLOAT3);

	DX11VertexBuffer qVB(nullptr, "", nullptr, window.GetDevice());

	Vector3f qBuffer[5] = {
		a,
		b,
		c,
		d,
		a
	};

	qVB.Create(&qBuffer[0], 5, vDecl);
	list.push_back(qVB);

}