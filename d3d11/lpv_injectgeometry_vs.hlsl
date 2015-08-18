#include "camera_transform.hlsli"
#include "register_slots.hlsli"
#include "lpv.hlsli"
#include "renderer_configuration.hlsli"

#include "rsm_read.hlsli"


struct VSInput
{
	uint vertexID : SV_VertexID;
};

struct VSOutput
{
	float4 positionCS : SV_Position;
	float3 cell       : CELL;
	float3 normal     : NORMAL;
	float  surfelArea : SURFELAREA;
};

VSOutput main(VSInput input)
{

	uint i = input.vertexID / g_lpv.rsmResolution;
	uint j = input.vertexID % g_lpv.rsmResolution;

	int3 texcoords = int3(i, j, 0);

	float3 position = g_position.Load(texcoords).xyz;
	float4 viewZ    = mul(g_cameraView, float4(position, 1)).z;

	VSOutput output;

	output.normal     = g_normal.Load(texcoords).xyz;
	output.cell       = LPVGetGridCell(position) + .5f * output.normal;

	output.positionCS = float4(2.f * (output.cell.x + .5f) / g_lpv.lpvResolution - 1.f,
	                           1.f - 2.f * (output.cell.y + .5f) / g_lpv.lpvResolution,
	                           0.f,
	                           1.f);

	output.surfelArea = 4.f * viewZ * viewZ / (g_lpv.rsmResolution * g_lpv.rsmResolution * g_lpv.cellSize * g_lpv.cellSize);

	return output;

}