#ifndef __MYE_QUAD_INPUT__
#define __MYE_QUAD_INPUT__

struct QuadInput
{
	float4 positionCS : SV_Position;
	float2 texcoord   : TEXCOORD;
	uint   instance   : SV_InstanceID;
};

#endif