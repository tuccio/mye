#ifndef __MYE_COMMON_SAMPLERS__
#define __MYE_COMMON_SAMPLERS__

#include "register_slots.hlsli"

SamplerState g_linearSampler      : register(__MYE_DX11_SAMPLER_SLOT_LINEAR);
SamplerState g_anisotropicSampler : register(__MYE_DX11_SAMPLER_SLOT_ANISOTROPIC);

#endif