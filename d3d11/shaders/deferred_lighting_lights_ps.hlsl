#pragma pack_matrix(row_major)

#include "light.hlsli"
#include "light_directional.hlsli"
#include "shadow_mapping.hlsli"
#include "gamma.hlsli"
#include "gbuffer_read.hlsli"

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
	float3 NdotL      = (dot(data.normal, L));

	return visibility * float4(NdotL * LightIrradiance(g_light, data.position), 1);

}