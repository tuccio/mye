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
		camera = NULL;
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
void DrawOctree(DX11Window &window,
				OctreeTraverser<T> &traverser);

void DrawAABB(DX11Window &window,
			  const AABB &aabb);

void DrawQuad(DX11Window &window,
			  const Vector3f &a,
			  const Vector3f &b,
			  const Vector3f &c,
			  const Vector3f &d);

int main(int argc, char *argv[])
{

	float size = 32;
	float n = 256;

	std::vector<Vector3f> points(n);
	
	Octree<Vector3f> octree(
		Vector3f(0),
		Vector3f(size));

	for (int i = 0; i < n; i++)
	{
		Vector3f p(rand(), rand(), rand());
		p = p * (size / RAND_MAX) - Vector3f(size * 0.5f);
		octree.Insert(p);
		points[i] = p;
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

	DX11ConstantBuffer mvpBuffer(NULL, "", NULL, device);
	mvpBuffer.Create(sizeof(float) * 16, Matrix4f(1.0f).Data());

	DX11ConstantBuffer colorBuffer(NULL, "", NULL, device);
	colorBuffer.Create(sizeof(float) * 4);

	DX11VertexBuffer verticesBuffer(NULL, "", NULL, device);

	VertexDeclaration vDecl;
	vDecl.AddAttribute(VertexDeclaration::VDA_POSITION, VertexDeclaration::VDAT_FLOAT3);

	if (!verticesBuffer.Create(&points[0], n, vDecl))
	{
		ShowErrorBox("D3D11 Error");
		return 1;
	}

	camera.LookAt(
		Vector3f(0, 0, - (size + 8)),
		Vector3f(0, 1, 0),
		Vector3f(0));

	Transformf cameraTransform = Transformf::Identity();

	bool drawOnlyOnePoint = false;
	int pointToDraw = -1;

	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	do
	{

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
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
			cameraTransform.GetTRSMatrix()).Data());

		colorBuffer.Bind(PIPELINE_PIXEL_SHADER, 0);

		static ColorRGBA octreeColor(1.0f, 0.0f, 0.2f, 1.0f);
		colorBuffer.SetData(octreeColor.Data());

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

		if (input.GetKeyboard()->IsPressed(MYE_VK_S))
		{
			Quaternionf q = cameraTransform.GetOrientation();
			cameraTransform.SetOrientation(Quaternionf(Vector3f(1, 0, 0), -1.5f) * q);
		}

		if (input.GetKeyboard()->IsPressed(MYE_VK_W))
		{
			Quaternionf q = cameraTransform.GetOrientation();
			cameraTransform.SetOrientation(Quaternionf(Vector3f(1, 0, 0), 1.5f) * q);
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

		OctreeTraverser<Vector3f> traverser(octree);
		DrawOctree<Vector3f>(window, traverser);

		/* Draw Points */

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
			device.GetImmediateContext()->Draw(n, 0);
		}

		window.GetSwapChain()->Present(1, 0);

	}
	while (msg.message != WM_QUIT);

	return 0;

}

