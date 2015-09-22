#ifndef __MYE_RSM_READ__
#define __MYE_RSM_READ__

#include "register_slots.hlsli"
#include "light.hlsli"
#include "common_samplers.hlsli"
#include "luminance.hlsli"

struct RSMTexel
{
	float3 position;
	float  area;
	float3 normal;
	float3 flux;
	//float  depth;
};

Texture2D<float4> g_position : register(__MYE_DX11_TEXTURE_SLOT_RSMPOSITION);
Texture2D<float4> g_normal   : register(__MYE_DX11_TEXTURE_SLOT_RSMNORMAL);
Texture2D<float4> g_flux     : register(__MYE_DX11_TEXTURE_SLOT_RSMFLUX);
//Texture2D<float>  g_depth    : register(__MYE_DX11_TEXTURE_SLOT_SHADOWMAP);

cbuffer cbLight : register(__MYE_DX11_BUFFER_SLOT_LIGHT)
{
	Light g_light;
};

RSMTexel RSMRead(in float2 texcoord)
{

	RSMTexel texel;

	float4 posArea = g_position.SampleLevel(g_pointClampedSampler, texcoord, 0);

	texel.position = posArea.xyz;
	texel.area     = posArea.w;
	texel.normal   = g_normal.SampleLevel(g_pointClampedSampler, texcoord, 0).xyz;
	texel.flux     = g_flux.SampleLevel(g_pointClampedSampler, texcoord, 0).rgb;

	return texel;

}

float RSMTexelLuminance(in RSMTexel texel)
{

	float  luminance = Luminance(texel.flux);
	float  NdotL     = dot(texel.normal, - g_light.direction);

	return luminance * NdotL;

}

#endif