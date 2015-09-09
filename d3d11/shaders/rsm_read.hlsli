#ifndef __MYE_RSM_READ__
#define __MYE_RSM_READ__

#include "register_slots.hlsli"
#include "light.hlsli"
#include "common_samplers.hlsli"

struct RSMTexel
{
	float3 position;
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

#ifdef MYE_VERTEX_SHADER

	texel.position = g_position.SampleLevel(g_bilinearSampler, texcoord, 0).xyz;
	texel.normal   = g_normal.SampleLevel(g_bilinearSampler, texcoord, 0).xyz;
	texel.flux     = g_flux.SampleLevel(g_bilinearSampler, texcoord, 0).rgb;

#else

	texel.position = g_position.Sample(g_trilinearSampler, texcoord).xyz;
	texel.normal   = g_normal.Sample(g_trilinearSampler, texcoord).xyz;
	texel.flux     = g_flux.Sample(g_trilinearSampler, texcoord).rgb;

#endif

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