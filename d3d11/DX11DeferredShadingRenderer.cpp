#include "DX11DeferredShadingRenderer.h"

#include "DX11VertexBuffer.h"
#include "DX11Utils.h"

#include "./detail/ShadersBuffers.h"

#include <mye/core/Game.h>
#include <mye/core/ResourceTypeManager.h>
#include <mye/core/Utils.h>

#include <mye/math/Math.h>

using namespace mye::dx11;
using namespace mye::core;
using namespace mye::win;
using namespace mye::math;

DX11DeferredShadingRenderer::DX11DeferredShadingRenderer(DX11Device & device, Window * window) :
	m_device(device),
	m_window(nullptr),
	m_target0(nullptr, "", nullptr, device),
	m_target1(nullptr, "", nullptr, device),
	m_target2(nullptr, "", nullptr, device),
	m_target3(nullptr, "", nullptr, device),
	m_initialized(false),
	m_transformBuffer(nullptr, "", nullptr, device),
	m_lightBuffer(nullptr, "", nullptr, device),
	m_materialBuffer(nullptr, "", nullptr, device),
	m_cameraBuffer(nullptr, "", nullptr, device),
	m_clearColor(0.0f)
{

	Parameters params({ { "renderTarget", "true" } });

	m_target0.SetParametersList(params);
	m_target1.SetParametersList(params);
	m_target2.SetParametersList(params);
	m_target3.SetParametersList(params);

	SetWindow(window);

	DX11DepthBufferConfiguration depthBufferConf;

	depthBufferConf.device         = &device;
	depthBufferConf.height         = 0;
	depthBufferConf.width          = 0;
	depthBufferConf.shaderResource = false;

	m_depthBuffer = DX11DepthBuffer(depthBufferConf);

}


DX11DeferredShadingRenderer::~DX11DeferredShadingRenderer(void)
{

}

bool DX11DeferredShadingRenderer::Init(void)
{

	VertexDeclaration deferredGeometryVD({
		VertexAttribute(VertexAttributeSemantic::POSITION, DataFormat::FLOAT3),
		VertexAttribute(VertexAttributeSemantic::TEXCOORD0, DataFormat::FLOAT2),
		VertexAttribute(VertexAttributeSemantic::NORMAL, DataFormat::FLOAT3)
	});

	VertexDeclaration deferredLightsVD({
		VertexAttribute(VertexAttributeSemantic::POSITION, DataFormat::FLOAT4)
	});

	
	auto deferredGeometryILV = MakeInputElementVector(deferredGeometryVD);
	auto deferredLightsILV = MakeInputElementVector(deferredLightsVD);

	m_deferredGeometryVS = ResourceTypeManager::GetSingleton().CreateResource<DX11VertexShader>(
		"DX11Shader",
		"./shaders/deferred_shading_geometry_vs.cso",
		nullptr,
		Parameters({
			{ "type", "vertex" },
			{ "precompiled", "true" },
			{ "inputLayoutVector", PointerToString(&deferredGeometryILV) }
		})
	);

	m_deferredLightsVS = ResourceTypeManager::GetSingleton().CreateResource<DX11VertexShader>(
		"DX11Shader",
		"./shaders/deferred_shading_lights_vs.cso",
		nullptr,
		Parameters({
			{ "type", "vertex" },
			{ "precompiled", "true" },
			{ "inputLayoutVector", PointerToString(&deferredLightsILV) }
		})
	);

	m_deferredGeometryPS = ResourceTypeManager::GetSingleton().CreateResource<DX11PixelShader>(
		"DX11Shader",
		"./shaders/deferred_shading_geometry_ps.cso",
		nullptr,
		Parameters({
			{ "type", "pixel" },
			{ "precompiled", "true" },
		})
	);
	
	m_deferredLightsPS = ResourceTypeManager::GetSingleton().CreateResource<DX11PixelShader>(
		"DX11Shader",
		"./shaders/deferred_shading_lights_ps.cso",
		nullptr,
		Parameters({
			{ "type", "pixel" },
			{ "precompiled", "true" },
		})
	);
	
	if (m_deferredGeometryVS->Load() &&
		m_deferredGeometryPS->Load() &&
		m_deferredLightsVS->Load() &&
		m_deferredLightsPS->Load() &&
		m_depthBuffer.Create() &&
		CreateConstantBuffers())
	{

		Vector2i clientSize = m_window->GetSize();
		OnResize(m_window, clientSize);

		m_window->AddListener(static_cast<IWindow::Listener*>(this));

		m_initialized = true;

	}
	
	return m_initialized;

}

