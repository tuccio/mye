#pragma pack_matrix(row_major)

/* Constant buffers */

cbuffer cbTransform : register(b0)
{
	float4x4 g_world;
	float4x4 g_worldView;
	float4x4 g_worldViewProj;
};

#ifdef MYE_USE_HEIGHT_MAP

Texture2D<float> g_heightMap        : register(__MYE_DX11_TEXTURE_SLOT_HEIGHTMAP);
SamplerState     g_heightMapSampler : register(__MYE_DX11_SAMPLER_SLOT_HEIGHTMAP);

#endif

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

};

/* Main */

VSOutput main(VSInput input)
{

	VSOutput output;

	output.positionCS  = mul(g_worldViewProj, float4(input.position, 1));

	output.normalWS    = mul(input.normal, (float3x3) g_world);
	output.positionWS  = mul((float3x3) g_world, input.position);

	return output;

}