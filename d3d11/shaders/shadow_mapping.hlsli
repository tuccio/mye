#ifndef __MYE_SHADOW_MAPPING__
#define __MYE_SHADOW_MAPPING__

#pragma pack_matrix(row_major)

#include "light.hlsli"
#include "register_slots.hlsli"
#include "renderer_configuration.hlsli"
#include "common_samplers.hlsli"
#include "vsm_moments.hlsli"
#include "gbuffer_read.hlsli"
#include "camera_transform.hlsli"
#include "esm.hlsli"

#if !defined(MYE_SHADOW_MAP_VSM) && defined(MYE_SHADOW_MAP_PCF)
#define __MYE_PCF_SHADOW_MAPPING
#endif

#ifdef MYE_SHADOW_MAP_CSM

#include "pssm.hlsli"

	#define __MYE_SHADOW_MAP_TEXTURE_TYPE Texture2DArray

cbuffer cbPSSM : register(__MYE_DX11_BUFFER_SLOT_PSSMBUFFER)
{

	struct PSSMSlice
	{
		float4x4 cropMatrix;
		float    near;
		float    far;
	} g_slice[__MYE_PSSM_MAX_SPLITS];

}

#else

	#define __MYE_SHADOW_MAP_TEXTURE_TYPE Texture2D

#endif

#ifdef MYE_SHADOW_MAP_VSM
	#ifdef MYE_SHADOW_MAP_EVSM
		#define __MYE_SHADOW_MAP_DATA_TYPE float4
	#else
		#define __MYE_SHADOW_MAP_DATA_TYPE float2
	#endif
#else
	#define __MYE_SHADOW_MAP_DATA_TYPE float
#endif

cbuffer cbLightSpace : register(__MYE_DX11_BUFFER_SLOT_LIGHTSPACETRANSFORM)
{
	float4x4 g_lightSpaceTransform;
}

//#define __MYE_POISSON_DISK_TAPS         10
//#define __MYE_POISSON_DISK_INVERSE_TAPS .1f
//
//static float2 g_poissonDisk[__MYE_POISSON_DISK_TAPS] = {
//		{ -0.7215953f, -0.2134517f },
//		{ -0.754268f, 0.3745826f },
//		{ 0.07589724f, -0.2226067f },
//		{ -0.06084631f, 0.3265062f },
//		{ -0.4331634f, -0.7978624f },
//		{ 0.7661136f, 0.2218201f },
//		{ -0.3700204f, 0.7790591f },
//		{ 0.3654317f, 0.8938f },
//		{ 0.1253316f, -0.972573f },
//		{ 0.567069f, -0.4168186f }
//};

//#define __MYE_POISSON_DISK_TAPS         16
//#define __MYE_POISSON_DISK_INVERSE_TAPS .0625f

//static float2 g_poissonDisk[__MYE_POISSON_DISK_TAPS] = {
//	{ -0.1007749f, -0.5127298f },
//	{ -0.6928392f, -0.6348424f },
//	{ 0.3616422f, -0.7251318f },
//	{ 0.5150201f, -0.160371f },
//	{ -0.3995895f, 0.245256f },
//	{ -0.09641803f, -0.9655263f },
//	{ 0.00518492f, -0.0838685f },
//	{ -0.5439374f, -0.1999681f },
//	{ 0.5214711f, 0.4929286f },
//	{ -0.05849996f, 0.6183367f },
//	{ 0.988231f, 0.1394065f },
//	{ 0.3690962f, 0.908119f },
//	{ -0.4700805f, 0.8261935f },
//	{ -0.8795773f, 0.4534842f },
//	{ -0.9701194f, -0.1198528f },
//	{ 0.825885f, -0.5506428f }
//};

#define __MYE_POISSON_DISK_TAPS         25
#define __MYE_POISSON_DISK_INVERSE_TAPS .04f

