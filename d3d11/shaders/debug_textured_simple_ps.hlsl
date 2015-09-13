#include "register_slots.hlsli"
#include "common_samplers.hlsli"

Texture2D g_diffuse : register(__MYE_DX11_TEXTURE_SLOT_DIFFUSE);

struct PSInput
{
	float4 positionCS : SV_Position;
	float2 texcoords  : TEXCOORD0;
};

float4 main(PSInput input) : SV_TARGET
{
	return g_diffuse.Sample(g_trilinearSampler, input.texcoords).rrrr;
}