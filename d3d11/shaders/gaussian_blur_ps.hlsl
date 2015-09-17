#include "register_slots.hlsli"
#include "common_samplers.hlsli"
#include "gauss.hlsli"
#include "quad_input.hlsli"

#ifndef MYE_GAUSSIAN_BLUR_SIGMA
#define MYE_GAUSSIAN_BLUR_SIGMA 1
#endif

#ifndef MYE_GAUSSIAN_BLUR_KERNEL_SIZE
#define MYE_GAUSSIAN_BLUR_KERNEL_SIZE 7
#endif

#define __MYE_GAUSSIAN_BLUR_KERNEL_END   (MYE_GAUSSIAN_BLUR_KERNEL_SIZE >> 1)
#define __MYE_GAUSSIAN_BLUR_KERNEL_START (- __MYE_GAUSSIAN_BLUR_KERNEL_END)

#define __MYE_GAUSSIAN_BLUR_TEXTURE_TYPE Texture2D

__MYE_GAUSSIAN_BLUR_TEXTURE_TYPE < MYE_GAUSSIAN_BLUR_TYPE > g_texture : register(MYE_GAUSSIAN_BLUR_TEXTURE_SLOT);

SamplerState g_blurSampler : register(__MYE_DX11_SAMPLER_SLOT_BLUR);

MYE_GAUSSIAN_BLUR_TYPE main(QuadInput input) : SV_Target0
{

	MYE_GAUSSIAN_BLUR_TYPE output = (MYE_GAUSSIAN_BLUR_TYPE) 0;

	float texsize = 1.f / MYE_GAUSSIAN_BLUR_RESOLUTION;

#ifdef MYE_SEPARABLE_CONVOLUTION_VERTICAL
	const float2 offset = { 0.f, texsize };
#elif defined(MYE_SEPARABLE_CONVOLUTION_HORIZONTAL)
	const float2 offset = { texsize, 0.f };
#endif

	[unroll]
	for (int i = __MYE_GAUSSIAN_BLUR_KERNEL_START; i <= __MYE_GAUSSIAN_BLUR_KERNEL_END; i++)
	{
	
		float gaussian = GaussianWeight(MYE_GAUSSIAN_BLUR_SIGMA, i);
		output += gaussian * g_texture.SampleLevel(g_blurSampler, input.texcoord + i * offset, 0);

	}

	return output;

}