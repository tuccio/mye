#include <mye/core/Camera.h>
#include <mye/core/ColorRGBA.h>
#include <mye/core/LooseOctree.h>
#include <mye/core/Model.h>
#include <mye/core/ResourceTypeManager.h>
#include <mye/core/Utils.h>

#include <mye/d3d11/DX11ConstantBuffer.h>
#include <mye/d3d11/DX11ShaderManager.h>
#include <mye/d3d11/DX11Window.h>
#include <mye/d3d11/DX11PixelShader.h>
#include <mye/d3d11/DX11VertexShader.h>
#include <mye/d3d11/DX11VertexBuffer.h>

#include <mye/win/MouseKeyboardInput.h>
#include <mye/win/Utils.h>

#include <mye/math/Geometry.h>

#include <deque>
#include <iostream>

using namespace mye::core;
using namespace mye::math;
using namespace mye::win;
using namespace mye::dx11;

void CompileShaders(void);

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

template <typename T>
void CreateLooseQuadtree(std::list<DX11VertexBuffer> &list,
						 DX11Window &window,
						 LooseOctree<T> &octree);

void CreateAABBt(std::list<DX11VertexBuffer> &list,
				DX11Window &window,const AABBf &AABBt);

void CreateQuad(std::list<DX11VertexBuffer> &list,
				DX11Window &window,
				const Vector3f &a,
				const Vector3f &b,
				const Vector3f &c,
				const Vector3f &d);

void DrawQuadList(std::list<DX11VertexBuffer> &list,
				  DX11Window &window);

