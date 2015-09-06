#ifndef __MYE_PACKING__
#define __MYE_PACKING__

float PackHalf2ToFloat(half2 v)
{
	return asfloat(asuint(v.x) << 16 | asuint(v.y));
}

half2 UnpackFloatToHalf2(float v)
{
	uint p = asuint(v);

	half2 t;

	t.x = f16tof32(p >> 16);
	t.y = f16tof32(0xFFFFFFFF & p);

	return t;

}

#endif