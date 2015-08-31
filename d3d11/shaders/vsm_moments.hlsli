#ifndef __MYE_VSM_MOMENTS__
#define __MYE_VSM_MOMENTS__


float2 ComputeMoments(in float depth)
{

	float2 moments;

	moments.x = depth;

	float dx = ddx(depth);
	float dy = ddx(depth);

	moments.y = depth * depth + 0.25f * (dx * dx + dy * dy);

	return moments;

}

#endif