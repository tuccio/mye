#ifndef __MYE_LIGHT_PARALLAX_MAPPING__
#define __MYE_LIGHT_PARALLAX_MAPPING__

#include "common_samplers.hlsli"

#ifdef MYE_PIXEL_SHADER

#define MYE_PARALLAX_SCALE       .3f
#define MYE_PARALLAX_MIN_SAMPLES 32
#define MYE_PARALLAX_MAX_SAMPLES 64

Texture2D g_normalHeightMap : register(__MYE_DX11_TEXTURE_SLOT_HEIGHTMAP);

void ParallaxMapping(in float3 eyeTS, in float3 normalTS, in float3x3 tangentToWorldSpace, inout float2 texcoord, inout float3 positionWS, out float3 normalWS)
{

	//normalWS = mul(tangentToWorldSpace, normalTS);

	//float3 Nts = 2.f * g_normalHeightMap.SampleGrad(g_linearSampler, texcoord, ddx(texcoord), ddy(texcoord)).xyz - 1.f;

	//float3 Nts = 2.f * g_normalHeightMap.Sample(g_linearSampler, texcoord).rgb - 1.f;
	//normalWS = mul(tangentToWorldSpace, Nts);
	//normalWS = Nts;

	//return;

	/* Sources:
	   http://www.gamedev.net/page/resources/_/technical/graphics-programming-and-theory/a-closer-look-at-parallax-occlusion-mapping-r3262
	   http://developer.amd.com/wordpress/media/2012/10/Tatarchuk-POM.pdf
	 */

	float2 dx = ddx(texcoord);
	float2 dy = ddy(texcoord);

	float3 N = normalize(normalTS);
	float3 E = normalize(eyeTS);	

	float  limit                = - MYE_PARALLAX_SCALE * length(eyeTS.xy) / eyeTS.z;

	float2 offsetDirection      = normalize(eyeTS.xy);
	float2 maxOffset            = offsetDirection * limit;

	int    samples              = (int) lerp(MYE_PARALLAX_MIN_SAMPLES, MYE_PARALLAX_MAX_SAMPLES, saturate(dot(E, N)));
	float  stepSize             = 1.f / samples;

	float  currentRayHeight     = 1.f;

	float2 lastOffset           = float2(0, 0);
	float2 currentOffset        = float2(0, 0);

	int    currentSample        = 0;

	float  lastSampledHeight    = 1.f;

	float  currentSampledHeight;

	while (currentSample < samples)
	{

		currentSampledHeight = g_normalHeightMap.SampleGrad(g_linearSampler, texcoord + currentOffset, dx, dy).a;

		if (currentSampledHeight > currentRayHeight)
		{

			float delta1  = currentSampledHeight - currentRayHeight;
			float delta2  = (currentRayHeight + stepSize) - lastSampledHeight;

			float ratio   = delta1 / (delta1 + delta2);

			currentOffset = lerp(lastOffset, currentOffset, ratio);

			currentSample = samples + 1;

		}
		else
		{

			currentSample++;

			currentRayHeight  -= stepSize;
							  
			lastOffset         = currentOffset;
			currentOffset     += stepSize * maxOffset;
							  
			lastSampledHeight  = currentSampledHeight;

		}

	}

	texcoord += currentOffset;

	//positionWS += currentSampledHeight * E;
	//normalWS    = 

	float3 finalNormal = g_normalHeightMap.Sample(g_linearSampler, texcoord).rgb;

	normalWS = mul(tangentToWorldSpace, finalNormal * 2.f - 1.f);

}

#endif

#endif