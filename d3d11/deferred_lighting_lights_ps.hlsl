#pragma pack_matrix(row_major)

#include "brdf.hlsli"
#include "light.hlsli"
#include "light_directional.hlsli"
#include "material.hlsli"
#include "register_slots.hlsli"
#include "renderer_configuration.hlsli"
//#include "vsm.hlsli"
//#include "shadow_mapping.hlsli"
#include "shadow_mapping_pcss.hlsli"

/* Constant buffers */

#define PI 3.14159265
#define GAMMA 1.8

/* Constant Buffers */

cbuffer cbCamera : register(b1)
{
	float4 g_cameraPosition;
};

/* G-Buffer */

Texture2D g_gbuffer0 : register(__MYE_DX11_TEXTURE_SLOT_GBUFFER0);
Texture2D g_gbuffer1 : register(__MYE_DX11_TEXTURE_SLOT_GBUFFER1);

void ReadGBuffer(in int2 screenPosition, out float3 position, out float3 normal, out float specularPower)
{

	int3 texturePosition = int3(screenPosition.xy, 0);

	float4 gbuffer0 = g_gbuffer0.Load(texturePosition);
	float4 gbuffer1 = g_gbuffer1.Load(texturePosition);

	normal   = gbuffer0.xyz;
	position = gbuffer1.xyz;

	specularPower = gbuffer0.w;

}


float3 Gamma(float3 color)
{
	return pow(color, GAMMA);
}

/* Main */

float4 main(float4 position : SV_Position) : SV_Target0
{

	float3 x;
	float3 N;
	float  specularPower;

	float3 L;
	float3 intensity;

	float4 diffuse;

	ReadGBuffer(position.xy, x, N, specularPower);
	ComputeLightParams(x, g_light, L, intensity);

	float visibility = ShadowMapVisibility(x);

	return float4(visibility * saturate(dot(N, L)) * intensity, 1);

}