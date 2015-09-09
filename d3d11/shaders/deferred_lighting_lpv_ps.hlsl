#pragma pack_matrix(row_major)

#include "gamma.hlsli"
#include "gbuffer_read.hlsli"
#include "register_slots.hlsli"
#include "lpv.hlsli"
#include "spherical_harmonics.hlsli"

/* Constant Buffers */

struct PSInput
{
	float4 positionCS : SV_position;
};

Texture3D<float4> g_lightVolumeRed   : register(__MYE_DX11_TEXTURE_SLOT_LPVLIGHT_RED);
Texture3D<float4> g_lightVolumeGreen : register(__MYE_DX11_TEXTURE_SLOT_LPVLIGHT_GREEN);
Texture3D<float4> g_lightVolumeBlue  : register(__MYE_DX11_TEXTURE_SLOT_LPVLIGHT_BLUE);
Texture3D<float4> g_geometryVolume   : register(__MYE_DX11_TEXTURE_SLOT_LPVGEOMETRY);

SamplerState      g_lpvSampler : register(__MYE_DX11_SAMPLER_SLOT_LPV);

/* Main */

float4 main(PSInput input) : SV_Target0
{

	LPV lpv = { g_lightVolumeRed, g_lightVolumeGreen, g_lightVolumeBlue, g_geometryVolume, g_lpvSampler };

	GBufferData data = GBufferRead(input.positionCS.xy);

	float4 shNormal = SHCosineLobe(- data.normal);

	//int3   cellCoords = LPVGetGridCell(data.position);
	//float3 texCoords  = float3(cellCoords.xyz) / g_lpv.lpvResolution;
	//float3 texCoords  = (data.position - g_lpv.minCorner) / g_lpv.cellSize / g_lpv.lpvResolution;

	float3 cell         = (data.position - g_lpv.minCorner) / g_lpv.cellSize;
	float3 sampleCoords = cell / g_lpv.lpvResolution;
	float4 shOcclusion  = LPVOcclusion(lpv, cell, float3(0, 0, 0));

	SHRGB sh;

	//sh.red   = g_lightVolumeRed.Load   (int4(cell, 0));
	//sh.green = g_lightVolumeGreen.Load (int4(cell, 0));
	//sh.blue  = g_lightVolumeBlue.Load  (int4(cell, 0));

	sh.red   = g_lightVolumeRed.Sample(g_lpvSampler,   sampleCoords);
	sh.green = g_lightVolumeGreen.Sample(g_lpvSampler, sampleCoords);
	sh.blue  = g_lightVolumeBlue.Sample(g_lpvSampler,  sampleCoords);

	float3 irradiance = max(0, SHDot(sh, shNormal)) * MYE_INV_PI;
	float  visibility = LPVVisibility(sh.red, shOcclusion);

	// As in the crytek paper, approximate the distance r from the surfel as half the
	// cell size, and thus calculate the irradiance like I/r^2

	float falloff = 4.f / (g_lpv.cellSize * g_lpv.cellSize);

	//return float4(visibility * intensity * falloff, 1.f);

	return float4(irradiance * falloff, 1.f);

}