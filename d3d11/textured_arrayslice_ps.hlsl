#include "register_slots.hlsli"

Texture2DArray g_diffuse : register(__MYE_DX11_TEXTURE_SLOT_DIFFUSE);
SamplerState   g_sampler : register(__MYE_DX11_SAMPLER_SLOT_DIFFUSE);

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

	
	return g_diffuse.Sample(g_sampler, float3(input.texcoords, g_slice));

	//return g_diffuse.Load(int4(1024 * input.texcoords, 0, 2));
}