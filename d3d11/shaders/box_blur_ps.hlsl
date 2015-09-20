#include "register_slots.hlsli"
#include "common_samplers.hlsli"

/*
 *
 * Shader for box blur using separable convolution, needs these macros to be defined:
 *
 * MYE_BOX_BLUR_RESOLUTION:   The input texture resolution
 * MYE_BOX_BLUR_TYPE:         Data type for the texture to be blurred
 * MYE_BOX_BLUR_KERNEL_SIZE:  Kernel size (should be an odd number, usually 3/5/7)
 * MYE_BOX_BLUR_TEXTURE_SLOT: The texture slot where the texture to blur is bound
 * MYE_SEPARABLE_CONVOLUTION_VERTICAL/MYE_SEPARABLE_CONVOLUTION_HORIZONTAL: Compiles the corresponding pass
 *
 */

#define MYE_BOX_BLUR_KERNEL_END   (MYE_BOX_BLUR_KERNEL_SIZE >> 1)
#define MYE_BOX_BLUR_KERNEL_START (- MYE_BOX_BLUR_KERNEL_END)

struct PSInput
{
	float4 positionCS : SV_position;
	float2 texcoord   : TEXCOORD;
#ifdef MYE_BOX_BLUR_ARRAY
	uint   target     : SV_RenderTargetArrayIndex;
#endif
};

#ifdef MYE_BOX_BLUR_ARRAY
#define __MYE_BOX_BLUR_TEXTURE_TYPE Texture2DArray
#else
#define __MYE_BOX_BLUR_TEXTURE_TYPE Texture2D
#endif

__MYE_BOX_BLUR_TEXTURE_TYPE < MYE_BOX_BLUR_TYPE > g_texture : register(MYE_BOX_BLUR_TEXTURE_SLOT);

SamplerState g_blurSampler : register(__MYE_DX11_SAMPLER_SLOT_BLUR);

MYE_BOX_BLUR_TYPE main(PSInput input) : SV_Target0
{

	MYE_BOX_BLUR_TYPE output = (MYE_BOX_BLUR_TYPE) 0;
	
	int w, h;
	g_texture.GetDimensions(w, h);

#ifdef MYE_SEPARABLE_CONVOLUTION_VERTICAL
	const float2 offset = { 0.f, 1.f / h };
#elif defined(MYE_SEPARABLE_CONVOLUTION_HORIZONTAL)
	const float2 offset = { 1.f / w, 0.f };
#endif

	[unroll]
	for (int i = MYE_BOX_BLUR_KERNEL_START; i <= MYE_BOX_BLUR_KERNEL_END; i++)
	{

#ifdef MYE_BOX_BLUR_ARRAY
		output += g_texture.SampleLevel(g_blurSampler, float3(input.texcoord + i * offset, input.target), 0);
#else
		output += g_texture.SampleLevel(g_blurSampler, input.texcoord + i * offset, 0);
#endif

	}

	output *= (1.f / MYE_BOX_BLUR_KERNEL_SIZE);

	return output;

}