static float2 g_poissonDisk[__MYE_POISSON_DISK_TAPS] = {
	{ -0.9898047f, 0.08594871f },
	{ -0.6846461f, -0.4100073f },
	{ -0.5209854f, 0.07060355f },
	{ -0.8811331f, 0.4624839f },
	{ -0.3824056f, 0.5220858f },
	{ 0.06794879f, -0.02269273f },
	{ -0.2544252f, -0.2117593f },
	{ -0.06239981f, 0.483956f },
	{ 0.4625237f, 0.2697102f },
	{ 0.2370407f, 0.570186f },
	{ -0.2845682f, 0.836255f },
	{ 0.1036202f, 0.9931732f },
	{ 0.680468f, 0.7108735f },
	{ 0.9394944f, 0.3413454f },
	{ 0.7305045f, -0.1501098f },
	{ 0.3764257f, -0.227539f },
	{ -0.574159f, -0.7482516f },
	{ -0.1991345f, -0.7522087f },
	{ -0.9488707f, -0.2366325f },
	{ 0.2172923f, -0.9012405f },
	{ 0.001194939f, -0.4869f },
	{ 0.529645f, -0.7007937f },
	{ 0.7903866f, -0.474292f },
	{ 0.3973028f, 0.8530729f },
	{ -0.2006629f, 0.1549372f }
};

cbuffer cbLight : register(__MYE_DX11_BUFFER_SLOT_LIGHT)
{
	Light g_light;
}

/* Shadow map and samplers */

#define __MYE_RANDOM_TEXTURE_SIZE 8

__MYE_SHADOW_MAP_TEXTURE_TYPE < __MYE_SHADOW_MAP_DATA_TYPE >  g_shadowMap : register(__MYE_DX11_TEXTURE_SLOT_SHADOWMAP);

SamplerState           g_shadowMapSampler    : register(__MYE_DX11_SAMPLER_SLOT_SHADOWMAP);
SamplerComparisonState g_shadowMapSamplerCmp : register(__MYE_DX11_SAMPLER_SLOT_SHADOWMAP_CMP);

/* Random cos/sin */

Texture2D<float2> g_randomCosSin : register(__MYE_DX11_TEXTURE_SLOT_RANDOM);

/* Shadow mapping */

float ShadowMapBiasedDepth(in float4 lightSpacePosition)
{
	return lightSpacePosition.z - r.shadowMapBias;
}

#ifdef __MYE_PCF_SHADOW_MAPPING

float2 ShadowMapPCFDepthGradient(in float4 lightSpacePosition, in float3 projectTexCoord)
{

	float3 lsp = { projectTexCoord.xy, g_shadowMap.Sample(g_shadowMapSampler, projectTexCoord) };

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

	/*float2 rightTexelLightSpace  = float2(texelScale, 0.f);
	float2 upTexelLightSpace     = float2(0.f, texelScale);

	float2 rightTexelScreenSpace = mul(lightSpaceToScreenSpace, rightTexelLightSpace);
	float2 upTexelScreenSpace    = mul(lightSpaceToScreenSpace, upTexelLightSpace);
		
	float2 lspDepthGradient      = float2(lspDDX.z, lspDDY.z);

	float  rightTexelDepthDelta  = dot(rightTexelScreenSpace, lspDepthGradient);
	float  upTexelDepthDelta     = dot(upTexelScreenSpace,    lspDepthGradient);

	return float2(rightTexelDepthDelta, upTexelDepthDelta);*/

	float2 lspDepthGradient = { lspDDX.z, lspDDY.z };

	return mul(lspDepthGradient, lightSpaceToScreenSpace);

}

/* PCF filtering */

