#pragma pack_matrix(row_major)

#include "light.hlsli"
#include "light_directional.hlsli"
#include "shadow_mapping.hlsli"
#include "gamma.hlsli"
#include "gbuffer_read.hlsli"
#include "camera_transform.hlsli"
#include "quad_input.hlsli"

/* Constant Buffers */

struct PSOutput
{
	float4 diffuse : SV_Target0;
	float4 specular : SV_Target1;
};

/* Main */

PSOutput main(QuadInput input)
{

	GBufferData data = GBufferRead(input.positionCS.xy);

	float3 L = LightVector(g_light, data.position);

	float4 visibility = ShadowMapVisibility(data);
	float  NdotL      = saturate(dot(data.normal, L));

	float3 V          = normalize(g_camera.position - data.position);
	float3 H          = normalize(L + V);
	float  NdotH      = saturate(dot(data.normal, H));

	float3 irradiance = LightIrradiance(g_light, data.position);

	PSOutput output;

	output.diffuse  = float4(visibility * NdotL * irradiance, 0.f);
	output.specular = float4(visibility * NdotL * pow(NdotH, data.specularPower) * irradiance, 0.f);

	return output;

}