template <typename T>
void DrawOctree(DX11Window &window,
				OctreeTraverser<T> &traverser)
{

	OctreeNode *node = traverser.GetCurrent();

	if (node)
	{

		DrawAABB(window, traverser.GetBounds());

		if (!node->IsLeaf())
		{

			OctreeInternalNode *internalNode = static_cast<OctreeInternalNode*>(node);

			traverser.MoveToChild(OctreeInternalNode::FRONT_LEFT_BOTTOM);
			DrawOctree<Vector3f>(window, traverser);
			traverser.MoveToParent();

			traverser.MoveToChild(OctreeInternalNode::FRONT_RIGHT_BOTTOM);
			DrawOctree<Vector3f>(window, traverser);
			traverser.MoveToParent();

			traverser.MoveToChild(OctreeInternalNode::FRONT_RIGHT_TOP);
			DrawOctree<Vector3f>(window, traverser);
			traverser.MoveToParent();

			traverser.MoveToChild(OctreeInternalNode::FRONT_LEFT_TOP);
			DrawOctree<Vector3f>(window, traverser);
			traverser.MoveToParent();

			traverser.MoveToChild(OctreeInternalNode::BACK_LEFT_TOP);
			DrawOctree<Vector3f>(window, traverser);
			traverser.MoveToParent();

			traverser.MoveToChild(OctreeInternalNode::BACK_RIGHT_TOP);
			DrawOctree<Vector3f>(window, traverser);
			traverser.MoveToParent();

			traverser.MoveToChild(OctreeInternalNode::BACK_RIGHT_BOTTOM);
			DrawOctree<Vector3f>(window, traverser);
			traverser.MoveToParent();

			traverser.MoveToChild(OctreeInternalNode::BACK_LEFT_BOTTOM);
			DrawOctree<Vector3f>(window, traverser);
			traverser.MoveToParent();

		}

	}

}

void DrawAABB(DX11Window &window, const AABB &aabb)
{

	auto corners = aabb.GetCorners();

	DrawQuad(window,
		corners[AABB::FRONT_LEFT_BOTTOM],
		corners[AABB::FRONT_RIGHT_BOTTOM],
		corners[AABB::FRONT_RIGHT_TOP],
		corners[AABB::FRONT_LEFT_TOP]);

	DrawQuad(window,
		corners[AABB::BACK_LEFT_BOTTOM],
		corners[AABB::BACK_RIGHT_BOTTOM],
		corners[AABB::BACK_RIGHT_TOP],
		corners[AABB::BACK_LEFT_TOP]);

	DrawQuad(window,
		corners[AABB::FRONT_LEFT_TOP],
		corners[AABB::FRONT_RIGHT_TOP],
		corners[AABB::BACK_RIGHT_TOP],
		corners[AABB::BACK_LEFT_TOP]);

	DrawQuad(window,
		corners[AABB::FRONT_RIGHT_BOTTOM],
		corners[AABB::FRONT_RIGHT_TOP],
		corners[AABB::BACK_RIGHT_TOP],
		corners[AABB::BACK_RIGHT_BOTTOM]);

	DrawQuad(window,
		corners[AABB::FRONT_LEFT_BOTTOM],
		corners[AABB::FRONT_LEFT_TOP],
		corners[AABB::BACK_LEFT_TOP],
		corners[AABB::BACK_LEFT_BOTTOM]);

}

void DrawQuad(DX11Window &window,
			  const Vector3f &a,
			  const Vector3f &b,
			  const Vector3f &c,
			  const Vector3f &d)
{

	VertexDeclaration vDecl;
	vDecl.AddAttribute(VertexDeclaration::VDA_POSITION, VertexDeclaration::VDAT_FLOAT3);

	DX11VertexBuffer qVB(NULL, "", NULL, window.GetDevice());

	Vector3f qBuffer[5] = {
		a,
		b,
		c,
		d,
		a
	};

	qVB.Create(&qBuffer[0], 5, vDecl);

	qVB.Bind();

	window.GetDevice().GetImmediateContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
	window.GetDevice().GetImmediateContext()->Draw(5, 0);

	qVB.Destroy();

}

void CompileShaders(void)
{

	Resource::ParametersList params;

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
		NULL,
		&params);

	DX11VertexShader *vertexShader = static_cast<DX11VertexShader*>(hShader.get());

	if (!hShader->Load())
	{

		const std::string &compileError = vertexShader->GetCompileError();

		if (!compileError.empty())
		{
			MessageBox(NULL,
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
		NULL,
		&params);

	DX11PixelShader *pixelShader = static_cast<DX11PixelShader*>(hShader.get());

	if (!hShader->Load())
	{

		MessageBox(NULL,
			pixelShader->GetCompileError().c_str(),
			"Pixel shader compile error",
			MB_OK);

	}

	vertexShader->Use();
	pixelShader->Use();

}