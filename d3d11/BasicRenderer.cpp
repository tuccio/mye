#include "BasicRenderer.h"

#include <mye/core/Game.h>
#include <mye/core/ResourceTypeManager.h>
#include <mye/core/Utils.h>

#include "DX11VertexBuffer.h"

#include "./detail/ShadersBuffers.h"

using namespace mye::dx11;
using namespace mye::core;
using namespace mye::win;
using namespace mye::math;

BasicRenderer::BasicRenderer(DX11Device &device, Window *window) :
	m_device(device),
	m_window(nullptr),
	m_initialized(false),
	m_transformBuffer(nullptr, "", nullptr, device),
	m_clearColor(0.0f)
{

	SetWindow(window);

	DX11DepthBufferConfiguration depthBufferConf;

	depthBufferConf.device = &device;
	depthBufferConf.height = 0;
	depthBufferConf.width = 0;

	m_depthBuffer = DX11DepthBuffer(depthBufferConf);

}


BasicRenderer::~BasicRenderer(void)
{
}

bool BasicRenderer::Init(void)
{

	VertexDeclaration basicVD({
		VertexAttribute(VertexAttributeSemantic::POSITION, DataFormat::FLOAT3),
		VertexAttribute(VertexAttributeSemantic::TEXCOORD0, DataFormat::FLOAT2)
	});

	auto basicILV = MakeInputElementVector(basicVD);

	m_basicVS = ResourceTypeManager::GetSingleton().CreateResource<DX11VertexShader>(
		"DX11Shader",
		"./shaders/basic_vs.cso",
		nullptr,
		Parameters({
			{ "type", "vertex" },
			{ "precompiled", "true" },
			{ "inputLayoutVector", PointerToString(&basicILV) }
		})
	);

	m_basicPS = ResourceTypeManager::GetSingleton().CreateResource<DX11PixelShader>(
		"DX11Shader",
		"./shaders/basic_ps.cso",
		nullptr,
		Parameters({
			{ "type", "pixel" },
			{ "precompiled", "true" },
		})
	);

	if (m_basicVS->Load() &&
		m_basicPS->Load() &&
		m_depthBuffer.Create() &&
		CreateConstantBuffers())
	{

		Vector2i clientSize = m_window->GetSize();
		OnResize(m_window, clientSize);

		m_window->AddListener(this);

		m_initialized = true;

	}

	return m_initialized;

}

void BasicRenderer::Shutdown(void)
{

	if (m_initialized)
	{

		SetWindow(nullptr);

		m_depthBuffer.Destroy();

		m_basicVS->Unload();
		m_basicPS->Unload();

		m_initialized = false;

	}

}

void BasicRenderer::Render(ID3D11RenderTargetView *target)
{

	m_device.SetBlending(false);

	m_depthBuffer.Clear();

	m_device.GetImmediateContext()->OMSetRenderTargets(
		1,
		&target,
		m_depthBuffer.GetDepthStencilView());

	SceneModule *scene = Game::GetSingleton().GetSceneModule();
	Camera *camera = scene->GetCamera();

	if (camera)
	{

		m_basicVS->Use();
		m_basicPS->Use();

		SceneModule::ObjectsList visibleObjects = scene->GetVisibleObjects();

		Matrix4f view = camera->GetViewMatrix();
		Matrix4f projection = camera->GetProjectionMatrix();

		for (GameObject *object : visibleObjects)
		{

			RenderComponent *rc = object->GetRenderComponent();

			if (rc)
			{

				TransformComponent *tc = object->GetTransformComponent();

				ModelPointer model = rc->GetModel();

				if (model && model->Load())
				{

					m_transformBuffer.Bind(PIPELINE_VERTEX_SHADER, 0);

					detail::TransformBuffer transformBuffer;

					transformBuffer.world = tc->GetWorldMatrix() * rc->GetModelMatrix();
					transformBuffer.worldView = view * transformBuffer.world;
					transformBuffer.worldViewProjection = projection * transformBuffer.worldView;

					m_transformBuffer.SetData(&transformBuffer);

					DX11VertexBuffer vertexBuffer(nullptr, "", nullptr, m_device);

					vertexBuffer.Create(model.get());
					vertexBuffer.Bind();

					m_device.GetImmediateContext()->
						IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

					m_device.GetImmediateContext()->
						Draw(vertexBuffer.GetVerticesCount(), 0);

					vertexBuffer.Destroy();

				}

			}

		}

	}

}

void BasicRenderer::SetWindow(Window *window)
{

	if (m_initialized)
	{

		if (m_window)
		{
			m_window->RemoveListener(this);
		}

		m_window = window;

		if (window)
		{

			Vector2i clientSize = m_window->GetSize();
			OnResize(m_window, clientSize);

			m_window->AddListener(this);

		}

	}
	else
	{

		m_window = window;

	}


}

void BasicRenderer::OnResize(IWindow *window, const Vector2i &size)
{

	m_depthBuffer.Resize(size.x(), size.y());

}

bool BasicRenderer::CreateConstantBuffers(void)
{

	return m_transformBuffer.Create(sizeof(detail::TransformBuffer));

}