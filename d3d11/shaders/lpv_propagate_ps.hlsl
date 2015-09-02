#include "lpv.hlsli"
#include "register_slots.hlsli"

#include "spherical_harmonics.hlsli"

struct PSInput
{
	float4 positionCS : SV_Position;
	uint   depth      : SV_RenderTargetArrayIndex;
	float3 cell       : CELL;
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

SHRGB GatherContribution(in LPV lpv,
                         in int3 cell)
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

	static const float2 sideProjDirection[4] = {
			{  1, 0 }, { -1, 0 }, { 0, 1 }, { 0, -1 }
	};

	[unroll]
	for (int i = 0; i < 6; i++)
	{

		float3 fluxDirection     = mul(neighbors[i], float3(0, 0, 1));
		float3 sourceDirection   = - fluxDirection;
							    
		float4 shOcclusion       = LPVOcclusion(lpv, cell, sourceDirection);

		float4 shFluxDirection   = SHCosineLobe(fluxDirection);
		float4 shSourceDirection = SHCosineLobe(sourceDirection);

		SHRGB  shAdjRadiance     = LPVLoadOffset(lpv, cell, sourceDirection);
							     
		float3 shIrradiance      = saturate(SHDot(shAdjRadiance, shFluxDirection));
		//float  occlusion         = LPVVisibility(shSourceDirection, shOcclusion);
		float  occlusion         = LPVVisibility(shFluxDirection, shOcclusion);

		SHRGB  shFrontFlux       = SHScale(shFluxDirection, MYE_LPV_FRONT_SOLID_ANGLE * MYE_INV_PI * occlusion * shIrradiance);

		sh = SHAdd(sh, shFrontFlux);

		[unroll]
		for (int j = 0; j < 4; j++)
		{

			float3 sideFluxDirection   = mul(neighbors[i], sideDirection[j]);
			float3 fluxProjDirection   = mul(neighbors[i], float3(sideProjDirection[j], 0));

			float4 shSideFluxDirection = SHCosineLobe(sideFluxDirection);
			float4 shSideProjDirection = SHCosineLobe(fluxProjDirection);

			float3 shSideIrradiance    = saturate(SHDot(shAdjRadiance, shSideFluxDirection));
			//float  sideOcclusion       = LPVVisibility(shSourceDirection, shOcclusion);

			float  sideOcclusion       = LPVVisibility(SHCosineLobe(-sideFluxDirection), shOcclusion);
			//float  sideOcclusion       = LPVVisibility(SHCosineLobe(sideFluxDirection), LPVOcclusion(lpv, cell, - sideFluxDirection));

			SHRGB  shSideFlux          = SHScale(shSideProjDirection, MYE_LPV_SIDE_SOLID_ANGLE * MYE_INV_PI * sideOcclusion * shSideIrradiance);

			sh = SHAdd(sh, shSideFlux);

		}

	}

	//return sh;

	// Divide by pi when reprojecting (crytek paper section 3.3)

	return SHScale(sh, MYE_INV_PI);

}


PSOutput main(PSInput input)
{

	LPV lpv = { g_lightVolumeRed, g_lightVolumeGreen, g_lightVolumeBlue, g_geometryVolume, g_lpvSampler };

	SHRGB sh = LPVLoadOffset(lpv, input.cell, int3(0, 0, 0));

	sh = SHAdd(sh, GatherContribution(lpv, input.cell));

	PSOutput output;

	output.shR = sh.red;
	output.shG = sh.green;
	output.shB = sh.blue;

	return output;

}