#ifndef __MYE_SHADOW_MAPPING_PCSS__
#define __MYE_SHADOW_MAPPING_PCSS__

#pragma pack_matrix(row_major)

#ifndef __MYE_SHADOW_MAP_BIAS
#define __MYE_SHADOW_MAP_BIAS (0.0001f)
#endif

#include "light.hlsli"
#include "register_slots.hlsli"
#include "renderer_configuration.hlsli"

cbuffer cbLightSpace : register(__MYE_DX11_BUFFER_SLOT_LIGHTSPACETRANSFORM)
{
	float4x4 g_lightSpaceTransform;
}


cbuffer cbLight : register(__MYE_DX11_BUFFER_SLOT_LIGHT)
{
	Light g_light;
}

cbuffer cbPoissonDisk
{
	float2 poissonDisk[16] = {
		float2(-0.94201624, -0.39906216),
		float2(0.94558609, -0.76890725),
		float2(-0.094184101, -0.92938870),
		float2(0.34495938, 0.29387760),
		float2(-0.91588581, 0.45771432),
		float2(-0.81544232, -0.87912464),
		float2(-0.38277543, 0.27676845),
		float2(0.97484398, 0.75648379),
		float2(0.44323325, -0.97511554),
		float2(0.53742981, -0.47373420),
		float2(-0.26496911, -0.41893023),
		float2(0.79197514, 0.19090188),
		float2(-0.24188840, 0.99706507),
		float2(-0.81409955, 0.91437590),
		float2(0.19984126, 0.78641367),
		float2(0.14383161, -0.14100790)
	};

};

/* Shadow map and sampler */


Texture2D<float>       g_shadowMap           : register(__MYE_DX11_TEXTURE_SLOT_SHADOWMAP);
										     
SamplerState           g_shadowMapSampler    : register(__MYE_DX11_SAMPLER_SLOT_SHADOWMAP);
SamplerComparisonState g_shadowMapSamplerCmp : register(__MYE_DX11_SAMPLER_SLOT_SHADOWMAP_CMP);


/* Shadow mapping */

#define LIGHT_SOFTNESS 1.0f

void FindBlocker(float2 uv, float zReceiver, out float avgBlockerDepth, out float numBlockers)
{

	//float searchWidth = LIGHT_SOFTNESS * (zReceiver - )

}

float ShadowMapVisibility(in float3 x)
{

	float bias = __MYE_SHADOW_MAP_BIAS;

	float4 lightSpacePosition = mul(g_lightSpaceTransform, float4(x, 1));

	float2 projectTexCoord;

	projectTexCoord.x = (lightSpacePosition.x / lightSpacePosition.w) * 0.5f + 0.5f;
	projectTexCoord.y = 1.0f - ((lightSpacePosition.y / lightSpacePosition.w) * 0.5f + 0.5f);

	float lightDepthValue = lightSpacePosition.z - bias;
	float shadowFactor = 0.0f;
	float texelScale = 1.0f / r.shadowMapResolution;

	float jx, jy;

	shadowFactor =
		0.25f * (
		g_shadowMap.SampleCmpLevelZero(g_shadowMapSamplerCmp, projectTexCoord + float2(-1.5f, 0.5f) * texelScale, lightDepthValue) +
		g_shadowMap.SampleCmpLevelZero(g_shadowMapSamplerCmp, projectTexCoord + float2(0.5f, 0.5f) * texelScale, lightDepthValue) +
		g_shadowMap.SampleCmpLevelZero(g_shadowMapSamplerCmp, projectTexCoord + float2(-1.5f, -1.5f) * texelScale, lightDepthValue) +
		g_shadowMap.SampleCmpLevelZero(g_shadowMapSamplerCmp, projectTexCoord + float2(0.5f, -1.5f) * texelScale, lightDepthValue));

	return shadowFactor;



}

#endif