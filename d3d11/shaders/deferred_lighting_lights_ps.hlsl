#pragma pack_matrix(row_major)

#include "light.hlsli"
#include "light_directional.hlsli"
//#include "vsm.hlsli"
#include "shadow_mapping.hlsli"
//#include "shadow_mapping_pcss.hlsli"
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
	float4 NdotL      = saturate(dot(data.normal, L));

	return float4(visibility * NdotL * LightIrradiance(g_light, data.position), 1);

}