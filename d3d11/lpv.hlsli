#ifndef __MYE_LPV__
#define __MYE_LPV__

#include "rsm_read.hlsli"

float3 LPVGetGridCell(in float3 position)
{
	
	float4 positionCS  = mul(g_cameraViewProj, float4(position, 1));
	float3 positionNDC = positionCS.xyz / positionCS.w;

	return 32 * float3(.5f * (positionNDC.xy + 1), positionNDC.z);

}

#endif