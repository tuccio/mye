#ifndef __MYE_ESM__
#define __MYE_ESM__

#include "renderer_configuration.hlsli"

float2 ESMWarp(in float depth)
{

	float d = 2.f * depth - 1.f;
	
	float p = exp(r.esmPositiveExponent * depth);
	float n = exp(- r.esmNegativeExponent * depth);
	
	return float2(p, n);

}

float2 ESMExponents()
{
	return float2(r.esmPositiveExponent, r.esmNegativeExponent);
}

#endif
