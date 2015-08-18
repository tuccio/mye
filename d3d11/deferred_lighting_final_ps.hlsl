#pragma pack_matrix(row_major)

#include "constants.hlsli"
#include "gamma.hlsli"
#include "material.hlsli"
#include "register_slots.hlsli"

/* Constant buffers */

cbuffer cbMaterial : register(__MYE_DX11_BUFFER_SLOT_MATERIAL)
{
	Material g_material;
};

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

	float3 diffuse  = saturate(Gamma(g_material.diffuseColor) * MYE_INV_PI * lighting.xyz);
	float3 specular = g_material.specularColor * lighting.w;

	//output.color = float4(saturate(diffuse + specular), 1);

	return float4(diffuse, 1.f);

}