float4 ShadowMapCompareDepthPCF(in GBufferData input, in float4 lightSpacePosition, in float3 projectTexCoord, in float lightDepthValue, in float texelScale, in float2 depthGradient, in float4x4 lightSpaceTransform)
{

	float3 offsetVector           = cross(input.normal, g_light.direction.xyz);
	float3 lightSpaceOffsetVector = normalize(mul((float3x3) lightSpaceTransform, offsetVector));

	float2 rotationCosSin = g_randomCosSin.Sample(g_pointSampler, float2(input.screenPosition) / __MYE_RANDOM_TEXTURE_SIZE);

	float2x2 rotation = {
		rotationCosSin.x, - rotationCosSin.y,
		rotationCosSin.y,   rotationCosSin.x
	};

	float shadowFactor = 0.f;

	[unroll]
	for (int i = 0; i < __MYE_POISSON_DISK_TAPS; i++)
	{

		float2 poissonOffset    = mul(rotation, g_poissonDisk[i]) * r.pcfKernel;
		//float2 lightSpaceOffset = lightSpaceOffsetVector.xy * poissonOffset * texelScale;
		float2 lightSpaceOffset = poissonOffset * texelScale;
		float  slopeScaledBias  = dot(lightSpaceOffset, depthGradient) * r.shadowMapSlopeScaledBias;

		//shadowFactor += g_shadowMap.SampleCmpLevelZero(g_shadowMapSamplerCmp, projectTexCoord + float3(lightSpaceOffset, 0), lightDepthValue + slopeScaledBias);
		shadowFactor += g_shadowMap.SampleCmpLevelZero(g_shadowMapSamplerCmp, projectTexCoord, lightDepthValue);

	}

	shadowFactor *= __MYE_POISSON_DISK_INVERSE_TAPS;

	return float4(shadowFactor, shadowFactor, shadowFactor, 1);

}

#elif !defined(MYE_SHADOW_MAP_VSM)

/* Regular shadow mapping comparison */

float4 ShadowMapCompareDepth(in float3 projectTexCoord, in float lightDepthValue)
{

	float depthValue = g_shadowMap.Sample(g_shadowMapSampler, projectTexCoord);
	//float depth = (depthValue < lightDepthValue);

	float depth = (lightDepthValue < depthValue);

	return float4(depth, depth, depth, 1.f);

}

#else


#ifndef MYE_SHADOW_MAP_EVSM

/* VSM depth comparison */

float4 ShadowMapCompareDepth(in float3 projectTexCoord, in float lightDepthValue)
{

#ifdef MYE_SHADOW_MAP_CSM
	float2 moments = g_shadowMap.SampleLevel(g_anisotropicSampler, projectTexCoord, 0);
#else
	float2 moments = g_shadowMap.SampleLevel(g_anisotropicSampler, projectTexCoord.xy, 0);
#endif

	float p = VSMChebyshevUpperBound(moments, lightDepthValue, r.vsmMinVariance, r.vsmMinBleeding);

	return float4(p, p, p, 1);

}

#else

/* EVSM depth comparison */

float4 ShadowMapCompareDepth(in float3 projectTexCoord, in float lightDepthValue)
{

#ifdef MYE_SHADOW_MAP_CSM
	float4 moments = g_shadowMap.SampleLevel(g_anisotropicSampler, projectTexCoord, 0);
#else
	float4 moments = g_shadowMap.SampleLevel(g_anisotropicSampler, projectTexCoord.xy, 0);
#endif

	float2 wDepth = ESMWarp(lightDepthValue);

	float2 depthScale  = r.vsmMinVariance * ESMExponents() * wDepth;
	float2 minVariance = depthScale * depthScale;
		   
	float  pPos = VSMChebyshevUpperBound(moments.xz, wDepth.x, minVariance.x, r.vsmMinBleeding);
	float  pNeg = VSMChebyshevUpperBound(moments.yw, wDepth.y, minVariance.y, r.vsmMinBleeding);
		   
	float  p = min(pPos, pNeg);

	return float4(p, p, p, 1);

}

#endif

#endif

#ifdef MYE_SHADOW_MAP_CSM

