#include "register_slots.hlsli"
#include "common_samplers.hlsli"

/*
*
* Shader for bilateral blur approximation with a separable convolution
*
* MYE_BILATERAL_BLUR_RESOLUTION:   The input texture resolution
* MYE_BILATERAL_BLUR_TEXTURE_SLOT: The texture slot where the texture to blur is bound
* MYE_SEPARABLE_CONVOLUTION_VERTICAL/MYE_SEPARABLE_CONVOLUTION_HORIZONTAL: Compiles the corresponding pass
*
*/

struct PSInput
{
	float4 positionCS : SV_position;
	float2 texcoord   : TEXCOORD;
};

#define __MYE_BILATERAL_BLUR_KERNEL_SIZE  13
#define __MYE_BILATERAL_BLUR_KERNEL_START -6
#define __MYE_BILATERAL_BLUR_KERNEL_END   6

#define __MYE_BILATERAL_BLUR_TEXTURE_TYPE Texture2D

__MYE_BILATERAL_BLUR_TEXTURE_TYPE < MYE_BILATERAL_BLUR_TYPE > g_texture : register(MYE_BILATERAL_BLUR_TEXTURE_SLOT);

SamplerState g_blurSampler : register(__MYE_DX11_SAMPLER_SLOT_BLUR);

MYE_BILATERAL_BLUR_TYPE main(PSInput input) : SV_Target0
{

	// Just Gaussian filter at the moment (TODO: range difference weighting)

	static const float G[__MYE_BILATERAL_BLUR_KERNEL_SIZE] = {
		0.00311688, 0.01168831, 0.03272727, 0.07090909, 0.12155844,
		0.16714286, 0.18571429, 0.16714286, 0.12155844, 0.07090909,
		0.03272727, 0.01168831, 0.00311688 };

	MYE_BILATERAL_BLUR_TYPE output = (MYE_BILATERAL_BLUR_TYPE) 0;

	float texsize = 1.f / MYE_BILATERAL_BLUR_RESOLUTION;

#ifdef MYE_SEPARABLE_CONVOLUTION_VERTICAL
	const float2 offset = { 0.f, texsize };
#elif defined(MYE_SEPARABLE_CONVOLUTION_HORIZONTAL)
	const float2 offset = { texsize, 0.f };
#endif

	[unroll]
	for (int i = __MYE_BILATERAL_BLUR_KERNEL_START; i <= __MYE_BILATERAL_BLUR_KERNEL_END; i++)
	{

		output += G[i - __MYE_BILATERAL_BLUR_KERNEL_START] * g_texture.SampleLevel(g_blurSampler, input.texcoord + i * offset, 0);

	}

	return output;

}