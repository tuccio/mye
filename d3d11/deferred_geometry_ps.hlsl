#pragma pack_matrix(row_major)

/* Constant buffers */

cbuffer cbMaterial
{
	float3 g_diffuse;
	float3 g_specular;
	float  g_specularPower;
};

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

	float4 diffuseAlbedo  : SV_Target0;
	float4 specularAlbedo : SV_Target1;
	float4 normal         : SV_Target2;
	float4 position       : SV_Target3;

};

/* Main */

PSOutput main(PSInput input) : SV_TARGET
{
	
	PSOutput output;

	output.diffuseAlbedo  = float4(g_diffuse, 1);
	output.specularAlbedo = float4(g_specular, g_specularPower);

	output.normal         = float4(normalize(input.normalWS), 1);
	output.position       = float4(input.positionWS, 1);

	return output;

}