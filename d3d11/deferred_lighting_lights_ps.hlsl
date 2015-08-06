#pragma pack_matrix(row_major)

#include "constants.hlsli"
#include "brdf.hlsli"
#include "light.hlsli"
#include "light_directional.hlsli"
#include "material.hlsli"
#include "register_slots.hlsli"
#include "renderer_configuration.hlsli"
//#include "vsm.hlsli"
#include "shadow_mapping.hlsli"
#include "gbuffer_read.hlsli"
//#include "shadow_mapping_pcss.hlsli"

/* Constant Buffers */

cbuffer cbCamera : register(b1)
{
	float4 g_cameraPosition;
};

float3 Gamma(float3 color)
{
	return pow(color, r.gamma);
}

struct PSInput
{
	float4 positionCS : SV_position;
};

/* Main */

float4 main(PSInput input) : SV_Target0
{

	GBufferData data = GBufferRead(input.positionCS.xy);

	float3 L = LightVector(data.position, g_light);

	float4 visibility = ShadowMapVisibility(data);
	float4 NdotL      = saturate(dot(data.normal, L));

	return visibility * NdotL * g_light.color * g_light.intensity;

}