#include "lpv.hlsli"

struct VSInput
{
	uint vertexID : SV_VertexID;
	uint depth    : SV_InstanceID;
};

struct VSOutput
{
	float4 positionCS : SV_Position;
	uint   depth      : SV_InstanceID;
	//float3 cell       : CELL;
};

VSOutput main(VSInput input)
{

	/*uint c  = g_lpv.lpvResolution;
	uint c2 = c * c;
	uint t  = (input.vertexID % c2);

	uint i  = input.vertexID / c2;
	uint j  = t / c;
	uint k  = t % c;*/

	VSOutput output;

	if (input.vertexID == 0)
	{
		output.positionCS = float4(-1, -1, .5f, 1);
	}
	else if (input.vertexID == 1)
	{
		output.positionCS = float4(-1, 1, .5f, 1);
	}
	else if (input.vertexID == 2)
	{
		output.positionCS = float4(1, -1, .5f, 1);
	}
	else
	{
		output.positionCS = float4(1, 1, .5f, 1);
	}

	output.depth = input.depth;

	return output;

	/*uint i = input.vertexID / g_lpv.lpvResolution;
	uint j = input.vertexID % g_lpv.lpvResolution;
	uint k = input.depth;

	VSOutput output;

	output.cell       = float3(i, j, k);

	float2 cellCS     = 2.f * (output.cell.xy + .5f) / g_lpv.lpvResolution - 1.f;

	output.positionCS = float4(cellCS.x, - cellCS.y, 0.f, 1.f);

	return output;*/

}