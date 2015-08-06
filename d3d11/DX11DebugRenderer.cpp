#include "DX11DebugRenderer.h"

#include <mye/core/ResourceTypeManager.h>
#include <mye/core/Utils.h>

#include "DX11VertexBuffer.h"
#include "DX11ConstantBuffer.h"
#include "DX11Module.h"

#include "./detail/ShadersBuffers.h"

#include <mye/core/Game.h>

using namespace mye::dx11;
using namespace mye::core;
using namespace mye::win;
using namespace mye::math;

DX11DebugRenderer::DX11DebugRenderer(void) :
	m_initialized(false),
	m_linearSampler(nullptr)
{
}


DX11DebugRenderer::~DX11DebugRenderer(void)
{
}

bool DX11DebugRenderer::Init(void)
{

	D3D11_SAMPLER_DESC linearSamplerDesc;

	ZeroMemory(&linearSamplerDesc, sizeof(D3D11_SAMPLER_DESC));

	linearSamplerDesc.Filter         = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	linearSamplerDesc.AddressU       = D3D11_TEXTURE_ADDRESS_CLAMP;
	linearSamplerDesc.AddressV       = D3D11_TEXTURE_ADDRESS_CLAMP;
	linearSamplerDesc.AddressW       = D3D11_TEXTURE_ADDRESS_CLAMP;
	linearSamplerDesc.MipLODBias     = 0.0f;
	linearSamplerDesc.MaxAnisotropy  = 1;
	linearSamplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	linearSamplerDesc.MinLOD         = 0.0f;
	linearSamplerDesc.MaxLOD         = D3D11_FLOAT32_MAX;

	if (!__MYE_DX11_HR_TEST_FAILED(DX11Device::GetSingleton()->CreateSamplerState(&linearSamplerDesc, &m_linearSampler)))
	{

		VertexDeclaration vd = {
				{ VertexAttributeSemantic::POSITION, DataFormat::FLOAT2 },
				{ VertexAttributeSemantic::TEXCOORD0, DataFormat::FLOAT2 }
		};
	
		auto debugTextureViewILV = MakeInputElementVector(vd);

		m_texturedSimpleVS = ResourceTypeManager::GetSingleton().CreateResource<DX11VertexShader>(
			"DX11Shader",
			"./shaders/textured_simple_vs.cso",
			nullptr,
			Parameters({
					{ "type", "vertex" },
					{ "precompiled", "true" },
					{ "inputLayoutVector", PointerToString(&debugTextureViewILV) }
			})
		);

		m_texturedSimplePS = ResourceTypeManager::GetSingleton().CreateResource<DX11PixelShader>(
			"DX11Shader",
			"./shaders/textured_simple_ps.cso",
			nullptr,
			Parameters({
					{ "type", "pixel" },
					{ "precompiled", "true" }
			})
		);

		m_texturedArrayPS = ResourceTypeManager::GetSingleton().CreateResource<DX11PixelShader>(
			"DX11Shader",
			"./shaders/textured_arrayslice_ps.cso",
			nullptr,
			Parameters({
					{ "type", "pixel" },
					{ "precompiled", "true" }
			})
		);

		VertexDeclaration pVD = {
			{ VertexAttributeSemantic::POSITION, DataFormat::FLOAT3 },
			{ VertexAttributeSemantic::DIFFUSE, DataFormat::FLOAT4 }
		};

		auto debugPrimitiveILV = MakeInputElementVector(pVD);

		m_primitiveVS = ResourceTypeManager::GetSingleton().CreateResource<DX11VertexShader>(
			"DX11Shader",
			"./shaders/debug_primitives_vs.cso",
			nullptr,
			Parameters({
					{ "type", "vertex" },
					{ "precompiled", "true" },
					{ "inputLayoutVector", PointerToString(&debugPrimitiveILV) }
			})
		);

		m_primitivePS = ResourceTypeManager::GetSingleton().CreateResource<DX11PixelShader>(
			"DX11Shader",
			"./shaders/debug_primitives_ps.cso",
			nullptr,
			Parameters({
					{ "type", "pixel" },
					{ "precompiled", "true" }
			})
		);

		if (m_texturedSimpleVS->Load() &&
			m_texturedSimplePS->Load() &&
			m_texturedArrayPS->Load() &&
			m_primitiveVS->Load() &&
			m_primitivePS->Load())
		{

			m_initialized = true;

		}

	}

	return m_initialized;

}

void DX11DebugRenderer::Shutdown(void)
{

	if (m_initialized)
	{

		m_texturedSimpleVS->Unload();
		m_texturedSimplePS->Unload();

		m_initialized = false;

		__MYE_DX11_RELEASE_COM(m_linearSampler);

	}

}

