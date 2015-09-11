#ifndef __MYE_LINEAR_DEPTH__
#define __MYE_LINEAR_DEPTH__

#include "camera_transform.hlsli"

float4 LinearizeDepth(in float4 position)
{
	//position.z = position.z * position.w / g_camera.far;
	return position;
}

#endif