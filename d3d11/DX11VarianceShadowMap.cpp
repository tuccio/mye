#include "DX11VarianceShadowMap.h"

#include <mye/core/ResourceTypeManager.h>

using namespace mye::dx11;
using namespace mye::core;

DX11VarianceShadowMap::DX11VarianceShadowMap(void) :
	m_initialized(false),
	m_splits(1),
	m_resolution(1024),
	m_exponentialTest(false),
	m_blurKernelSize(0) { }

bool DX11VarianceShadowMap::Create(void)
{

	if (__CreateShaders() && __CreateRenderTargets())
	{
		m_initialized = true;
	}

	return true;

}

void DX11VarianceShadowMap::Destroy(void)
{

	if (m_initialized)
	{
		__DestroyRenderTargets();
		m_initialized = false;
	}
	
}

void DX11VarianceShadowMap::Render(void)
{

	auto viewports = DX11Device::GetSingleton().GetViewports();

	D3D11_VIEWPORT vsmViewport = { 0, 0, m_resolution, m_resolution, 0.f, 1.f };

	DX11Device::GetSingleton().SetViewports(&vsmViewport, 1);

	DX11Device::GetSingleton().SetBlending(false);
	DX11Device::GetSingleton().SetDepthTest(DX11DepthTest::OFF);

	ID3D11RenderTargetView * vsmDepthRt[] = { m_vsmDepth[0].GetRenderTargetView(), m_vsmDepth[1].GetRenderTargetView() };

	/* Generate VSM on RT1 */

	DX11Device::GetSingleton().GetImmediateContext()->OMSetRenderTargets(1, &vsmDepthRt[1], nullptr);

	// Load in case some parameter was changed
	m_vsmGeneration->Load();
	m_vsmGeneration->Use();

	DX11Device::GetSingleton().GetImmediateContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	DX11Device::GetSingleton().GetImmediateContext()->Draw(4, 0);

	/* Blur horizontally on RT0 */

	m_blur[0]->Use();

	DX11Device::GetSingleton().GetImmediateContext()->OMSetRenderTargets(1, &vsmDepthRt[0], nullptr);
	m_vsmDepth[1].Bind(DX11PipelineStage::PIXEL_SHADER, 32);

	DX11Device::GetSingleton().GetImmediateContext()->Draw(4, 0);

	m_vsmDepth[1].Unbind();

	/* Blur vertically on RT1 */

	m_blur[1]->Use();

	DX11Device::GetSingleton().GetImmediateContext()->OMSetRenderTargets(1, &vsmDepthRt[1], nullptr);
	m_vsmDepth[0].Bind(DX11PipelineStage::PIXEL_SHADER, 32);

	DX11Device::GetSingleton().GetImmediateContext()->Draw(4, 0);

	m_vsmDepth[0].Unbind();

	/* Generate mipmaps for trilinear sampling of VSM */

	m_vsmDepth[1].GenerateMips();

	DX11Device::GetSingleton().GetImmediateContext()->OMSetRenderTargets(0, nullptr, nullptr);
	DX11Device::GetSingleton().SetViewports(&viewports[0], viewports.size());	

}

int DX11VarianceShadowMap::GetResolution(void) const
{
	return m_resolution;
}

void DX11VarianceShadowMap::SetResolution(int resolution)
{

	m_resolution = resolution;

	if (m_initialized)
	{
		Destroy();
		Create();
	}
	
}

int DX11VarianceShadowMap::GetSplits(void) const
{
	return m_splits;
}

void DX11VarianceShadowMap::SetSplits(int splits)
{

	m_splits = splits;

	if (m_initialized)
	{
		Destroy();
		Create();
	}

}

void DX11VarianceShadowMap::SetExponentialTest(bool exponentialTest)
{

	m_exponentialTest = exponentialTest;

	if (m_initialized)
	{
		Destroy();
		Create();
	}

}

bool DX11VarianceShadowMap::GetExponentialTest(void) const
{
	return m_exponentialTest;
}

void DX11VarianceShadowMap::SetBlurKernelSize(int blur)
{
	m_blurKernelSize = blur;
	__CreateShaders();
}

int DX11VarianceShadowMap::GetBlurKernelSize(void) const
{
	return m_blurKernelSize;
}

DX11ShaderResource & DX11VarianceShadowMap::GetVSM(void)
{
	return m_vsmDepth[1];
}

bool DX11VarianceShadowMap::__CreateShaders(void)
{

	String resolution = ToString(m_resolution);

	m_vsmGeneration = ResourceTypeManager::GetSingleton().CreateResource<DX11ShaderProgram>("DX11Shader",
	                                                                                        "./shaders/vsm_generation.msh",
	                                                                                        nullptr,
	                                                                                        { { "type", "program" } });

	m_blur[0] = ResourceTypeManager::GetSingleton().CreateResource<DX11ShaderProgram>("DX11Shader",
	                                                                                  "./shaders/vsm_blur_h.msh",
	                                                                                  nullptr,
	                                                                                  { { "type", "program" } });

	m_blur[1] = ResourceTypeManager::GetSingleton().CreateResource<DX11ShaderProgram>("DX11Shader",
	                                                                                  "./shaders/vsm_blur_v.msh",
	                                                                                  nullptr,
	                                                                                  { { "type", "program" } });

	if (m_exponentialTest)
	{

		m_vsmGeneration->AddMacroDefinition("MYE_SHADOW_MAP_EVSM");

		m_blur[0]->AddMacroDefinition("MYE_BOX_BLUR_TYPE", "float4");
		m_blur[1]->AddMacroDefinition("MYE_BOX_BLUR_TYPE", "float4");

	}
	else
	{

		m_vsmGeneration->RemoveMacroDefinition("MYE_SHADOW_MAP_EVSM");

		m_blur[0]->AddMacroDefinition("MYE_BOX_BLUR_TYPE", "float2");
		m_blur[1]->AddMacroDefinition("MYE_BOX_BLUR_TYPE", "float2");

	}

	if (m_blurKernelSize > 0)
	{
		auto blurSize = ToString(m_blurKernelSize);
		m_blur[0]->AddMacroDefinition("MYE_BOX_BLUR_KERNEL_SIZE", blurSize.CString());
		m_blur[1]->AddMacroDefinition("MYE_BOX_BLUR_KERNEL_SIZE", blurSize.CString());
	}

	return m_vsmGeneration->Load() &&
	       m_blur[0]->Load() && m_blur[1]->Load();

}

bool DX11VarianceShadowMap::__CreateRenderTargets(void)
{

	Parameters params0 = { { "renderTarget", "true" } };

	if (m_splits > 1)
	{
		params0.Add("slices", ToString(m_splits));
	}

	Parameters params1 = params0;
	params1.Add("generateMips", "true");

	m_vsmDepth[0].SetParametersList(params0);
	m_vsmDepth[1].SetParametersList(params1);

	DataFormat data = (m_exponentialTest ? DataFormat::FLOAT4 : DataFormat::FLOAT2);

	return m_vsmDepth[0].Create(m_resolution, m_resolution, data) &&
	       m_vsmDepth[1].Create(m_resolution, m_resolution, data);

}

void DX11VarianceShadowMap::__DestroyRenderTargets(void)
{
	m_vsmDepth[0].Destroy();
	m_vsmDepth[1].Destroy();
}