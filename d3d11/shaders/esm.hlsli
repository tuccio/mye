#ifndef __MYE_ESM__
#define __MYE_ESM__

#define MYE_ESM_POS_EXP 30
#define MYE_ESM_NEG_EXP 5

float2 ESMWarp(in float depth)
{

	float d = 2.f * depth - 1.f;
	
	float p = exp(MYE_ESM_POS_EXP * depth);
	float n = exp(- MYE_ESM_NEG_EXP * depth);
	
	return float2(p, n);

}

float2 ESMExponents()
{
	return float2(MYE_ESM_POS_EXP, MYE_ESM_NEG_EXP);
}

#endif
