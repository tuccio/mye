#pragma pack_matrix(row_major)

#include "constants.hlsli"
#include "gbuffer_read.hlsli"
#include "renderer_configuration.hlsli"
#include "register_slots.hlsli"
#include "common_samplers.hlsli"
#include "camera_transform.hlsli"

#define __MYE_RANDOM_TEXTURE_SIZE 8

/*
 * SSAO shader, needs some macros to be defined:
 * MYE_SSAO_RADIUS: Kernel size (hemisphere radius)
 * MYE_SSAO_MAX_DISTANCE: Maximum distance of influence for the occlusion
 */

#define MYE_SSAO_SAMPLES 16

/* Constant Buffers */

struct PSInput
{
	float4 positionCS : SV_position;
	float2 texcoord   : TEXCOORD;
};

Texture2D<float2> g_randomCosSin : register(__MYE_DX11_TEXTURE_SLOT_RANDOM);
Texture2D<float>  g_depth        : register(__MYE_DX11_TEXTURE_SLOT_GBUFFER2);

/* Main */

float main(PSInput input) : SV_Target0
{

	float2 rotationCosSin = g_randomCosSin.Sample(g_pointSampler, input.positionCS.xy / __MYE_RANDOM_TEXTURE_SIZE);

	float2x2 rotation = {
		rotationCosSin.x, - rotationCosSin.y,
		rotationCosSin.y,   rotationCosSin.x
	};

	GBufferData data = GBufferSample(input.texcoord);

	static float2 poissonDisc[MYE_SSAO_SAMPLES] = {
		{ -0.1007749f, -0.5127298f },
		{ -0.6928392f, -0.6348424f },
		{ 0.3616422f, -0.7251318f },
		{ 0.5150201f, -0.160371f },
		{ -0.3995895f, 0.245256f },
		{ -0.09641803f, -0.9655263f },
		{ 0.00518492f, -0.0838685f },
		{ -0.5439374f, -0.1999681f },
		{ 0.5214711f, 0.4929286f },
		{ -0.05849996f, 0.6183367f },
		{ 0.988231f, 0.1394065f },
		{ 0.3690962f, 0.908119f },
		{ -0.4700805f, 0.8261935f },
		{ -0.8795773f, 0.4534842f },
		{ -0.9701194f, -0.1198528f },
		{ 0.825885f, -0.5506428f }
	};

	float  occlusion = 0.f;
	float2 maxOffset = (float) MYE_SSAO_RADIUS / r.resolution;
	
	float  invMaxDistance = 1.f / MYE_SSAO_MAX_DISTANCE;

	[unroll]
	for (int i = 0; i < MYE_SSAO_SAMPLES; i++)
	{

		float2 texcoord        = input.texcoord + maxOffset * mul(rotation, poissonDisc[i]);

		GBufferData sampleData = GBufferSample(texcoord);

		float3 sampleDirection = normalize(sampleData.position - data.position);

		float  NdotS           = max(dot(data.normal, sampleDirection), 0);
		float  distance        = length(sampleDirection);

		occlusion += NdotS * smoothstep(1.f, 0.f, min(length(distance), MYE_SSAO_MAX_DISTANCE) * invMaxDistance);

	}

	float obscurance = 1.f - occlusion / MYE_SSAO_SAMPLES;

	return obscurance;

}