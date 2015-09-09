#ifndef __MYE_REGISTER_SLOTS__
#define __MYE_REGISTER_SLOTS__

#define __MYE_DX11_TEXTURE_SLOT_DIFFUSE            t0
#define __MYE_DX11_TEXTURE_SLOT_NORMALMAP          t1
#define __MYE_DX11_TEXTURE_SLOT_HEIGHTMAP          t2
#define __MYE_DX11_TEXTURE_SLOT_SHADOWMAP          t3
#define __MYE_DX11_TEXTURE_SLOT_RSMPOSITION        t4
#define __MYE_DX11_TEXTURE_SLOT_RSMNORMAL          t5
#define __MYE_DX11_TEXTURE_SLOT_RSMFLUX            t6
#define __MYE_DX11_TEXTURE_SLOT_GBUFFER0           t10
#define __MYE_DX11_TEXTURE_SLOT_GBUFFER1           t11
#define __MYE_DX11_TEXTURE_SLOT_GBUFFER2           t12
#define __MYE_DX11_TEXTURE_SLOT_GBUFFER3           t13
#define __MYE_DX11_TEXTURE_SLOT_RANDOM             t14
#define __MYE_DX11_TEXTURE_SLOT_LPVLIGHT_RED       t15
#define __MYE_DX11_TEXTURE_SLOT_LPVLIGHT_GREEN     t16
#define __MYE_DX11_TEXTURE_SLOT_LPVLIGHT_BLUE      t17
#define __MYE_DX11_TEXTURE_SLOT_LPVGEOMETRY        t18
#define __MYE_DX11_TEXTURE_SLOT_LIGHTBUFFER        t19
										           
#define __MYE_DX11_SAMPLER_SLOT_DIFFUSE            s0
#define __MYE_DX11_SAMPLER_SLOT_NORMALMAP          s1
#define __MYE_DX11_SAMPLER_SLOT_HEIGHTMAP          s2
#define __MYE_DX11_SAMPLER_SLOT_SHADOWMAP          s3
#define __MYE_DX11_SAMPLER_SLOT_SHADOWMAP_CMP      s4
#define __MYE_DX11_SAMPLER_SLOT_POINT              s5
#define __MYE_DX11_SAMPLER_SLOT_BILINEAR           s6
#define __MYE_DX11_SAMPLER_SLOT_TRILINEAR          s7
#define __MYE_DX11_SAMPLER_SLOT_ANISOTROPIC        s8
#define __MYE_DX11_SAMPLER_SLOT_LPV                s9
#define __MYE_DX11_SAMPLER_SLOT_RANDOM             s10
#define __MYE_DX11_SAMPLER_SLOT_BLUR               s11

#define __MYE_DX11_BUFFER_SLOT_CAMERATRANSFORM     b7
#define __MYE_DX11_BUFFER_SLOT_LIGHT               b8
#define __MYE_DX11_BUFFER_SLOT_MATERIAL            b9
#define __MYE_DX11_BUFFER_SLOT_RENDERINFO          b10
#define __MYE_DX11_BUFFER_SLOT_LIGHTSPACETRANSFORM b11
#define __MYE_DX11_BUFFER_SLOT_PSSMBUFFER          b12

#endif