#ifndef __MYE_SHADOW_MAPPING__
#define __MYE_SHADOW_MAPPING__

#pragma pack_matrix(row_major)

//#define MYE_SHADOW_MAPPING_PCF

#define __MYE_SHADOW_MAPPING_PCF_LOOP_BEGIN -2
#define __MYE_SHADOW_MAPPING_PCF_LOOP_END   5
#define __MYE_SHADOW_MAPPING_PCF_BLUR_RATIO .04f

#ifndef __MYE_SHADOW_MAP_BIAS
#define __MYE_SHADOW_MAP_BIAS (0.001f)
#endif

#include "light.hlsli"
#include "register_slots.hlsli"
#include "renderer_configuration.hlsli"

#ifdef __MYE_LIGHT_DIRECTIONAL__

#include "pssm.hlsli"
#include "camera_transform.hlsli"

#define MYE_SHADOWMAP_TYPE Texture2DArray<float>

cbuffer cbPSSM : register(__MYE_DX11_BUFFER_SLOT_PSSMBUFFER)
{

	struct PSSMSlice
	{
		float4x4 cropMatrix;
		float    near;
		float    far;
	} g_slice[__MYE_PSSM_SPLITS];

}

#else
#define MYE_SHADOWMAP_TYPE Texture2D<float>
#endif

cbuffer cbLightSpace : register(__MYE_DX11_BUFFER_SLOT_LIGHTSPACETRANSFORM)
{
	float4x4 g_lightSpaceTransform;
}


cbuffer cbLight : register(__MYE_DX11_BUFFER_SLOT_LIGHT)
{
	Light g_light;
}

/* Shadow map and samplers */

MYE_SHADOWMAP_TYPE     g_shadowMap           : register(__MYE_DX11_TEXTURE_SLOT_SHADOWMAP);
SamplerState           g_shadowMapSampler    : register(__MYE_DX11_SAMPLER_SLOT_SHADOWMAP);
SamplerComparisonState g_shadowMapSamplerCmp : register(__MYE_DX11_SAMPLER_SLOT_SHADOWMAP_CMP);

/* Shadow mapping */

float ShadowMapBiasedDepth(in float4 lightSpacePosition)
{
	return lightSpacePosition.z - __MYE_SHADOW_MAP_BIAS;
}

#ifdef MYE_SHADOW_MAPPING_PCF

float2 ShadowMapPCFDepthDelta(in float4 lightSpacePosition, in float texelScale)
{

	float3 lsp = lightSpacePosition.xyz;

	float3 lspDDX = ddx(lsp);
	float3 lspDDY = ddy(lsp);

	float2x2 screenSpaceToLightSpace = {
		lspDDX.x, lspDDY.x,
		lspDDX.y, lspDDY.y
	};

	float invDeterminant = 1.f / determinant(screenSpaceToLightSpace);

	float2x2 lightSpaceToScreenSpace = {
		  screenSpaceToLightSpace._22 * invDeterminant, - screenSpaceToLightSpace._12 * invDeterminant,
		- screenSpaceToLightSpace._21 * invDeterminant,   screenSpaceToLightSpace._11 * invDeterminant
	};

	float2 rightTexelLightSpace  = float2(texelScale, 0.f);
	float2 upTexelLightSpace     = float2(0.f, texelScale);

	float2 rightTexelScreenSpace = mul(lightSpaceToScreenSpace, rightTexelLightSpace);
	float2 upTexelScreenSpace    = mul(lightSpaceToScreenSpace, upTexelLightSpace);
		
	float2 lspDepthGradient      = float2(lspDDX.z, lspDDY.z);

	float  rightTexelDepthDelta  = dot(rightTexelScreenSpace, lspDepthGradient);
	float  upTexelDepthDelta     = dot(upTexelScreenSpace,    lspDepthGradient);

	return float2(rightTexelDepthDelta, upTexelDepthDelta);

}

float2 ShadowMapPCFDepthDelta2(in float4 lightSpacePosition, in float texelScale)
{

	float3 lsp = lightSpacePosition.xyz;

	float3 lspDDX = ddx(lsp);
	float3 lspDDY = ddy(lsp);

	float2x2 screenSpaceToLightSpace;

	screenSpaceToLightSpace._11 = lspDDX.x;
	screenSpaceToLightSpace._12 = lspDDY.x;
	screenSpaceToLightSpace._21 = lspDDX.y;
	screenSpaceToLightSpace._22 = lspDDY.y;

	float invDeterminant = 1.f / determinant(screenSpaceToLightSpace);

	float2x2 lightSpaceToScreenSpace = {
		  screenSpaceToLightSpace._22 * invDeterminant, - screenSpaceToLightSpace._12 * invDeterminant,
		- screenSpaceToLightSpace._21 * invDeterminant,   screenSpaceToLightSpace._11 * invDeterminant
	};

	float2 lspDepthGradient = float2(lspDDX.z, lspDDY.z);

	//dot(lspDepthGradient, mul(lightSpaceToScreenSpace, x));

	return float2(lspDepthGradient.x * lightSpaceToScreenSpace._11 + lspDepthGradient.y * lightSpaceToScreenSpace._21,
	              lspDepthGradient.x * lightSpaceToScreenSpace._12 + lspDepthGradient.y * lightSpaceToScreenSpace._22);


}

