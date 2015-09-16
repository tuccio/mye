#pragma pack_matrix(row_major)

#include "camera_transform.hlsli"
#include "linear_depth.hlsli"

/* Constant buffers */

cbuffer cbTransform : register(b0)
{
	float4x4 g_world;
	float4x4 g_worldView;
	float4x4 g_worldViewProj;
};

/* Input/Output structures */

struct VSInput
{
	float3 position  : POSITION;
	float2 texcoord  : TEXCOORD0;
	float3 normal    : NORMAL;
	float3 tangent   : TANGENT;
	float3 bitangent : BITANGENT;
};

struct VSOutput
{

	float4 positionCS  : SV_Position;
	float3 normalWS    : NORMALWS;
	float3 positionWS  : POSITIONWS;

	float2 texcoord    : TEXCOORD0;

#ifdef MYE_USE_NORMAL_HEIGHT_MAP

	float3 normalTS    : NORMALTS;

	float3 tangentWS   : TANGENTWS;
	float3 bitangentWS : BITANGENTWS;

#ifdef MYE_USE_PARALLAX

	float3 eyeTS       : EYETS;

#endif

#endif

};

/* Main */

VSOutput main(VSInput input)
{

	VSOutput output;

	float3 normalWS   = mul((float3x3) g_world, input.normal);

	float4 positionCS = mul(g_worldViewProj, float4(input.position, 1));
	output.positionCS = LinearizeDepth(positionCS);

	output.normalWS   = normalWS;
	output.positionWS = mul(g_world, float4(input.position, 1)).xyz;

	output.texcoord    = input.texcoord;

#ifdef MYE_USE_NORMAL_HEIGHT_MAP

	float3 tangentWS   = mul((float3x3) g_world, input.tangent);
	float3 bitangentWS = mul((float3x3) g_world, input.bitangent);

	float3x3 worldToTangentSpace = {
		tangentWS,
		bitangentWS,
		normalWS
	};

	output.normalTS    = mul(worldToTangentSpace, normalWS);

	output.tangentWS   = tangentWS;
	output.bitangentWS = bitangentWS;

#ifdef MYE_USE_PARALLAX

	float3 eyeWS = output.positionWS - g_camera.position;

	output.eyeTS = mul(worldToTangentSpace, eyeWS);

#endif

#endif

	return output;

}