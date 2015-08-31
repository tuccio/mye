#pragma pack_matrix(row_major)

#include "material.hlsli"
#include "light.hlsli"
#include "light_directional.hlsli"
#include "register_slots.hlsli"
#include "renderer_configuration.hlsli"

#define MYE_CONE90_SOLID_ANGLE 2.0943951f

/* Constant buffers */

cbuffer cbLight : register(__MYE_DX11_BUFFER_SLOT_LIGHT)
{
	Light g_light;
};

cbuffer cbMaterial : register(__MYE_DX11_BUFFER_SLOT_MATERIAL)
{
	Material g_material;
};

/* Input/Output structures */

struct PSInput
{

	float4 positionCS : SV_Position;
	float2 texcoord   : TEXCOORD;
	float3 normalWS   : NORMALWS;
	float3 positionWS : POSITIONWS;

};

struct PSOutput
{
	float4 position : SV_Target0;
	float4 normal   : SV_Target1;
	float4 flux     : SV_Target2;
};

/* Main */

PSOutput main(PSInput input)
{

	float3 N = normalize(input.normalWS);
	float3 L = LightVector(g_light, input.positionWS);

	float4 NdotL = saturate(dot(N, L));

	float  omega      = MYE_CONE90_SOLID_ANGLE / (r.shadowMapResolution * r.shadowMapResolution);
	float3 irradiance = LightIrradiance(g_light, input.positionWS);

	PSOutput output;

	output.position = float4(input.positionWS, 1);
	output.normal   = float4(N, 1);
	output.flux     = float4(irradiance * MYE_INV_PI * g_material.diffuseColor * NdotL * omega, 1);

	return output;

}