#ifndef __MYE_RENDER_INFO__
#define __MYE_RENDER_INFO__

#include "register_slots.hlsli"


cbuffer cbRendererConfiguration : register(__MYE_DX11_BUFFER_SLOT_RENDERINFO)
{

    struct RendererConfiguration
    {

        int2  resolution;
			  
        float gamma;
			  
        int   shadowMapResolution;
		float shadowMapInvResolution;
		float shadowMapBias;
		float shadowMapSlopeScaledBias;
		float shadowMapNormalOffsetBias;
			  
		float vsmMinVariance;
		float vsmMinBleeding;
			  
		uint  csmSplits;			  
		bool  csmDebug;

		bool  pcfEnabled;
		uint  pcfKernel;
		float pcfKernelInvSquare;

		float lpvAttenuation;

    } r;

};

#endif