void DX11DebugRenderer::Render(ID3D11RenderTargetView * target)
{

	/* Lines */

	if (!m_lines.empty())
	{

		DX11Device::GetSingleton().SetBlending(true);

		DX11Module * graphics = DX11Module::GetSingletonPointer();

		DX11DepthBuffer & depth = graphics->GetDepthBuffer();
		
		DX11Device::GetSingleton().SetDepthTest(DX11DepthTest::ON);
		DX11Device::GetSingleton().GetImmediateContext()->OMSetRenderTargets(1, &target, depth.GetDepthStencilView());

		Camera * camera = Game::GetSingleton().GetSceneModule()->GetCamera();

		Matrix4 viewMatrix = camera->GetViewMatrix();
		Matrix4 projMatrix = camera->GetProjectionMatrix();
		Matrix4 viewProjMatrix = projMatrix * viewMatrix;

		std::vector<float> lines(m_lines.size() * 14);

		VertexDeclaration pVD = {
				{ VertexAttributeSemantic::POSITION, DataFormat::FLOAT3 },
				{ VertexAttributeSemantic::DIFFUSE, DataFormat::FLOAT4 }
		};

		VertexData vd(pVD);

		vd.Allocate(m_lines.size() * 2);

		for (int k = 0; k < m_lines.size(); k++)
		{

			vd.SetVertexAttribute(k * 2, VertexAttributeSemantic::POSITION, DataFormat::FLOAT3, &m_lines[k].from);
			vd.SetVertexAttribute(k * 2, VertexAttributeSemantic::DIFFUSE,  DataFormat::FLOAT4, &m_lines[k].color);

			vd.SetVertexAttribute(k * 2 + 1, VertexAttributeSemantic::POSITION, DataFormat::FLOAT3, &m_lines[k].to);
			vd.SetVertexAttribute(k * 2 + 1, VertexAttributeSemantic::DIFFUSE,  DataFormat::FLOAT4, &m_lines[k].color);

		}

		m_primitiveVS->Use();
		m_primitivePS->Use();

		DX11VertexBuffer vb;
		vb.Create(vd.GetData(), vd.GetVerticesCount(), pVD);

		DX11ConstantBuffer transformBuffer;
		transformBuffer.Create(sizeof(detail::TransformBuffer));

		detail::TransformBuffer t;

		t.world               = Matrix4(1);
		t.worldView           = viewMatrix;
		t.worldViewProjection = viewProjMatrix;

		transformBuffer.SetData(&t);
		transformBuffer.Bind(DX11PipelineStage::VERTEX_SHADER, 0);

		vb.Bind();
		DX11Device::GetSingleton().GetImmediateContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
		DX11Device::GetSingleton().GetImmediateContext()->Draw(vd.GetVerticesCount(), 0);
		vb.Unbind();

		vb.Destroy();

		m_lines.clear();

		transformBuffer.Destroy();

	}

	/* Shader resource */

	if (!m_shaderResources.empty())
	{

		DX11Device::GetSingleton().SetBlending(false);
		DX11Device::GetSingleton().SetDepthTest(DX11DepthTest::OFF);
		DX11Device::GetSingleton().GetImmediateContext()->OMSetRenderTargets(1, &target, nullptr);

		VertexDeclaration srVD = {
				{ VertexAttributeSemantic::POSITION, DataFormat::FLOAT2 },
				{ VertexAttributeSemantic::TEXCOORD0, DataFormat::FLOAT2 }
		};

		Vector2i screenResolution = DX11Module::GetSingleton().GetRendererConfiguration()->GetScreenResolution();
		Matrix4 orthoProj = OrthographicProjectionD3DLH(0.f, (float) screenResolution.x(), 0.f, (float) screenResolution.y(), 0.f, 1.f);

		DX11ConstantBuffer transformBuffer;
		transformBuffer.Create(sizeof(Matrix4));

		m_texturedSimpleVS->Use();
		
		//transformBuffer.SetData(&orthoProj);

		DX11ConstantBuffer srBuffer;

		struct __SRBuffer
		{
			int slice;
			int __filler[3];
		};

		srBuffer.Create(sizeof(__SRBuffer));

		float quad[] = {
			0.f, 1.f, 0.f, 0.f,
			1.f, 0.f, 1.f, 1.f,
			0.f, 0.f, 0.f, 1.f,
			1.f, 1.f, 1.f, 0.f,
			1.f, 0.f, 1.f, 1.f,
			0.f, 1.f, 0.f, 0.f
		};

		DX11VertexBuffer quadBuffer;
		quadBuffer.Create(quad, 6, srVD);

		for (__ShadowResource & sr : m_shaderResources)
		{

			ID3D11ShaderResourceView * srv = sr.shaderResource->GetShaderResourceView();

			ID3D11Texture2D * texture;
			srv->GetResource((ID3D11Resource **) &texture);

			D3D11_TEXTURE2D_DESC textureDesc;

			texture->GetDesc(&textureDesc);

			if (textureDesc.ArraySize > 1)
			{

				__SRBuffer b = { sr.slice };

				m_texturedArrayPS->Use();

				srBuffer.SetData(&b);
				srBuffer.Bind(DX11PipelineStage::PIXEL_SHADER, 0);

			}
			else
			{
				m_texturedSimplePS->Use();
			}


			Vector2 position(sr.position[0], sr.position[1]);
			Vector2 size(sr.size[0], sr.size[1]);

			Matrix4 qTransform = orthoProj * TranslationMatrix4(Vector3(position, 0.f)) * ScaleMatrix4(Vector3(size, 0.f));

			transformBuffer.SetData(&qTransform);
			transformBuffer.Bind(DX11PipelineStage::VERTEX_SHADER, 0);

			sr.shaderResource->Bind(DX11PipelineStage::PIXEL_SHADER, __MYE_DX11_TEXTURE_SLOT_DIFFUSE);
			DX11Device::GetSingleton().GetImmediateContext()->PSSetSamplers(__MYE_DX11_TEXTURE_SLOT_DIFFUSE, 1, &m_linearSampler);

			quadBuffer.Bind();
			DX11Device::GetSingleton().GetImmediateContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			DX11Device::GetSingleton().GetImmediateContext()->Draw(6, 0);
			quadBuffer.Unbind();

			sr.shaderResource->Unbind();

			__MYE_DX11_RELEASE_COM(srv);
			__MYE_DX11_RELEASE_COM(texture);

		}

		quadBuffer.Destroy();

		transformBuffer.Destroy();
		srBuffer.Destroy();

		m_shaderResources.clear();

	}

	DX11Device::GetSingleton().GetImmediateContext()->OMSetRenderTargets(0, nullptr, nullptr);

}

