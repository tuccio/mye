#pragma pack_matrix(row_major)

#include "constants.hlsli"
#include "material.hlsli"
#include "register_slots.hlsli"

/* Constant buffers */

cbuffer cbMaterial : register(b0)
{
	Material g_material;
};

/* Input/Output structures */

struct PSInput
{
	float4 positionCS : SV_Position;
	float2 texcoord   : TEXCOORD;
};

struct PSOutput
{
	float4 color : SV_Target0;
};

/* Buffers */

Texture2D g_lightbuffer : register(t0);

/* Main */

PSOutput main(PSInput input) : SV_TARGET
{

	PSOutput output;

	int3 screenPosition = int3(input.positionCS.xy, 0);

	float4 lighting = g_lightbuffer.Load(screenPosition);

	float3 diffuse  = g_material.diffuseColor  * lighting.xyz;
	float3 specular = g_material.specularColor * lighting.w;

	//output.color = float4(saturate(diffuse + specular), 1);

	output.color = saturate(float4(g_material.diffuseColor * lighting.xyz * MYE_INV_PI, 1));

	return output;

}