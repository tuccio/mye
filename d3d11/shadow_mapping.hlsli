#ifndef __MYE_SHADOW_MAPPING__
#define __MYE_SHADOW_MAPPING__

#pragma pack_matrix(row_major)

#ifndef __MYE_SHADOW_MAP_BIAS
#define __MYE_SHADOW_MAP_BIAS (0.0001f)
#endif

#include "light.hlsli"
#include "register_slots.hlsli"
#include "renderer_configuration.hlsli"

//#define MYE_SHADOW_MAPPING_PCF

cbuffer cbLightSpace : register(__MYE_DX11_BUFFER_SLOT_LIGHTSPACETRANSFORM)
{
	float4x4 g_lightSpaceTransform;
}


cbuffer cbLight : register(__MYE_DX11_BUFFER_SLOT_LIGHT)
{
	Light g_light;
}

/* Shadow map and sampler */

Texture2D<float>       g_shadowMap           : register(__MYE_DX11_TEXTURE_SLOT_SHADOWMAP);
SamplerState           g_shadowMapSampler    : register(__MYE_DX11_SAMPLER_SLOT_SHADOWMAP);

SamplerComparisonState g_shadowMapSamplerCmp : register(__MYE_DX11_SAMPLER_SLOT_SHADOWMAP_CMP);


/* Shadow mapping */

float ShadowMapVisibility(in float3 x)
{

	float bias = __MYE_SHADOW_MAP_BIAS;

	float4 lightSpacePosition = mul(g_lightSpaceTransform, float4(x, 1));

	float2 projectTexCoord;

	projectTexCoord.x = (lightSpacePosition.x / lightSpacePosition.w) * 0.5f + 0.5f;
	projectTexCoord.y = 1.0f - ((lightSpacePosition.y / lightSpacePosition.w) * 0.5f + 0.5f);



#ifdef MYE_SHADOW_MAPPING_PCF

	float lightDepthValue = lightSpacePosition.z - bias;	
	float shadowFactor = 0.0f;
	float texelScale = 1.0f / r.shadowMapResolution;

	float jx, jy;

	shadowFactor = 
		0.25f * (
		g_shadowMap.SampleCmpLevelZero(g_shadowMapSamplerCmp, projectTexCoord + float2(-1.5f,  0.5f) * texelScale, lightDepthValue) +
		g_shadowMap.SampleCmpLevelZero(g_shadowMapSamplerCmp, projectTexCoord + float2( 0.5f,  0.5f) * texelScale, lightDepthValue) +
		g_shadowMap.SampleCmpLevelZero(g_shadowMapSamplerCmp, projectTexCoord + float2(-1.5f, -1.5f) * texelScale, lightDepthValue) +
		g_shadowMap.SampleCmpLevelZero(g_shadowMapSamplerCmp, projectTexCoord + float2( 0.5f, -1.5f) * texelScale, lightDepthValue));

	return shadowFactor;

#else

	float lightDepthValue = lightSpacePosition.z - bias;
	float depthValue = g_shadowMap.Sample(g_shadowMapSampler, projectTexCoord);

	if (depthValue < lightDepthValue)
	{
		return 0.0f;
	}
	else
	{
		return 1.0f;
	}

#endif

}

#endif