void DX11DebugRenderer::EnqueueShaderResource(DX11ShaderResource & shaderResource, const Vector2i & position, const Vector2i & size, int slice)
{
	shaderResource.GetShaderResourceView()->AddRef();
	m_shaderResources.emplace_back(__ShadowResource { &shaderResource, { position.x(), position.y() }, { size.x(), size.y() }, slice });
}

void DX11DebugRenderer::EnqueueLine(const Vector3 & from, const Vector3 & to, const Vector4 & color)
{
	
	__Line line = {
			{ from.x(), from.y(), from.z() },
			{ to.x(), to.y(), to.z() },
			{ color.r(), color.g(), color.b(), color.a() }
	};

	m_lines.push_back(line);

}

void DX11DebugRenderer::EnqueueFrustum(const Frustum & frustum, const Vector4 & color)
{

	auto corners = frustum.GetCorners();

	EnqueueLine(corners[static_cast<int>(FrustumCorners::LEFT_BOTTOM_NEAR)],  corners[static_cast<int>(FrustumCorners::LEFT_TOP_NEAR)],     color);
	EnqueueLine(corners[static_cast<int>(FrustumCorners::RIGHT_BOTTOM_NEAR)], corners[static_cast<int>(FrustumCorners::RIGHT_TOP_NEAR)],    color);
	EnqueueLine(corners[static_cast<int>(FrustumCorners::LEFT_BOTTOM_NEAR)],  corners[static_cast<int>(FrustumCorners::RIGHT_BOTTOM_NEAR)], color);
	EnqueueLine(corners[static_cast<int>(FrustumCorners::LEFT_TOP_NEAR)],     corners[static_cast<int>(FrustumCorners::RIGHT_TOP_NEAR)],    color);

	EnqueueLine(corners[static_cast<int>(FrustumCorners::LEFT_BOTTOM_FAR)],   corners[static_cast<int>(FrustumCorners::LEFT_TOP_FAR)],      color);
	EnqueueLine(corners[static_cast<int>(FrustumCorners::RIGHT_BOTTOM_FAR)],  corners[static_cast<int>(FrustumCorners::RIGHT_TOP_FAR)],     color);
	EnqueueLine(corners[static_cast<int>(FrustumCorners::LEFT_BOTTOM_FAR)],   corners[static_cast<int>(FrustumCorners::RIGHT_BOTTOM_FAR)],  color);
	EnqueueLine(corners[static_cast<int>(FrustumCorners::LEFT_TOP_FAR)],      corners[static_cast<int>(FrustumCorners::RIGHT_TOP_FAR)],     color);

	EnqueueLine(corners[static_cast<int>(FrustumCorners::LEFT_TOP_NEAR)],     corners[static_cast<int>(FrustumCorners::LEFT_TOP_FAR)],      color);
	EnqueueLine(corners[static_cast<int>(FrustumCorners::LEFT_BOTTOM_NEAR)],  corners[static_cast<int>(FrustumCorners::LEFT_BOTTOM_FAR)],   color);
	EnqueueLine(corners[static_cast<int>(FrustumCorners::RIGHT_TOP_NEAR)],    corners[static_cast<int>(FrustumCorners::RIGHT_TOP_FAR)],     color);
	EnqueueLine(corners[static_cast<int>(FrustumCorners::RIGHT_BOTTOM_NEAR)], corners[static_cast<int>(FrustumCorners::RIGHT_BOTTOM_FAR)],  color);

}