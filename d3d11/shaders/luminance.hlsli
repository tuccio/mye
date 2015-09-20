#ifndef __MYE_LUMINANCE__
#define __MYE_LUMINANCE__

float Luminance(in float3 color)
{
	return dot(float3(.2125f, .7154f, .0721f), color);
}

#endif