#pragma pack_matrix(row_major)

#include "constants.hlsli"
#include "gamma.hlsli"
#include "material.hlsli"
#include "light.hlsli"
#include "light_directional.hlsli"
#include "register_slots.hlsli"
#include "renderer_configuration.hlsli"
#include "common_samplers.hlsli"

#ifdef MYE_SHADOW_MAP_VSM
#include "vsm_moments.hlsli"
#endif

#define MYE_CONE90_SOLID_ANGLE        2.09439510f

/* Constant buffers */

cbuffer cbLight : register(__MYE_DX11_BUFFER_SLOT_LIGHT)
{
	Light g_light;
};

cbuffer cbMaterial : register(__MYE_DX11_BUFFER_SLOT_MATERIAL)
{
	Material g_material;
};

#ifdef MYE_USE_DIFFUSE_TEXTURE

Texture2D    g_diffuseTexture : register(__MYE_DX11_TEXTURE_SLOT_DIFFUSE);

#endif

/* Input/Output structures */

struct PSInput
{

	float4 positionCS : SV_Position;
	float2 texcoord   : TEXCOORD;
	float3 normalWS   : NORMALWS;
	float3 positionWS : POSITIONWS;

};

struct PSOutput
{

	float4 position : SV_Target0;
	float4 normal   : SV_Target1;
	float4 flux     : SV_Target2;
	float4 depth    : SV_Target3;

};

// Evaluates the pixel surface area using a linear approximation
// obtained calculating the Jacobian matrix for the position
// in screen space

float EstimatePixelSize(in float3 position)
{

	float3 pdx = ddx(position);
	float3 pdy = ddy(position);

	float3x2 J = {
		pdx.x, pdy.x,
		pdx.y, pdy.y,
		pdx.z, pdy.z
	};

	float3 deltaMin = mul(J, float2(-.5f, -.5f));
	float3 deltaMax = mul(J, float2(.5f, .5f));

	// Get the rectangle area

	float3 delta = deltaMax - deltaMin;

	return length(cross(float3(delta.xy, 0), float3(0, delta.yz)));

}

/* Main */

PSOutput main(PSInput input)
{

	float3 N = normalize(input.normalWS);
	float3 L = LightVector(g_light, input.positionWS);

	float3 albedo;

#ifdef MYE_USE_DIFFUSE_TEXTURE

	albedo = Gamma(g_diffuseTexture.Sample(g_anisotropicSampler, input.texcoord));

#else

	albedo = Gamma(g_material.diffuseColor);

#endif

	// TODO: fixme
	float  area  = EstimatePixelSize(input.positionWS);

	float4 NdotL = saturate(dot(N, L));

	float3 irradiance = LightIrradiance(g_light, input.positionWS);

	PSOutput output;

	output.position = float4(input.positionWS, area);
	output.normal   = float4(N, 1);
	output.flux     = float4(saturate(irradiance * NdotL) * albedo * MYE_CONE90_SOLID_ANGLE * MYE_INV_PI, 1);
	output.depth    = float4(LightSpaceLinearDepth(g_light, input.positionCS, input.positionWS), 0, 0, 1);

	return output;

}