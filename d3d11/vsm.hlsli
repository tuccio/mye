#ifndef __MYE_VSM__
#define __MYE_VSM__

#include "light.hlsli"
#include "light_directional.hlsli"
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


/* Shadow map and sampler */

Texture2D    g_shadowMap        : register(__MYE_DX11_TEXTURE_SLOT_SHADOWMAP);
SamplerState g_shadowMapSampler : register(__MYE_DX11_SAMPLER_SLOT_SHADOWMAP);

SamplerComparisonState g_shadowMapSamplerCmp : register(__MYE_DX11_SAMPLER_SLOT_SHADOWMAP_CMP);

/* Variance Shadow mapping */

float ChebyshevUpperBound(float2 moments, float t)
{

	float p = (t <= moments.x);

	float variance = moments.y - moments.x * moments.x;
	variance = min(variance, r.vsmMinVariance);

	float d = t - moments.x;

	float pmax = variance / (variance + d * d);

	return max(p, pmax);

}

float ShadowMapVisibility(in float3 x)
{
	
	float4 worldSpacePosition = float4(x, 1);
	float4 lightSpacePosition = mul(g_lightSpaceTransform, worldSpacePosition);

	float2 projectTexCoord;

	projectTexCoord.x = (lightSpacePosition.x / lightSpacePosition.w) * 0.5f + 0.5f;
	projectTexCoord.y = 1.0f - ((lightSpacePosition.y / lightSpacePosition.w) * 0.5f + 0.5f);

	float2 moments = g_shadowMap.Sample(g_shadowMapSampler, projectTexCoord).xy;

	float pmax = ChebyshevUpperBound(moments, LightSpaceLinearDepth(g_light, lightSpacePosition, worldSpacePosition));
	return smoothstep(r.vsmMinBleeding, 1.0f, pmax);

}

#endif