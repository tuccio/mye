#include "register_slots.hlsli"
#include "common_samplers.hlsli"

Texture2DArray g_diffuse : register(__MYE_DX11_TEXTURE_SLOT_DIFFUSE);

cbuffer cbDebugTextureArray : register(b0)
{
	uint g_slice;
};

struct PSInput
{
	float4 positionCS : SV_Position;
	float2 texcoords  : TEXCOORD0;
};

float4 main(PSInput input) : SV_TARGET
{
	return g_diffuse.Sample(g_trilinearSampler, float3(input.texcoords, g_slice));
}