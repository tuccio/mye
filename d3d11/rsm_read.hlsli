#ifndef __MYE_RSM_READ__
#define __MYE_RSM_READ__

#include "register_slots.hlsli"
#include "light.hlsli"

struct RSMTexel
{
	float3 position;
	float3 normal;
	float4 flux;
	//float  depth;
};

Texture2D<float4> g_position : register(__MYE_DX11_TEXTURE_SLOT_RSMPOSITION);
Texture2D<float4> g_normal   : register(__MYE_DX11_TEXTURE_SLOT_RSMNORMAL);
Texture2D<float4> g_flux     : register(__MYE_DX11_TEXTURE_SLOT_RSMALBEDO);
//Texture2D<float>  g_depth    : register(__MYE_DX11_TEXTURE_SLOT_SHADOWMAP);

SamplerState g_linearSampler : register(__MYE_DX11_SAMPLER_SLOT_LINEAR);

cbuffer cbLight : register(__MYE_DX11_BUFFER_SLOT_LIGHT)
{
	Light g_light;
};

RSMTexel RSMRead(in float2 texcoord)
{

	RSMTexel texel;

	texel.position = g_position.Sample(g_linearSampler, texcoord).xyz;
	texel.normal   = g_normal.Sample(g_linearSampler,   texcoord).xyz;
	texel.flux     = g_flux.Sample(g_linearSampler, texcoord);

	return texel;

}

float RSMTexelLuminance(in RSMTexel texel)
{

	float  luminance = 0.2126 * texel.flux.r + 0.7152 * texel.flux.g + 0.0722 * texel.flux.b;
	float3 L         = - g_light.direction.xyz;
	float  NdotL     = saturate(dot(texel.normal, L));

	return luminance * NdotL;

}

#endif