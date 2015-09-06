#ifndef __MYE_LIGHT_PARALLAX_MAPPING__
#define __MYE_LIGHT_PARALLAX_MAPPING__

#ifdef MYE_USE_PARALLAX

#include "common_samplers.hlsli"


#define MYE_PARALLAX_SCALE       .05f
#define MYE_PARALLAX_MIN_SAMPLES 4
#define MYE_PARALLAX_MAX_SAMPLES 64

void ParallaxMapping(in Texture2D normalHeightMap, in float3 eyeTS, in float3 normalTS, in float3x3 tangentToWorldSpace, inout float2 texcoord, inout float3 positionWS, out float3 normalWS)
{

	/* Sources:
	 * Real-Time Rendering 6.7.3, 6.7.4
	 * http://www.gamedev.net/page/resources/_/technical/graphics-programming-and-theory/a-closer-look-at-parallax-occlusion-mapping-r3262
	 * http://developer.amd.com/wordpress/media/2012/10/Tatarchuk-POM.pdf
	 */

	float2 dx = ddx(texcoord);
	float2 dy = ddy(texcoord);

	float3 N = normalize(normalTS);
	float3 E = normalize(eyeTS);

	// If not scaled, the ray can reach height 1
	float maxOffsetLength = - MYE_PARALLAX_SCALE * length(E.xy) / E.z;				

	// Set up the ray tracing: choose the number of samples depending on the angle, uniform sample
	// along the surface plane

	float2 offsetDirection = normalize(E.xy);
	float2 maxOffset       = offsetDirection * maxOffsetLength;

	int   samples  = (int) lerp(MYE_PARALLAX_MAX_SAMPLES, MYE_PARALLAX_MIN_SAMPLES, dot(E, N));
	float stepSize = 1.f / samples;

	// Initialize the starting view ray height and the texture offsets.

	float  currentRayHeight = 1.f;
	float2 currentOffset    = { 0, 0 };
	float2 lastOffset       = { 0, 0 };
	
	float lastSampledHeight     = 1;
	float currentSampledHeight = 1;

	int currentSample = 0;

	while (currentSample < samples)
	{

		currentSampledHeight = normalHeightMap.SampleGrad(g_linearSampler, texcoord + currentOffset, dx, dy).a;

		if (currentSampledHeight > currentRayHeight)
		{
			
			// Intersection between the linear approximation of the heightmap samples
			// and the ray

			float delta1 = currentSampledHeight - currentRayHeight;
			float delta2 = (currentRayHeight + stepSize) - lastSampledHeight;
			float ratio  = delta1 / (delta1 + delta2);

			currentOffset = ratio * lastOffset + 1.f - ratio * currentOffset;
			
			currentSample = samples + 1;	

		}
		else
		{

			currentSample++;

			currentRayHeight -= stepSize;

			lastOffset        = currentOffset;
			currentOffset    += stepSize * maxOffset;

			lastSampledHeight = currentSampledHeight;

		}
	}

	texcoord += currentOffset;
	
	float3 sampledNormalTS = 2.f * normalHeightMap.Sample(g_linearSampler, texcoord).rgb - 1.f;
	
	normalWS = mul(tangentToWorldSpace, sampledNormalTS);

}

#endif

#endif