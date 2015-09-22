#ifndef __MYE_RENDER_INFO__
#define __MYE_RENDER_INFO__

#include "register_slots.hlsli"


cbuffer cbRendererConfiguration : register(__MYE_DX11_BUFFER_SLOT_RENDERINFO)
{

    struct RendererConfiguration
    {

        int2  resolution;
			  
        float        gamma;

		int          shadowMapResolution;
		float        shadowMapInvResolution;
		float        shadowMapBias;
		float        shadowMapSlopeScaledBias;
		float        shadowMapNormalOffsetBias;

		float        vsmMinVariance;
		float        vsmMinBleeding;

		float        esmPositiveExponent;
		float        esmNegativeExponent;

		uint         csmSplits;
		int          csmDebug;
		
		int          pcfEnabled;
		uint         pcfKernel;
		float        pcfKernelInvSquare;

		float        lpvAttenuation;

		float        ppBloomThreshold;
		float        ppBloomPower;
		
		float        ppTonemapMiddleGrey;
		float        ppTonemapWhite;
		float        ppTonemapLogAverage;

    } r;

};

#endif