#include "lpv.hlsli"

struct VSInput
{
	uint vertexID : SV_VertexID;
};

struct VSOutput
{
	float4 positionCS : SV_Position;
	float3 cell       : CELL;
};

VSOutput main(VSInput input)
{

	uint c  = g_lpv.lpvResolution;
	uint c2 = c * c;
	uint t  = (input.vertexID % c2);

	uint i  = input.vertexID / c2;
	uint j  = t / c;
	uint k  = t % c;

	VSOutput output;

	output.cell       = float3(i, j, k);

	/*output.positionCS = float4(2.f * (output.cell.xy + .5f) / g_lpv.lpvResolution - 1.f,
	                           0.f,
	                           1.f);*/

	float invLPVResolution = 1.f / g_lpv.lpvResolution;

	output.positionCS = float4(2.f * (.5f + output.cell.x) * invLPVResolution - 1.f,
	                           1.f - 2.f * (.5f + output.cell.y) * invLPVResolution,
	                           0.f,
	                           1.f);

	return output;

}