void DX11DeferredShadingRenderer::Shutdown(void)
{

	if (m_initialized)
	{

		SetWindow(nullptr);

		m_depthBuffer.Destroy();

		m_target0.Destroy();
		m_target1.Destroy();
		m_target2.Destroy();
		m_target3.Destroy();

		m_deferredGeometryVS->Unload();
		m_deferredGeometryPS->Unload();
		m_deferredLightsVS->Unload();
		m_deferredLightsPS->Unload();

		m_initialized = false;

	}

}

void DX11DeferredShadingRenderer::Render(ID3D11RenderTargetView * target)
{

	ID3D11RenderTargetView *renderTargets[] = {
		m_target0.GetRenderTargetView(),
		m_target1.GetRenderTargetView(),
		m_target2.GetRenderTargetView(),
		m_target3.GetRenderTargetView()
	};

	m_target0.ClearRenderTarget(Vector4f(0.0f));
	m_target1.ClearRenderTarget(Vector4f(0.0f));
	m_target2.ClearRenderTarget(Vector4f(0.0f));
	m_target3.ClearRenderTarget(Vector4f(0.0f));

	m_depthBuffer.Clear();

	SceneModule * scene = Game::GetSingleton().GetSceneModule();
	Camera      * camera = scene->GetCamera();

	if (camera)
	{

		auto visibleObjects = scene->GetVisibleObjects();

		Matrix4 view       = camera->GetViewMatrix();
		Matrix4 projection = camera->GetProjectionMatrix();

		/* First step */

		m_deferredGeometryVS->Use();
		m_deferredGeometryPS->Use();

		m_device.GetImmediateContext()->OMSetRenderTargets(
			4,
			renderTargets,
			m_depthBuffer.GetDepthStencilView());

		m_device.SetBlending(false);
		m_device.SetDepthTest(DX11DepthTest::ON);

		for (GameObject * object : visibleObjects)
		{

			RenderComponent * rc = object->GetRenderComponent();

			if (rc)
			{

				TransformComponent * tc = object->GetTransformComponent();

				MeshPointer mesh = rc->GetMesh();

				if (mesh && mesh->Load())
				{

					detail::TransformBuffer transformBuffer;

					transformBuffer.world               = tc->GetWorldMatrix() * rc->GetModelMatrix();
					transformBuffer.worldView           = view * transformBuffer.world;
					transformBuffer.worldViewProjection = projection * transformBuffer.worldView;

					m_transformBuffer.Bind(DX11PipelineStage::VERTEX_SHADER, 0);
					m_transformBuffer.SetData(&transformBuffer);

					MaterialPointer material = rc->GetMaterial();

					detail::MaterialBuffer materialBuffer;

					materialBuffer.diffuseColor  = material->GetDiffuseColor();
					materialBuffer.specularColor = material->GetSpecularColor();
					materialBuffer.specular      = material->GetSpecular();
					materialBuffer.metallic      = material->GetMetallic();
					materialBuffer.roughness     = material->GetRoughness();

					m_materialBuffer.Bind(DX11PipelineStage::PIXEL_SHADER, 0);
					m_materialBuffer.SetData(&materialBuffer);

					DX11VertexBuffer vertexBuffer(nullptr, "", nullptr, m_device);

					vertexBuffer.Create(mesh.get());
					vertexBuffer.Bind();

					m_device.GetImmediateContext()->
						IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

					m_device.GetImmediateContext()->
						Draw(vertexBuffer.GetVerticesCount(), 0);

					vertexBuffer.Unbind();
					vertexBuffer.Destroy();

				}

			}

		}

		/* Second step */

		m_deferredLightsVS->Use();
		m_deferredLightsPS->Use();

		m_device.SetDepthTest(DX11DepthTest::OFF);

		m_device.GetImmediateContext()->OMSetRenderTargets(
			1,
			&target,
			m_depthBuffer.GetDepthStencilView());

		m_target0.Bind(DX11PipelineStage::PIXEL_SHADER, 0);
		m_target1.Bind(DX11PipelineStage::PIXEL_SHADER, 1);
		m_target2.Bind(DX11PipelineStage::PIXEL_SHADER, 2);
		m_target3.Bind(DX11PipelineStage::PIXEL_SHADER, 3);

		Vector4f quad[6] = {
			Vector4f(-1.0f, 1.0f, 0.0f, 1.0f),
			Vector4f(1.0f, -1.0f, 0.0f, 1.0f),
			Vector4f(-1.0f, -1.0f, 0.0f, 1.0f),
			Vector4f(1.0f, 1.0f, 0.0f, 1.0f),
			Vector4f(1.0f, -1.0f, 0.0f, 1.0f),
			Vector4f(-1.0f, 1.0f, 0.0f, 1.0f)
		};

		DX11VertexBuffer quadBuffer(nullptr, "", nullptr, m_device);
		quadBuffer.Create(quad, 6, VertexDeclaration({ VertexAttribute(VertexAttributeSemantic::POSITION, DataFormat::FLOAT4) }));

		quadBuffer.Bind();

		for (Light * light : scene->GetLightsList())
		{

			const mye::math::Matrix4 & t = light->GetWorldMatrix();

			detail::LightBuffer lightBuffer =
			{

				mye::math::Vector4f(light->GetColor() * light->GetIntensity(), 1),

				t * mye::math::Vector4f(light->GetPosition(), 1),
				t * mye::math::Vector4f(light->GetDirection(), 1),

				light->GetSpotAngle(),
				light->GetRange()

			};

			m_lightBuffer.SetData(&lightBuffer);
			m_lightBuffer.Bind(DX11PipelineStage::PIXEL_SHADER, 0);

			detail::CameraBuffer cameraBuffer;

			cameraBuffer.position = mye::math::Vector4f(camera->GetPosition(), 1);

			m_cameraBuffer.SetData(&cameraBuffer);
			m_cameraBuffer.Bind(DX11PipelineStage::PIXEL_SHADER, 1);

			m_device.GetImmediateContext()->
				IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			m_device.GetImmediateContext()->
				Draw(quadBuffer.GetVerticesCount(), 0);

		}

		quadBuffer.Unbind();

		quadBuffer.Destroy();

		m_target0.Unbind();
		m_target1.Unbind();
		m_target2.Unbind();
		m_target3.Unbind();

	}	

}

void DX11DeferredShadingRenderer::SetWindow(Window * window)
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

void DX11DeferredShadingRenderer::OnResize(IWindow * window, const Vector2i & size)
{

	m_target0.Destroy();
	m_target1.Destroy();
	m_target2.Destroy();
	m_target3.Destroy();

	m_target0.Create(size.x(), size.y(), DataFormat::FLOAT4);
	m_target1.Create(size.x(), size.y(), DataFormat::FLOAT4);
	m_target2.Create(size.x(), size.y(), DataFormat::FLOAT4);
	m_target3.Create(size.x(), size.y(), DataFormat::FLOAT4);

	m_depthBuffer.Resize(size.x(), size.y());

}

bool DX11DeferredShadingRenderer::CreateConstantBuffers(void)
{

	return
		m_transformBuffer.Create(sizeof(detail::TransformBuffer)) &&
		m_lightBuffer.Create(sizeof(detail::LightBuffer)) &&
		m_cameraBuffer.Create(sizeof(detail::CameraBuffer)) &&
		m_materialBuffer.Create(sizeof(detail::MaterialBuffer));

}