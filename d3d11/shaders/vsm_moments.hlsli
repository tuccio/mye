#ifndef __MYE_VSM_MOMENTS__
#define __MYE_VSM_MOMENTS__

/* Source: http://http.developer.nvidia.com/GPUGems3/gpugems3_ch08.html */

float linstep(in float min, in float max, in float alpha)
{
	return saturate((alpha - min) / (max - min));
}

float2 VSMComputeMoments(in float depth)
{

	float2 moments;

	moments.x = depth;

	float dx = ddx(depth);
	float dy = ddy(depth);

	moments.y = depth * depth + .25f * (dx * dx + dy * dy);

	return moments;

}

float VSMChebyshevUpperBound(in float2 moments, in float t, in float minVariance, in float minBleeding)
{

	float p = (t <= moments.x);

	float variance = moments.y - moments.x * moments.x;
	variance = max(variance, minVariance);

	float d = t - moments.x;

	float pmax = variance / (variance + d * d);

	return max(p, linstep(minBleeding, 1.f, pmax));

}

#endif