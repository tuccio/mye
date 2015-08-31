#ifndef __MYE_SHADOW_MAPPING__
#define __MYE_SHADOW_MAPPING__

#pragma pack_matrix(row_major)

#include "light.hlsli"
#include "register_slots.hlsli"
#include "renderer_configuration.hlsli"

#ifdef __MYE_LIGHT_DIRECTIONAL__

#include "pssm.hlsli"
#include "camera_transform.hlsli"
#include "gbuffer_read.hlsli"

#define MYE_SHADOWMAP_TYPE Texture2DArray<float>

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
#define MYE_SHADOWMAP_TYPE Texture2D<float>
#endif

cbuffer cbLightSpace : register(__MYE_DX11_BUFFER_SLOT_LIGHTSPACETRANSFORM)
{
	float4x4 g_lightSpaceTransform;
}

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

MYE_SHADOWMAP_TYPE     g_shadowMap           : register(__MYE_DX11_TEXTURE_SLOT_SHADOWMAP);
SamplerState           g_shadowMapSampler    : register(__MYE_DX11_SAMPLER_SLOT_SHADOWMAP);
SamplerComparisonState g_shadowMapSamplerCmp : register(__MYE_DX11_SAMPLER_SLOT_SHADOWMAP_CMP);
SamplerState           g_randomSampler       : register(__MYE_DX11_SAMPLER_SLOT_RANDOM);

/* Random cos/sin */

Texture2D<float2> g_randomCosSin : register(__MYE_DX11_TEXTURE_SLOT_RANDOM);

/* Shadow mapping */

float ShadowMapBiasedDepth(in float4 lightSpacePosition)
{
	return lightSpacePosition.z - r.shadowMapBias;
}

void CalculateRightAndUpTexelDepthDeltas(in float3 vShadowTexDDX,
										 in float3 vShadowTexDDY,
										 in float  texelSize,
										 out float fUpTextDepthWeight,
										 out float fRightTextDepthWeight
										 ) {

	// We use the derivatives in X and Y to create a transformation matrix.  Because these derivives give us the 
	// transformation from screen space to shadow space, we need the inverse matrix to take us from shadow space 
	// to screen space.  This new matrix will allow us to map shadow map texels to screen space.  This will allow 
	// us to find the screen space depth of a corresponding depth pixel.
	// This is not a perfect solution as it assumes the underlying geometry of the scene is a plane.  A more 
	// accureate way of finding the actual depth would be to do a deferred rendering approach and actually 
	//sample the depth.

	// Using an offset, or using variance shadow maps is a better approach to reducing these artifacts in most cases.

	float2x2 matScreentoShadow = float2x2(vShadowTexDDX.xy, vShadowTexDDY.xy);
	float fDeterminant = determinant(matScreentoShadow);

	float fInvDeterminant = 1.0f / fDeterminant;

	float2x2 matShadowToScreen = float2x2 (
		matScreentoShadow._22 * fInvDeterminant, matScreentoShadow._12 * -fInvDeterminant,
		matScreentoShadow._21 * -fInvDeterminant, matScreentoShadow._11 * fInvDeterminant);

	float2 vRightShadowTexelLocation  = float2(texelSize, 0.0f);
	float2 vUpShadowTexelLocation = float2(0.0f, texelSize);

	// Transform the right pixel by the shadow space to screen space matrix.
	float2 vRightTexelDepthRatio = mul(vRightShadowTexelLocation, matShadowToScreen);
	float2 vUpTexelDepthRatio = mul(vUpShadowTexelLocation, matShadowToScreen);

	// We can now caculate how much depth changes when you move up or right in the shadow map.
	// We use the ratio of change in x and y times the dervivite in X and Y of the screen space 
	// depth to calculate this change.
	fUpTextDepthWeight =
		vUpTexelDepthRatio.x * vShadowTexDDX.z
		+ vUpTexelDepthRatio.y * vShadowTexDDY.z;
	fRightTextDepthWeight =
		vRightTexelDepthRatio.x * vShadowTexDDX.z
		+ vRightTexelDepthRatio.y * vShadowTexDDY.z;

}

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

