#ifndef __MYE_COMMON_SAMPLERS__
#define __MYE_COMMON_SAMPLERS__

#include "register_slots.hlsli"

SamplerState g_pointSampler       : register(__MYE_DX11_SAMPLER_SLOT_POINT);
SamplerState g_bilinearSampler    : register(__MYE_DX11_SAMPLER_SLOT_BILINEAR);
SamplerState g_trilinearSampler   : register(__MYE_DX11_SAMPLER_SLOT_TRILINEAR);
SamplerState g_anisotropicSampler : register(__MYE_DX11_SAMPLER_SLOT_ANISOTROPIC);

#endif