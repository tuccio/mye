#ifndef __MYE_RENDER_INFO__
#define __MYE_RENDER_INFO__

#include "register_slots.hlsli"


cbuffer cbRendererConfiguration : register(__MYE_DX11_BUFFER_SLOT_RENDERINFO)
{

    struct RendererConfiguration
    {
        int2   resolution;

        float  gamma;

        int    shadowMapResolution;

		float  vsmMinVariance;
		float  vsmMinBleeding;

    } r;

};

#endif