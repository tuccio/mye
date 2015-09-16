#pragma pack_matrix(row_major)

#include "gamma.hlsli"
#include "gbuffer_read.hlsli"
#include "register_slots.hlsli"
#include "lpv.hlsli"
#include "spherical_harmonics.hlsli"
#include "common_samplers.hlsli"
#include "renderer_configuration.hlsli"

/* Constant Buffers */

struct PSInput
{
	float4 positionCS : SV_position;
};

Texture2D<float>  g_occlusion        : register(__MYE_DX11_TEXTURE_SLOT_OCCLUSION);

Texture3D<float4> g_lightVolumeRed   : register(__MYE_DX11_TEXTURE_SLOT_LPVLIGHT_RED);
Texture3D<float4> g_lightVolumeGreen : register(__MYE_DX11_TEXTURE_SLOT_LPVLIGHT_GREEN);
Texture3D<float4> g_lightVolumeBlue  : register(__MYE_DX11_TEXTURE_SLOT_LPVLIGHT_BLUE);

SamplerState      g_lpvSampler : register(__MYE_DX11_SAMPLER_SLOT_LPV);


/* Main */

float4 main(PSInput input) : SV_Target0
{

	GBufferData data = GBufferRead(input.positionCS.xy);

	float4 shNormal = SHCosineLobe(- data.normal);

	//int3   cellCoords = LPVGetGridCell(data.position);
	//float3 texCoords  = float3(cellCoords.xyz) / g_lpv.lpvResolution;
	//float3 texCoords  = (data.position - g_lpv.minCorner) / g_lpv.cellSize / g_lpv.lpvResolution;

	float3 cell         = (data.position - g_lpv.minCorner + .5f) / g_lpv.cellSize;
	float3 sampleCoords = cell / g_lpv.lpvResolution;

	SHRGB sh;

	sh.red   = g_lightVolumeRed.Sample(g_lpvSampler,   sampleCoords);
	sh.green = g_lightVolumeGreen.Sample(g_lpvSampler, sampleCoords);
	sh.blue  = g_lightVolumeBlue.Sample(g_lpvSampler,  sampleCoords);

	float2 texcoord         = input.positionCS.xy / r.resolution;
	float  ambientOcclusion = g_occlusion.Sample(g_bilinearSampler, texcoord);

	// As in the crytek paper, approximate the distance r from the surfel as half the
	// cell size, and thus calculate the irradiance like I/r^2

	float  falloff          = 4.f / (g_lpv.cellSize * g_lpv.cellSize);
	float3 irradiance       = ambientOcclusion * saturate(r.lpvAttenuation * SHDot(sh, shNormal) * falloff);
	//float3 irradiance       = ambientOcclusion * saturate(r.lpvAttenuation * SHReconstruct(sh, shNormal) * falloff);

	return float4(irradiance, 0.f);

}