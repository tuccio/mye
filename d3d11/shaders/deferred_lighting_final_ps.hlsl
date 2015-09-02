#pragma pack_matrix(row_major)

#include "constants.hlsli"
#include "gamma.hlsli"
#include "material.hlsli"
#include "register_slots.hlsli"
#include "common_samplers.hlsli"

/* Constant buffers */

cbuffer cbMaterial : register(__MYE_DX11_BUFFER_SLOT_MATERIAL)
{
	Material g_material;
};

#ifdef MYE_USE_DIFFUSE_TEXTURE

Texture2D    g_diffuseTexture : register(__MYE_DX11_TEXTURE_SLOT_DIFFUSE);

#endif

/* Input/Output structures */

struct PSInput
{
	float4 positionCS : SV_Position;
	float2 texcoord   : TEXCOORD;
};

/* Buffers */

Texture2D g_lightbuffer : register(__MYE_DX11_TEXTURE_SLOT_LIGHTBUFFER);

/* Main */

float4 main(PSInput input) : SV_TARGET
{

	int3 screenPosition = int3(input.positionCS.xy, 0);

	float4 lighting = g_lightbuffer.Load(screenPosition);

	float3 diffuseColor;

#ifdef MYE_USE_DIFFUSE_TEXTURE
	diffuseColor = Gamma(g_diffuseTexture.Sample(g_anisotropicSampler, input.texcoord));
#else
	diffuseColor = Gamma(g_material.diffuseColor);
#endif

	float3 diffuse  = saturate(diffuseColor * MYE_INV_PI * lighting.xyz);
	float3 specular = g_material.specularColor * lighting.w;

	//output.color = float4(saturate(diffuse + specular), 1);
	//return float4(.5f * input.texcoord + .5f, 0, 1);
	return float4(diffuse, 1.f);

}