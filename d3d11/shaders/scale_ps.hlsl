#include "quad_input.hlsli"
#include "common_samplers.hlsli"

Texture2D < MYE_SCALE_TYPE > g_texture : register(MYE_SCALE_TEXTURE_SLOT);

float4 main(QuadInput input) : SV_Target0
{
	return g_texture.Sample(g_bilinearSampler, input.texcoord);
}