int main(int argc, char *argv[])
{

	std::string modelPath = "D:\\Documenti\\mye\\test\\box.obj";

	Mesh model(nullptr, modelPath, nullptr);

	model.Load();
	auto minMax = model.GetMinMaxVertices();

	AABBf modelBounds = AABBf::FromMinMax(minMax.first, minMax.second);
	Vector3i axesSizeOrder = modelBounds.GetAxesOrderBySize();
	Vector3f modelBoundsHalfExtents = modelBounds.GetHalfExtents();

	float size = modelBoundsHalfExtents[axesSizeOrder[2]] * 2 + 1;
	unsigned int maxdepth = 64;
	unsigned int looseness = 2;

	bool drawOctree = true;
	bool drawModel = true;
	bool fillModel = true;

	/* Create octree */

	LooseOctree<Trianglef> octree(
		modelBounds.GetCenter(),
		size,
		maxdepth,
		looseness);

	for (int i = 0; i < model.GetTrianglesCount(); i++)
	{

		Trianglef triangle;

		model.GetVertexAttribute(i,
			0,
			VertexAttributeSemantic::POSITION,
			DataFormat::FLOAT3,
			&triangle.v0());

		model.GetVertexAttribute(i,
			1,
			VertexAttributeSemantic::POSITION,
			DataFormat::FLOAT3,
			&triangle.v1());

		model.GetVertexAttribute(i,
			2,
			VertexAttributeSemantic::POSITION,
			DataFormat::FLOAT3,
			&triangle.v2());

		octree.Insert(triangle, BoundingAABB(triangle));

	}

	unsigned int fails = 0;

	for (int i = 0; i < model.GetTrianglesCount(); i++)
	{

		Trianglef triangle;

		model.GetVertexAttribute(i,
			0,
			VertexAttributeSemantic::POSITION,
			DataFormat::FLOAT3,
			&triangle.v0());

		model.GetVertexAttribute(i,
			1,
			VertexAttributeSemantic::POSITION,
			DataFormat::FLOAT3,
			&triangle.v1());

		model.GetVertexAttribute(i,
			2,
			VertexAttributeSemantic::POSITION,
			DataFormat::FLOAT3,
			&triangle.v2());

		AABBf oldAABBt = BoundingAABB(triangle);
		AABBf newAABBt = oldAABBt.TransformAffine(TranslationMatrix4(Vector3f(0.5f, 0, 0)));

		if (!octree.Relocate(triangle, oldAABBt, newAABBt))
		{
			fails++;
		}

	}

	std::cout << "Fails: " << fails << std::endl;

	/* Setup window, graphics, input */

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
		modelBounds.GetCenter() + Vector3f(0, 0, - (2 * size)),
		Vector3f(0, 1, 0),
		modelBounds.GetCenter());

	camera.SetFarClipDistance(100 * size);
	camera.SetNearClipDistance(0.001f);

	cameraRatioAdjuster.camera = &camera;

	window.Create();
	window.SetCaption("Loose Octree test");

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

	/* Vertex buffers */

	std::list<DX11VertexBuffer> quadlist;
	CreateLooseQuadtree(quadlist, window, octree);

	DX11VertexBuffer modelVB(nullptr, "", nullptr, device);
	modelVB.Create(&model);

	/* Main loop */

	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	do
	{

		Frustumf f = camera.GetFrustum();

		input.GetMouse()->SetWheelDelta(0);

		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

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
			Quaternionf q = camera.GetOrientation();
			camera.SetOrientation(Quaternionf(Vector3f(0, 1, 0), 1.5f) * q);
		}

		if (input.GetKeyboard()->IsPressed(MYE_VK_A))
		{
			Quaternionf q = camera.GetOrientation();
			camera.SetOrientation(Quaternionf(Vector3f(0, 1, 0), -1.5f) * q);
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
			Quaternionf q = camera.GetOrientation();
			camera.SetOrientation(Quaternionf(Vector3f(1, 0, 0), delta * 1.5f) * q);
		}

		if (input.GetKeyboard()->IsPressed(MYE_VK_1))
		{
			drawModel = !drawModel;
		}

		if (input.GetKeyboard()->IsPressed(MYE_VK_2))
		{
			drawOctree = !drawOctree;
		}

		if (input.GetKeyboard()->IsPressed(MYE_VK_3))
		{
			fillModel = !fillModel;
		}

		window.ClearColorBuffer(ColorRGBA(1, 1, 1, 1));
		window.ClearDepthBuffer();

		mvpBuffer.Bind(PIPELINE_VERTEX_SHADER, 0);		

		mvpBuffer.SetData(
			(camera.GetProjectionMatrix() *
			camera.GetViewMatrix()).Data());

		colorBuffer.Bind(PIPELINE_PIXEL_SHADER, 0);

		if (drawOctree)
		{
			colorBuffer.SetData(ColorRGBA(0.15f, 0.15f, 0.2f, 1.0f).Data());
			DrawQuadList(quadlist, window);
		}

		if (Intersect(modelBounds, f) == VolumeSide::OUTSIDE)
		{
			std::cout << "Culled " << rand() << std::endl;
		}

		if (drawModel)
		{
			colorBuffer.SetData(ColorRGBA(0.25f, 0.0f, 0.0f, 1.0f).Data());

			modelVB.Bind();

			if (!fillModel)
			{
				device.GetImmediateContext()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
			}
			else
			{
				device.GetImmediateContext()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			}
			
			device.GetImmediateContext()->Draw(modelVB.GetVerticesCount(), 0);
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
void CreateLooseQuadtree(std::list<DX11VertexBuffer> &list,
						 DX11Window &window,
						 LooseOctree<T> &octree)
{

	std::deque<LooseOctreeNode<T>*> q;

	q.push_back(octree.GetRoot());

	while (!q.empty())
	{

		LooseOctreeNode<T> *node = q.front();
		q.pop_front();

		if (node->GetObjectsCount() > 0)
		{
			CreateAABBt(list, window, node->GetBounds());
		}

		for (int i = 0; i < 8; i++)
		{

			LooseOctreeNode<T> *child = node->GetChild(static_cast<OctreeChild>(i));

			if (child)
			{
				q.push_back(child);
			}

		}

	}

}

void CreateAABBt(std::list<DX11VertexBuffer> &list,
				DX11Window &window,
				const AABBf &AABBt)
{

	auto corners = AABBt.GetCorners();

	CreateQuad(list,
		window,
		corners[static_cast<int>(AABBCorners::LEFT_BOTTOM_NEAR)],
		corners[static_cast<int>(AABBCorners::RIGHT_BOTTOM_NEAR)],
		corners[static_cast<int>(AABBCorners::RIGHT_TOP_NEAR)],
		corners[static_cast<int>(AABBCorners::LEFT_TOP_NEAR)]);

	CreateQuad(list,
		window,
		corners[static_cast<int>(AABBCorners::LEFT_BOTTOM_FAR)],
		corners[static_cast<int>(AABBCorners::RIGHT_BOTTOM_FAR)],
		corners[static_cast<int>(AABBCorners::RIGHT_TOP_FAR)],
		corners[static_cast<int>(AABBCorners::LEFT_TOP_FAR)]);

	CreateQuad(list,
		window,
		corners[static_cast<int>(AABBCorners::LEFT_TOP_NEAR)],
		corners[static_cast<int>(AABBCorners::RIGHT_TOP_NEAR)],
		corners[static_cast<int>(AABBCorners::RIGHT_TOP_FAR)],
		corners[static_cast<int>(AABBCorners::LEFT_TOP_FAR)]);

	CreateQuad(list,
		window,
		corners[static_cast<int>(AABBCorners::RIGHT_BOTTOM_NEAR)],
		corners[static_cast<int>(AABBCorners::RIGHT_TOP_NEAR)],
		corners[static_cast<int>(AABBCorners::RIGHT_TOP_FAR)],
		corners[static_cast<int>(AABBCorners::RIGHT_BOTTOM_FAR)]);

	CreateQuad(list,
		window,
		corners[static_cast<int>(AABBCorners::LEFT_BOTTOM_NEAR)],
		corners[static_cast<int>(AABBCorners::LEFT_TOP_NEAR)],
		corners[static_cast<int>(AABBCorners::LEFT_TOP_FAR)],
		corners[static_cast<int>(AABBCorners::LEFT_BOTTOM_FAR)]);

}

void CreateQuad(std::list<DX11VertexBuffer> &list,
				DX11Window &window,
				const Vector3f &a,
				const Vector3f &b,
				const Vector3f &c,
				const Vector3f &d)
{

	VertexDeclaration vDecl;
	vDecl.AddAttribute(VertexAttributeSemantic::POSITION, DataFormat::FLOAT3);

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