float4 ShadowMapCompareDepthPCF(in GBufferData input, in float4 lightSpacePosition, in float3 projectTexCoord, in float lightDepthValue, in float texelScale, in float2 depthGradient, in float4x4 lightSpaceTransform)
{

	//float shadowFactor = 0.f;

	//int pcfLoopEnd   = ((uint) r.pcfKernel) >> 1;
	//int pcfLoopBegin = - pcfLoopEnd;

	//for (int i = pcfLoopBegin; i <= pcfLoopEnd; i++)
	//{

	//	for (int j = pcfLoopBegin; j <= pcfLoopEnd; j++)
	//	{

	//		float2 delta             = float2(i, j);
	//		float2 lightSpaceDelta   = delta * texelScale;
	//		float  slopeScaledBias   = dot(lightSpaceDelta, depthGradient)/* * r.shadowMapSlopeScaledBias*/;
	//		float  depthLinearApprox = lightDepthValue + slopeScaledBias;

	//		float  sampleContribution = g_shadowMap.SampleCmpLevelZero(g_shadowMapSamplerCmp, projectTexCoord + float3(lightSpaceDelta, 0), depthLinearApprox);

	//		shadowFactor += sampleContribution;

	//	}
	//}

	//shadowFactor *= r.pcfKernelInvSquare;

	/*float shadowFactor =
		0.25f * (
		g_shadowMap.SampleCmpLevelZero(g_shadowMapSamplerCmp, projectTexCoord + float3(float2(-0.5f,  0.5f) * texelScale, 0), lightDepthValue) +
		g_shadowMap.SampleCmpLevelZero(g_shadowMapSamplerCmp, projectTexCoord + float3(float2( 0.5f,  0.5f) * texelScale, 0), lightDepthValue) +
		g_shadowMap.SampleCmpLevelZero(g_shadowMapSamplerCmp, projectTexCoord + float3(float2(-1.5f, -1.5f) * texelScale, 0), lightDepthValue) +
		g_shadowMap.SampleCmpLevelZero(g_shadowMapSamplerCmp, projectTexCoord + float3(float2( 0.5f, -1.5f) * texelScale, 0), lightDepthValue));*/

	float3 offsetVector           = cross(input.normal, g_light.direction.xyz);
	float3 lightSpaceOffsetVector = normalize(mul((float3x3) lightSpaceTransform, offsetVector));

	float2 rotationCosSin = g_randomCosSin.Sample(g_randomSampler, float2(input.screenPosition) / r.resolution);

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

		shadowFactor += g_shadowMap.SampleCmpLevelZero(g_shadowMapSamplerCmp, projectTexCoord + float3(lightSpaceOffset, 0), lightDepthValue + slopeScaledBias);
		//shadowFactor += g_shadowMap.SampleCmpLevelZero(g_shadowMapSamplerCmp, projectTexCoord, lightDepthValue);

	}

	shadowFactor *= __MYE_POISSON_DISK_INVERSE_TAPS;

	return float4(shadowFactor, shadowFactor, shadowFactor, 1);

}

float4 ShadowMapCompareDepth(in float3 projectTexCoord, in float lightDepthValue)
{

	float depthValue = g_shadowMap.Sample(g_shadowMapSampler, projectTexCoord);

	if (depthValue < lightDepthValue)
	{
		return float4(0.f, 0.f, 0.f, 0.f);
	}
	else
	{
		return float4(1.f, 1.f, 1.f, 1.f);
	}

}

#if defined(__MYE_LIGHT_DIRECTIONAL__)

float4 ShadowMapPSSM(in GBufferData input)
{

	float4 x = { input.position, 1 };
	//float4 lightSpacePosition = mul(g_lightSpaceTransform, x);

	// Get the camera space z to choose what split we should pick
	// as our shadow map

	float4 cameraSpace = mul(g_camera.viewProj, x);
	float  cameraDepth = cameraSpace.z;

	int split = r.csmSplits;

	for (int i = 0; i < r.csmSplits; i++)
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
	
	// Finally compare the depth with the shadow map value like in regular shadow mapping

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

		if (r.pcfEnabled)
		{

			float  texelScale = 1.f / r.shadowMapResolution;

			float2 texelDepthDelta;

			float4 lightSpaceOffsetPosition = mul(g_lightSpaceTransform, normalOffsetPosition);
			//CalculateRightAndUpTexelDepthDeltas(ddx(lightSpaceOffsetPosition), ddy(lightSpaceOffsetPosition), texelScale, texelDepthDelta.x, texelDepthDelta.y);
			texelDepthDelta = ShadowMapPCFDepthGradient(lightSpaceOffsetPosition, projectTexCoord);

			//float2 texelDepthDelta = ShadowMapPCFDepthDelta(lightSpaceOffsetPosition, texelScale);

			return ShadowMapCompareDepthPCF(input, croppedLightSpacePosition, projectTexCoord, lightDepthValue, texelScale, texelDepthDelta, croppedLightTransform);

		}
		else
		{

			return ShadowMapCompareDepth(projectTexCoord, lightDepthValue);

		}

	}

}

#endif


#else

float4 ShadowMap(in GBufferData input)
{

	float4 lightSpacePosition = mul(g_lightSpaceTransform, float4(input.position, 1));

	float3 projectTexCoord;

	projectTexCoord.x = (lightSpacePosition.x / lightSpacePosition.w) * .5f + .5f;
	projectTexCoord.y = 1.f - ((lightSpacePosition.y / lightSpacePosition.w) * .5f + .5f);
	projectTexCoord.z = 0.f;

	float  texelScale;
	float2 texelDepthDelta;

	float lightDepthValue = ShadowMapBiasedDepth(lightSpacePosition, texelScale, texelDepthDelta);

	return float4(ShadowMapCompareDepth(projectTexCoord, lightDepthValue, texelScale, texelDepthDelta).rrr, 1);

}

#endif

float4 ShadowMapVisibility(in GBufferData input)
{

#if defined(__MYE_LIGHT_DIRECTIONAL__)

	return ShadowMapPSSM(input);

#else

	return ShadowMap(input);

#endif

}