#include "DX11Module.h"
#include "DX11VertexBuffer.h"
#include "DX11Font.h"
#include "DX11RasterizerState.h"

#include <mye/core/Game.h>
#include <mye/core/ResourceTypeManager.h>
#include <mye/core/Utils.h>

using namespace mye::dx11;
using namespace mye::core;
using namespace mye::math;

DX11Module::DX11Module(void) :
	m_ownedDevice(false),
	m_mvpBuffer(nullptr),
	m_modelTextureSampler(nullptr),
	m_fontTextureSampler(nullptr)
{
	NewWindow();
}

DX11Module::DX11Module(DX11Window *window) :
	m_ownedDevice(false),
	m_mvpBuffer(nullptr)
{
	SetWindow(window);
}

DX11Module::~DX11Module(void)
{
	FreeWindow();
}

bool DX11Module::Init(void)
{

	bool initialized = false;

	m_window->Show();

	//m_mvp->Create(sizeof(float) * 16, Matrix4f(1.0f).Data());

	if (m_mvpBuffer->Create(sizeof(float) * 16, Matrix4f(1.0f).Data()) &&
		m_textColorBuffer->Create(sizeof(float) * 4) &&
		m_window->Init())
	{

		RasterizerInfo rasterizeInfo = { false, CullMode::NONE };
		DX11RasterizerState rasterizeState(*m_device, rasterizeInfo);

		rasterizeState.Use();

		D3D11_SAMPLER_DESC fontSamplerDesc;
		
		ZeroMemory(&fontSamplerDesc, sizeof(D3D11_SAMPLER_DESC));
		
		fontSamplerDesc.Filter         = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
 		fontSamplerDesc.AddressU       = D3D11_TEXTURE_ADDRESS_BORDER;
		fontSamplerDesc.AddressV       = D3D11_TEXTURE_ADDRESS_BORDER;
		fontSamplerDesc.AddressW       = D3D11_TEXTURE_ADDRESS_BORDER;
		fontSamplerDesc.MipLODBias     = 0.0f;
		fontSamplerDesc.MaxAnisotropy  = 1;
		fontSamplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		fontSamplerDesc.MinLOD         = 0.0f;
		fontSamplerDesc.MaxLOD         = D3D11_FLOAT32_MAX;

		D3D11_SAMPLER_DESC modelSamplerDesc;

		ZeroMemory(&modelSamplerDesc, sizeof(D3D11_SAMPLER_DESC));

		modelSamplerDesc.Filter         = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		modelSamplerDesc.AddressU       = D3D11_TEXTURE_ADDRESS_WRAP;
		modelSamplerDesc.AddressV       = D3D11_TEXTURE_ADDRESS_WRAP;
		modelSamplerDesc.AddressW       = D3D11_TEXTURE_ADDRESS_WRAP;
		modelSamplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		modelSamplerDesc.MaxAnisotropy  = 1;
		modelSamplerDesc.MinLOD         = 0.0f;
		modelSamplerDesc.MaxLOD         = D3D11_FLOAT32_MAX;

		if (!HRTESTFAILED(m_device->GetDevice()->CreateSamplerState(&fontSamplerDesc, &m_fontTextureSampler)) &&
			!HRTESTFAILED(m_device->GetDevice()->CreateSamplerState(&modelSamplerDesc, &m_modelTextureSampler)))
		{

			VertexDeclaration textVD;
			textVD.AddAttribute(VertexAttributeSemantic::POSITION, VertexAttributeType::FLOAT2);
			textVD.AddAttribute(VertexAttributeSemantic::TEXCOORD0, VertexAttributeType::FLOAT2);

			auto textILV = MakeInputElementVector(textVD);

			VertexDeclaration basicVD;
			basicVD.AddAttribute(VertexAttributeSemantic::POSITION, VertexAttributeType::FLOAT3);
			basicVD.AddAttribute(VertexAttributeSemantic::TEXCOORD0, VertexAttributeType::FLOAT2);

			auto basicILV = MakeInputElementVector(basicVD);

			Parameters textParams;
			textParams["type"] = "vertex";
			textParams["precompiled"] = "true";
			textParams["inputLayoutVector"] = PointerToString(&textILV);

			Parameters basicParameters;
			basicParameters["type"] = "vertex";
			basicParameters["precompiled"] = "true";
			basicParameters["inputLayoutVector"] = PointerToString(&basicILV);
			

			m_text2dvs = ResourceTypeManager::GetSingleton().CreateResource<DX11VertexShader>(
				"DX11Shader",
				"./shaders/text2dvs.cso",
				nullptr,
				textParams);

			m_basicvs = ResourceTypeManager::GetSingleton().CreateResource<DX11VertexShader>(
				"DX11Shader",
				"./shaders/basicvs.cso",
				nullptr,
				basicParameters);

			textParams.Clear();

			textParams["type"] = "pixel";
			textParams["precompiled"] = "true";

			basicParameters.Clear();

			basicParameters["type"] = "pixel";
			basicParameters["precompiled"] = "true";

			m_text2dps = ResourceTypeManager::GetSingleton().CreateResource<DX11PixelShader>(
				"DX11Shader",
				"./shaders/text2dps.cso",
				nullptr,
				textParams);

			m_basicps = ResourceTypeManager::GetSingleton().CreateResource<DX11PixelShader>(
				"DX11Shader",
				"./shaders/basicps.cso",
				nullptr,
				basicParameters);

			if (m_text2dvs->Load() &&
				m_text2dps->Load() &&
				m_basicvs->Load() &&
				m_basicps->Load())
			{
				initialized = true;
			}

		}

	}

	return initialized;

}

