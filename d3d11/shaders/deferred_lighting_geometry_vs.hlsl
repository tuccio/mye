#pragma pack_matrix(row_major)

#ifdef MYE_USE_HEIGHT_MAP

#include "camera_transform.hlsli"

#endif

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
	float3 normal    : NORMAL;

#ifdef MYE_USE_HEIGHT_MAP

	float2 texcoord  : TEXCOORD0;

	float3 tangent   : TANGENT;
	float3 bitangent : BITANGENT;

#endif

};

struct VSOutput
{

	float4 positionCS  : SV_Position;
	float3 normalWS    : NORMALWS;
	float3 positionWS  : POSITIONWS;

#ifdef MYE_USE_HEIGHT_MAP

	float2 texcoord    : TEXCOORD0;

	float3 eyeTS       : EYETS;

	float3 normalTS    : NORMALTS;

	float3 tangentWS   : TANGENTWS;
	float3 bitangentWS : BITANGENTWS;

#endif

};

/* Main */

VSOutput main(VSInput input)
{

	float3 normalWS = mul((float3x3) g_world, input.normal);

	VSOutput output;

	output.positionCS  = mul(g_worldViewProj, float4(input.position, 1));

	output.normalWS    = normalWS;
	output.positionWS  = mul((float3x3) g_world, input.position);

#ifdef MYE_USE_HEIGHT_MAP

	float3 tangentWS   = mul((float3x3) g_world, input.tangent);
	float3 bitangentWS = mul((float3x3) g_world, input.bitangent);

	float3x3 tangentToWorldSpace = {
		tangentWS,
		bitangentWS,
		normalWS
	};

	float3x3 worldToTangentSpace = transpose(tangentToWorldSpace);

	float3 eyeWS = output.positionWS - g_camera.position;

	output.eyeTS       = mul(worldToTangentSpace, eyeWS);
	output.normalTS    = mul(worldToTangentSpace, normalWS);
				       
	output.texcoord    = input.texcoord;

	output.tangentWS   = tangentWS;
	output.bitangentWS = bitangentWS;

#endif

	return output;

}