float4 ShadowMapPSSM(in GBufferData input)
{

	float4 x = { input.position, 1 };
	//float4 lightSpacePosition = mul(g_lightSpaceTransform, x);

	// Get the camera space z to choose what split we should pick
	// as our shadow map

	float4 cameraSpace = mul(g_camera.viewProj, x);
	float  cameraDepth = cameraSpace.z;

	unsigned int split = r.csmSplits;

	for (unsigned int i = 0; i < r.csmSplits; i++)
	{
		if (cameraDepth >= g_slice[i].near && cameraDepth < g_slice[i].far)
		{
			split = i;
			//break;
		}
	}

	//float4x4 croppedTransform = mul(g_slice[split].cropMatrix, g_lightSpaceTransform);

	// Apply the crop matrix to get the same matrix used to generate the shadow map
	//float4   croppedLightSpacePosition = mul(g_slice[split].cropMatrix, lightSpacePosition);
	float4x4 croppedLightTransform     = mul(g_slice[split].cropMatrix, g_lightSpaceTransform);
	float4   normalOffsetPosition      = { input.position + input.normal * r.shadowMapNormalOffsetBias, 1 };
	float4   croppedLightSpacePosition = mul(croppedLightTransform, normalOffsetPosition);

	float3 projectTexCoord;

	projectTexCoord.x = croppedLightSpacePosition.x * .5f + .5f;
	projectTexCoord.y = 1.f - (croppedLightSpacePosition.y * .5f + .5f);
	projectTexCoord.z = split;

	float lightDepthValue = ShadowMapBiasedDepth(croppedLightSpacePosition);

	if (r.csmDebug)
	{
		
		float4 debugColors[6] = {
			float4(1, 0, 0, 1),
			float4(0, 1, 0, 1),
			float4(0, 0, 1, 1),
			float4(1, 1, 0, 1),
			float4(1, 0, 1, 1),
			float4(0, 1, 1, 1)
		};

		return debugColors[split];

	}
	else
	{

		// Finally compare the depth with the shadow map value like in regular shadow mapping

#ifdef __MYE_PCF_SHADOW_MAPPING

		float  texelScale = 1.f / r.shadowMapResolution;

		float2 texelDepthDelta;

		float4 lightSpaceOffsetPosition = mul(g_lightSpaceTransform, normalOffsetPosition);

		texelDepthDelta = ShadowMapPCFDepthGradient(lightSpaceOffsetPosition, projectTexCoord);

		//float2 texelDepthDelta = ShadowMapPCFDepthDelta(lightSpaceOffsetPosition, texelScale);

		return ShadowMapCompareDepthPCF(input, croppedLightSpacePosition, projectTexCoord, lightDepthValue, texelScale, texelDepthDelta, croppedLightTransform);

#else

		return ShadowMapCompareDepth(projectTexCoord, lightDepthValue);

#endif

	}

}

#else

float4 ShadowMap(in GBufferData input)
{

	float4 lightSpacePosition = mul(g_lightSpaceTransform, float4(input.position, 1));

	float3 projectTexCoord;

	projectTexCoord.x = lightSpacePosition.x * .5f + .5f;
	projectTexCoord.y = 1.f - (lightSpacePosition.y * .5f + .5f);
	projectTexCoord.z = 0.f;

	float4 normalOffsetPosition     = { input.position + input.normal * r.shadowMapNormalOffsetBias, 1 };
	float4 lightSpaceOffsetPosition = mul(g_lightSpaceTransform, normalOffsetPosition);
	float  lightDepthValue          = ShadowMapBiasedDepth(lightSpaceOffsetPosition);

#ifdef __MYE_PCF_SHADOW_MAPPING
	
	float2 texelDepthDelta;
	float  texelScale = 1.f / r.shadowMapResolution;

	texelDepthDelta = ShadowMapPCFDepthGradient(lightSpaceOffsetPosition, projectTexCoord);

	return ShadowMapCompareDepthPCF(input, lightSpaceOffsetPosition, projectTexCoord, lightDepthValue, texelScale, texelDepthDelta, g_lightSpaceTransform);

#else

	return ShadowMapCompareDepth(projectTexCoord, lightDepthValue);

#endif

}

#endif

float4 ShadowMapVisibility(in GBufferData input)
{

#ifdef MYE_SHADOW_MAP_CSM

	return ShadowMapPSSM(input);

#else

	return ShadowMap(input);

#endif

}

#endif