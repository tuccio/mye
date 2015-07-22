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

DX11DebugRenderer::DX11DebugRenderer(DX11Device &device) :
	m_device(device),
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

	linearSamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	linearSamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	linearSamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	linearSamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	linearSamplerDesc.MipLODBias = 0.0f;
	linearSamplerDesc.MaxAnisotropy = 1;
	linearSamplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	linearSamplerDesc.MinLOD = 0.0f;
	linearSamplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	if (!__MYE_DX11_HR_TEST_FAILED(m_device.GetDevice()->CreateSamplerState(&linearSamplerDesc, &m_linearSampler)))
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

		m_device.SetBlending(true);

		DX11Module * graphics = DX11Module::GetSingletonPointer();

		DX11DepthBuffer & depth = graphics->GetDepthBuffer();
		
		m_device.SetDepthTest(DX11DepthTest::ON);
		m_device.GetImmediateContext()->OMSetRenderTargets(1, &target, depth.GetDepthStencilView());

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

		DX11VertexBuffer vb(nullptr, "", nullptr, m_device);
		vb.Create(vd.GetData(), vd.GetVerticesCount(), pVD);

		DX11ConstantBuffer transformBuffer(nullptr, "", nullptr, m_device);
		transformBuffer.Create(sizeof(detail::TransformBuffer));

		detail::TransformBuffer t;

		t.world               = Matrix4(1);
		t.worldView           = viewMatrix;
		t.worldViewProjection = viewProjMatrix;

		transformBuffer.SetData(&t);
		transformBuffer.Bind(DX11PipelineStage::VERTEX_SHADER, 0);

		vb.Bind();
		m_device.GetImmediateContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
		m_device.GetImmediateContext()->Draw(vd.GetVerticesCount(), 0);
		vb.Unbind();

		vb.Destroy();

		m_lines.clear();

		transformBuffer.Destroy();

	}

	/* Shader resource */

	if (!m_shaderResources.empty())
	{

		m_device.SetBlending(false);
		m_device.SetDepthTest(DX11DepthTest::OFF);
		m_device.GetImmediateContext()->OMSetRenderTargets(1, &target, nullptr);

		VertexDeclaration srVD = {
				{ VertexAttributeSemantic::POSITION, DataFormat::FLOAT2 },
				{ VertexAttributeSemantic::TEXCOORD0, DataFormat::FLOAT2 }
		};

		Vector2i screenResolution = DX11Module::GetSingleton().GetRendererConfiguration()->GetScreenResolution();
		Matrix4 orthoProj = OrthographicProjectionD3DLH(0.f, (float) screenResolution.x(), 0.f, (float) screenResolution.y());

		DX11ConstantBuffer transformBuffer(nullptr, "", nullptr, m_device);
		transformBuffer.Create(sizeof(Matrix4));

		m_texturedSimpleVS->Use();
		m_texturedSimplePS->Use();

		transformBuffer.SetData(&orthoProj);

		m_device.GetImmediateContext()->PSSetSamplers(__MYE_DX11_TEXTURE_SLOT_DIFFUSE, 1, &m_linearSampler);

		for (__ShadowResource & sr : m_shaderResources)
		{

			Vector2 position(sr.position[0], sr.position[1]);
			Vector2 size(sr.size[0], sr.size[1]);

			float quad[] = {
				position.x(), position.y() + size.y(), 0.f, 0.f,
				position.x() + size.x(), position.y(), 1.f, 1.f,
				position.x(), position.y(), 0.f, 1.f,
				position.x() + size.x(), position.y() + size.y(), 1.f, 0.f,
				position.x() + size.x(), position.y(), 1.f, 1.f,
				position.x(), position.y() + size.y(), 0.f, 0.f
			};

			DX11VertexBuffer quadBuffer(nullptr, "", nullptr, m_device);
			quadBuffer.Create(quad, 6, srVD);

			sr.shaderResource->Bind(DX11PipelineStage::PIXEL_SHADER, __MYE_DX11_TEXTURE_SLOT_DIFFUSE);

			transformBuffer.Bind(DX11PipelineStage::VERTEX_SHADER, 0);

			quadBuffer.Bind();
			m_device.GetImmediateContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			m_device.GetImmediateContext()->Draw(6, 0);
			quadBuffer.Unbind();

			sr.shaderResource->Unbind();
			sr.shaderResource->GetShaderResourceView()->Release();

			quadBuffer.Destroy();

		}

		transformBuffer.Destroy();
		m_shaderResources.clear();

	}

	m_device.GetImmediateContext()->OMSetRenderTargets(0, nullptr, nullptr);

}

void DX11DebugRenderer::EnqueueShaderResource(DX11ShaderResource & shaderResource, const Vector2i & position, const Vector2i & size)
{
	shaderResource.GetShaderResourceView()->AddRef();
	m_shaderResources.emplace_back(__ShadowResource { &shaderResource, { position.x(), position.y() }, { size.x(), size.y() } });
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

	EnqueueLine(corners[static_cast<int>(FrustumCorners::LEFT_BOTTOM_FAR)],   corners[static_cast<int>(FrustumCorners::LEFT_TOP_FAR)], color);
	EnqueueLine(corners[static_cast<int>(FrustumCorners::RIGHT_BOTTOM_FAR)],  corners[static_cast<int>(FrustumCorners::RIGHT_TOP_FAR)], color);
	EnqueueLine(corners[static_cast<int>(FrustumCorners::LEFT_BOTTOM_FAR)],   corners[static_cast<int>(FrustumCorners::RIGHT_BOTTOM_FAR)], color);
	EnqueueLine(corners[static_cast<int>(FrustumCorners::LEFT_TOP_FAR)],      corners[static_cast<int>(FrustumCorners::RIGHT_TOP_FAR)], color);

	EnqueueLine(corners[static_cast<int>(FrustumCorners::LEFT_TOP_NEAR)],     corners[static_cast<int>(FrustumCorners::LEFT_TOP_FAR)], color);
	EnqueueLine(corners[static_cast<int>(FrustumCorners::LEFT_BOTTOM_NEAR)],  corners[static_cast<int>(FrustumCorners::LEFT_BOTTOM_FAR)], color);
	EnqueueLine(corners[static_cast<int>(FrustumCorners::RIGHT_TOP_NEAR)],    corners[static_cast<int>(FrustumCorners::RIGHT_TOP_FAR)], color);
	EnqueueLine(corners[static_cast<int>(FrustumCorners::RIGHT_BOTTOM_NEAR)], corners[static_cast<int>(FrustumCorners::RIGHT_BOTTOM_FAR)], color);

}