void DX11Module::ShutDown(void)
{
	m_window->Hide();
}

void DX11Module::Render(void)
{

	m_device->SetBlending(false);

	m_device->GetImmediateContext()->ClearRenderTargetView(
		m_window->GetRenderTargetView(),
		m_clearColor.Data()
		);

	m_device->GetImmediateContext()->ClearDepthStencilView(
		m_window->GetDepthStencilView(),
		D3D11_CLEAR_DEPTH,
		1.0f,
		0
		);

	SceneModule *scene = Game::GetSingleton().GetSceneModule();
	Camera *camera = scene->GetCamera();

	std::list<Text2DComponent*> textes;

	if (camera)
	{

		SceneModule::ObjectsList visibleObjects = scene->GetVisibleObjects();

		Matrix4f viewProjection = camera->GetProjectionMatrix() *
			camera->GetViewMatrix();

		for (GameObject *object : visibleObjects)
		{

			RenderComponent *rc = object->GetRenderComponent();

			if (rc)
			{

				RigidBodyComponent *rb = object->GetRigidBodyComponent();
				TransformComponent *tc = object->GetTransformComponent();

				ModelPointer model = rc->GetModel();

				if (model)
				{

					model->Load();
					m_mvpBuffer->Bind(PIPELINE_VERTEX_SHADER, 0);

// 					if (rb)
// 					{
// 						m_mvpBuffer->SetData((viewProjection *
// 							rb->GetWorldMatrix()).Data());
// 					}
// 					else
					{
						Matrix4f mvp = viewProjection * tc->GetWorldMatrix() * rc->GetModelMatrix();
						m_mvpBuffer->SetData(mvp.Data());
					}

					m_basicvs->Use();
					m_basicps->Use();

					m_device->GetImmediateContext()->PSSetSamplers(0, 1, &m_modelTextureSampler);

					DX11VertexBuffer vertexBuffer(nullptr, "", nullptr, *m_device);

					vertexBuffer.Create(model.get());
					vertexBuffer.Bind();

					m_device->GetImmediateContext()->
						IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

					m_device->GetImmediateContext()->
						Draw(vertexBuffer.GetVerticesCount(), 0);

					vertexBuffer.Destroy();

				}

			}
			
			Text2DComponent *t2dc = object->GetText2DComponent();

			if (t2dc)
			{
				textes.push_back(t2dc);
			}

		}

		m_text2dvs->Use();
		m_text2dps->Use();

		m_device->GetImmediateContext()->PSSetSamplers(0, 1, &m_fontTextureSampler);

		m_device->SetBlending(true);

		for (Text2DComponent *t2dc : textes)
		{

			// Draw text

			DX11FontPointer font = Resource::DynamicCast<DX11Font>(t2dc->GetFont());
			DX11VertexBufferPointer vb = Resource::DynamicCast<DX11VertexBuffer>(t2dc->GetVertexBuffer());

			if (vb && font && font->Load() && vb->Load())
			{

				font->Use();
				vb->Bind();

				m_textColorBuffer->SetData(t2dc->GetColor().Data());
				m_textColorBuffer->Bind(PIPELINE_PIXEL_SHADER, 0);

				m_device->GetImmediateContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
				m_device->GetImmediateContext()->Draw(vb->GetVerticesCount(), 0);

			}

		}

	}

	

	m_window->GetSwapChain()->Present(1, 0);

}

void DX11Module::SetWindow(DX11Window *window)
{

	FreeWindow();

	m_device            = &(window->GetDevice());
	m_window            = window;
	m_ownedDevice       = false;

	m_mvpBuffer               = new DX11ConstantBuffer(nullptr, "", nullptr, *m_device);
	m_textColorBuffer         = new DX11ConstantBuffer(nullptr, "", nullptr, *m_device);

	m_mainWindowPointer = static_cast<IWindow*>(m_window);

}

void DX11Module::NewWindow(void)
{

	FreeWindow();

	m_device      = new DX11Device;
	m_window      = new DX11Window(*m_device);
	m_ownedDevice = true;

	m_window->Create();

	m_mvpBuffer         = new DX11ConstantBuffer(nullptr, "", nullptr, *m_device);
	m_textColorBuffer   = new DX11ConstantBuffer(nullptr, "", nullptr, *m_device);

	m_mainWindowPointer = static_cast<IWindow*>(m_window);

}

void DX11Module::FreeWindow(void)
{

	if (m_ownedDevice)
	{
		delete m_window;
		delete m_device;
		delete m_mvpBuffer;
	}

	m_window = nullptr;
	m_device = nullptr;
	m_ownedDevice = false;

	m_mainWindowPointer = nullptr;

}

Vector2f DX11Module::GetPointSize(void)
{

	Vector2i size = m_window->GetSize();

	return Vector2f(1.0f / size.x(), 1.0f / size.y());

}