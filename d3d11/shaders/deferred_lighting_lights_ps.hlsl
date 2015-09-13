#pragma pack_matrix(row_major)

#include "light.hlsli"
#include "light_directional.hlsli"
#include "shadow_mapping.hlsli"
#include "gamma.hlsli"
#include "gbuffer_read.hlsli"
#include "camera_transform.hlsli"

/* Constant Buffers */

struct PSInput
{
	float4 positionCS : SV_position;
};

/* Main */

float4 main(PSInput input) : SV_Target0
{

	GBufferData data = GBufferRead(input.positionCS.xy);

	float3 L = LightVector(g_light, data.position);

	float4 visibility = ShadowMapVisibility(data);
	float3 NdotL      = saturate(dot(data.normal, L));

	float3 V          = normalize(g_camera.position - data.position);
	float3 H          = normalize(L + V);
	float  NdotH      = saturate(dot(data.normal, H));

	return visibility * float4(NdotL * LightIrradiance(g_light, data.position), pow(NdotH, data.specularPower));

}