float ShadowMapCompareDepth(in float4 lightSpacePosition, in float3 projectTexCoord, in float lightDepthValue, in float texelScale, in float2 texelDepthDelta)
{

	float shadowFactor = 0.f;

	for (int i = __MYE_SHADOW_MAPPING_PCF_LOOP_BEGIN; i < __MYE_SHADOW_MAPPING_PCF_LOOP_END; i++)
	{
		for (int j = __MYE_SHADOW_MAPPING_PCF_LOOP_BEGIN; j < __MYE_SHADOW_MAPPING_PCF_LOOP_END; j++)
		{

			float2 pcfCoords    = float2(i, j) * texelScale;
			float  depthCompare = lightDepthValue + dot(pcfCoords, texelDepthDelta);

			shadowFactor += g_shadowMap.SampleCmpLevelZero(g_shadowMapSamplerCmp, projectTexCoord + float3(pcfCoords, 0), depthCompare);

		}
	}

	shadowFactor *= __MYE_SHADOW_MAPPING_PCF_BLUR_RATIO;

	/*shadowFactor =
		0.25f * (
		g_shadowMap.SampleCmpLevelZero(g_shadowMapSamplerCmp, projectTexCoord + float3(float2(-0.5f,  0.5f) * texelScale, 0), lightDepthValue) +
		g_shadowMap.SampleCmpLevelZero(g_shadowMapSamplerCmp, projectTexCoord + float3(float2( 0.5f,  0.5f) * texelScale, 0), lightDepthValue) +
		g_shadowMap.SampleCmpLevelZero(g_shadowMapSamplerCmp, projectTexCoord + float3(float2(-1.5f, -1.5f) * texelScale, 0), lightDepthValue) +
		g_shadowMap.SampleCmpLevelZero(g_shadowMapSamplerCmp, projectTexCoord + float3(float2( 0.5f, -1.5f) * texelScale, 0), lightDepthValue));*/

	return shadowFactor;

}



#else

float ShadowMapCompareDepth(in float3 projectTexCoord, in float lightDepthValue)
{

	float depthValue = g_shadowMap.Sample(g_shadowMapSampler, projectTexCoord);

	if (depthValue < lightDepthValue)
	{
		return 0.f;
	}
	else
	{
		return 1.f;
	}

}

#endif

#if defined(__MYE_LIGHT_DIRECTIONAL__)

float ShadowMapPSSM(in float3 x)
{

	float4 lightSpacePosition = mul(g_lightSpaceTransform, float4(x, 1));

#ifdef MYE_SHADOW_MAPPING_PCF

	float  texelScale      = 1.f / r.shadowMapResolution;
	//float2 texelDepthDelta = ShadowMapPCFDepthDelta(lightSpacePosition, texelScale);
	float2 texelDepthDelta = ShadowMapPCFDepthDelta2(lightSpacePosition, texelScale);

#endif

	// Get the camera space z to choose what split we should pick
	// as our shadow map

	float4 cameraSpace = mul(g_cameraViewProj, float4(x, 1));
	float  cameraDepth = cameraSpace.z;

	int split;

	for (int i = 0; i < __MYE_PSSM_SPLITS; i++)
	{
		if (cameraDepth >= g_slice[i].near && cameraDepth < g_slice[i].far)
		{
			split = i;
			break;
		}
	}

	//float4x4 croppedTransform = mul(g_slice[split].cropMatrix, g_lightSpaceTransform);

	// Apply the crop matrix to get the same matrix used to generate the shadow map
	float4 croppedLightSpacePosition = mul(g_slice[split].cropMatrix, lightSpacePosition);

	float3 projectTexCoord;

	projectTexCoord.x = croppedLightSpacePosition.x * .5f + .5f;
	projectTexCoord.y = 1.f - (croppedLightSpacePosition.y * .5f + .5f);
	projectTexCoord.z = split;

	float lightDepthValue = ShadowMapBiasedDepth(croppedLightSpacePosition);
	
	// Finally compare the depth with the shadow map value like in regular shadow mapping

#ifdef MYE_SHADOW_MAPPING_PCF

	return ShadowMapCompareDepth(croppedLightSpacePosition, projectTexCoord, lightDepthValue, texelScale, texelDepthDelta);

#else

	return ShadowMapCompareDepth(projectTexCoord, lightDepthValue);

#endif

}

#else

float ShadowMap(in float3 x)
{

	float4 lightSpacePosition = mul(g_lightSpaceTransform, float4(x, 1));

	float3 projectTexCoord;

	projectTexCoord.x = (lightSpacePosition.x / lightSpacePosition.w) * .5f + .5f;
	projectTexCoord.y = 1.f - ((lightSpacePosition.y / lightSpacePosition.w) * .5f + .5f);
	projectTexCoord.z = 0.f;

	float  texelScale;
	float2 texelDepthDelta;

	float lightDepthValue = ShadowMapBiasedDepth(lightSpacePosition, texelScale, texelDepthDelta);

	return ShadowMapCompareDepth(projectTexCoord, lightDepthValue, texelScale, texelDepthDelta);

}

#endif

float ShadowMapVisibility(in float3 x)
{

#if defined(__MYE_LIGHT_DIRECTIONAL__)

	return ShadowMapPSSM(x);

#else

	return ShadowMap(x);

#endif

}

#endif