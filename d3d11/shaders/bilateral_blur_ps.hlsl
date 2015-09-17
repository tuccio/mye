#include "register_slots.hlsli"
#include "common_samplers.hlsli"
#include "constants.hlsli"
#include "gauss.hlsli"

/*
*
* Shader for bilateral blur approximation with a separable convolution
*
* MYE_BILATERAL_BLUR_KERNEL_SIZE:  Filter kernel size
* MYE_BILATERAL_BLUR_GAUSS_SIGMA:  Sigma for the gaussian weight
* MYE_BILATERAL_BLUR_RESOLUTION:   The input texture resolution
* MYE_BILATERAL_BLUR_TEXTURE_SLOT: The texture slot where the texture to blur is bound
*
* MYE_SEPARABLE_CONVOLUTION_VERTICAL/MYE_SEPARABLE_CONVOLUTION_HORIZONTAL: Compiles the corresponding pass
*
*/

struct PSInput
{
	float4 positionCS : SV_position;
	float2 texcoord   : TEXCOORD;
};

#ifndef MYE_BILATERAL_BLUR_GAUSS_SIGMA
#define MYE_BILATERAL_BLUR_GAUSS_SIGMA 1
#endif

#ifndef MYE_BILATERAL_BLUR_KERNEL_SIZE
#define MYE_BILATERAL_BLUR_KERNEL_SIZE 7
#endif

#define __MYE_BILATERAL_BLUR_KERNEL_END   (MYE_BILATERAL_BLUR_KERNEL_SIZE >> 1)
#define __MYE_BILATERAL_BLUR_KERNEL_START (- __MYE_BILATERAL_BLUR_KERNEL_END)

#define __MYE_BILATERAL_BLUR_TEXTURE_TYPE Texture2D

__MYE_BILATERAL_BLUR_TEXTURE_TYPE < MYE_BILATERAL_BLUR_TYPE > g_texture : register(MYE_BILATERAL_BLUR_TEXTURE_SLOT);

SamplerState g_blurSampler : register(__MYE_DX11_SAMPLER_SLOT_BLUR);

MYE_BILATERAL_BLUR_TYPE main(PSInput input) : SV_Target0
{

	float texsize = 1.f / MYE_BILATERAL_BLUR_RESOLUTION;

#ifdef MYE_SEPARABLE_CONVOLUTION_VERTICAL
	const float2 offset = { 0.f, texsize };
#elif defined(MYE_SEPARABLE_CONVOLUTION_HORIZONTAL)
	const float2 offset = { texsize, 0.f };
#endif

	float  normalization = 1.f;
	MYE_BILATERAL_BLUR_TYPE centerColor = g_texture.SampleLevel(g_blurSampler, input.texcoord, 0);

	MYE_BILATERAL_BLUR_TYPE output = centerColor;

	// Normalize colors depending on the vector length
	float distanceNormalizationFactor = length((MYE_BILATERAL_BLUR_TYPE) 1);

	[unroll]
	for (int i = __MYE_BILATERAL_BLUR_KERNEL_START; i <= __MYE_BILATERAL_BLUR_KERNEL_END; i++)
	{

		MYE_BILATERAL_BLUR_TYPE sampleColor = g_texture.SampleLevel(g_blurSampler, input.texcoord + i * offset, 0);

		float closeness = length(sampleColor - centerColor) / distanceNormalizationFactor;
		float gaussian  = GaussianWeight(MYE_BILATERAL_BLUR_GAUSS_SIGMA, i);

		float sampleWeight = closeness * gaussian;

		output += sampleWeight * sampleColor;

		normalization += sampleWeight;

	}

	output /= normalization;

	return output;

}