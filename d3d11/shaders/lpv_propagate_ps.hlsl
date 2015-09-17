#include "lpv.hlsli"
#include "register_slots.hlsli"

#include "spherical_harmonics.hlsli"

struct PSInput
{
	float4 positionCS : SV_Position;
	uint   depth      : SV_RenderTargetArrayIndex;
	//float3 cell       : CELL;
};

struct PSOutput
{
	float4 shR : SV_Target0;
	float4 shG : SV_Target1;
	float4 shB : SV_Target2;
};

Texture3D<float4> g_lightVolumeRed   : register(__MYE_DX11_TEXTURE_SLOT_LPVLIGHT_RED);
Texture3D<float4> g_lightVolumeGreen : register(__MYE_DX11_TEXTURE_SLOT_LPVLIGHT_GREEN);
Texture3D<float4> g_lightVolumeBlue  : register(__MYE_DX11_TEXTURE_SLOT_LPVLIGHT_BLUE);
Texture3D<float4> g_geometryVolume   : register(__MYE_DX11_TEXTURE_SLOT_LPVGEOMETRY);

SamplerState      g_lpvSampler       : register(__MYE_DX11_SAMPLER_SLOT_LPV);

#define MYE_LPV_FRONT_SOLID_ANGLE (0.4006696846f)
#define MYE_LPV_SIDE_SOLID_ANGLE  (0.4234413544f)

SHRGB GatherContribution(in LPV lpv, in float3 cell)
{

	SHRGB sh = (SHRGB) 0;

	static const float3x3 neighbors[6] = {

		// z

			{
				 1,  0,  0,
				 0,  1,  0,
				 0,  0,  1
			},

		// -z

			{
				-1,  0,  0,
				 0,  1,  0,
				 0,  0, -1
			},

		// y

			{
				 1,  0,  0,
				 0,  0,  1,
				 0, -1,  0
			},

		// -y

			{
				 1,  0,  0,
				 0,  0, -1,
				 0,  1,  0
			},

		// x

			{
				 0,  0,  1,
				 0,  1,  0,
				-1,  0,  0
			},

		// -x

			{
				 0,  0, -1,
				 0,  1,  0,
				 1,  0,  0
			}

	};

	static const float3 sideDirection[4] = {

		// x
			{  0.447213595, 0, 0.894427191 },

		// -x
			{ -0.447213595, 0, 0.894427191 },

		// y
			{ 0,  0.447213595, 0.894427191 },

		// -y
			{ 0, -0.447213595, 0.894427191 }

	};

	static const float2 sideReprojDirection[4] = {
			{  1, 0 }, { -1, 0 }, { 0, 1 }, { 0, -1 }
	};

	// Gather all the flux coming from the 6 main directions of the cube (the ones orthogonal to the faces)

	[unroll]
	for (int i = 0; i < 6; i++)
	{

		// For each main direction, read the intensity SH cofficients coming from it
		// by reading the corresponding neighbor LPV cell, then reconstruct the intensity
		// and reproject it.

		float3 mainDirection   = mul(neighbors[i], float3(0, 0, 1));
		float4 mainDirectionSH = SHCosineLobe(mainDirection);

		SHRGB  neighborSH      = LPVLoadOffset(lpv, cell, - mainDirection);

		float4 mainOcclusionSH = LPVOcclusion(lpv, cell, - mainDirection);
		float  mainVisibility  = LPVVisibility(mainOcclusionSH, mainDirection);

		SHRGB  mainIntensitySH = SHScale(mainDirectionSH, mainVisibility * saturate(SHReconstruct(neighborSH, mainDirection)) * MYE_LPV_FRONT_SOLID_ANGLE * MYE_INV_PI);

		sh = SHAdd(sh, mainIntensitySH);

		// Now we consider the light going through the remaining 4 faces the neighbor
		// flux can traverse and reproject in directions orthogonal to the cube faces.

		[unroll]
		for (int j = 0; j < 4; j++)
		{

			float3 sideFluxDirection = mul(neighbors[i], sideDirection[j]);
			float3 reprojDirection   = mul(neighbors[i], float3(sideReprojDirection[j], 0));

			float4 reprojDirectionSH = SHCosineLobe(reprojDirection);

			float  sideOcclusionSH   = LPVOcclusion(lpv, cell, - sideFluxDirection);
			float4 sideVisibility    = LPVVisibility(sideOcclusionSH, sideFluxDirection);

			SHRGB  sideIntensitySH   = SHScale(reprojDirectionSH, sideVisibility * saturate(SHReconstruct(neighborSH, sideFluxDirection)) * MYE_LPV_SIDE_SOLID_ANGLE * MYE_INV_PI);

			sh = SHAdd(sh, sideIntensitySH);

		}

	}

	return SHScale(sh, MYE_INV_PI);

}


PSOutput main(PSInput input)
{


	float3 cell = { input.positionCS.xy, input.depth + .5f };

	LPV lpv = { g_lightVolumeRed, g_lightVolumeGreen, g_lightVolumeBlue, g_geometryVolume, g_lpvSampler };

	SHRGB sh = (SHRGB) 0;
	
#ifndef MYE_PROPAGATE_SKIP_SELF
	sh = SHAdd(sh, LPVLoadOffset(lpv, cell, int3(0, 0, 0)));
#endif

	SHRGB shContribution = GatherContribution(lpv, cell);

	sh = SHAdd(sh, shContribution);

	PSOutput output;

	output.shR = sh.red;
	output.shG = sh.green;
	output.shB = sh.blue;

	return output;

}