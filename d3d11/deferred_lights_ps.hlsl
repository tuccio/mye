#pragma pack_matrix(row_major)

/* Constant buffers */


cbuffer cbLight
{

	float4 lightColor;
	float4 lightPosition;
	float4 lightDirection;

	float  lightSpotAngle;
	float  lightRange;

};

cbuffer cbCamera
{
	float4 cameraPosition;
};

/* G-Buffer */

Texture2D g_diffuseTexture  : register(t0);
Texture2D g_specularTexture : register(t1);
Texture2D g_normalTexture   : register(t2);
Texture2D g_positionTexture : register(t3);

void ReadGBuffer(
	in int2 screenPosition,
	out float3 position,
	out float3 normal,
	out float3 diffuseAlbedo,
	out float3 specularAlbedo,
	out float specularPower)
{

	float4 packedSpecular;
	int3 texturePosition = int3(screenPosition.xy, 0);

	position = g_positionTexture.Load(texturePosition).xyz;
	normal = g_normalTexture.Load(texturePosition).xyz;

	diffuseAlbedo = g_diffuseTexture.Load(texturePosition).xyz;

	packedSpecular = g_specularTexture.Load(texturePosition);

	specularAlbedo = packedSpecular.xyz;
	specularPower = packedSpecular.w;

}

void ComputeLighting(
	in float3 position,
	in float3 normal,
	in float3 diffuseAlbedo,
	in float3 specularAlbedo,
	in float specularPower,
	out float4 color)
{

	float3 L = (lightPosition.xyz - position);
	float distance = length(L);

	L /= distance;

	float attenuation = max(0, lightRange / (distance * distance));

	float3 diffuse = saturate(dot(normal, L)) * diffuseAlbedo * lightColor.xyz;

	color = float4(attenuation * diffuse, 1);

}

float4 main(float4 screenPosition : SV_Position) : SV_TARGET0
{

	float3 position;
	float3 normal;
	float3 diffuseAlbedo;
	float3 specularAlbedo;
	float specularPower;

	float4 color;

	ReadGBuffer(screenPosition.xy, position, normal, diffuseAlbedo, specularAlbedo, specularPower);
	ComputeLighting(position, normal, diffuseAlbedo, specularAlbedo, specularPower, color);

	//float2 coords = float2(0.5f, 0.5f) * (screenPosition.xy + float2(1, 1));
	float2 coords = screenPosition.xy;
	//	return g_diffuseTexture.Sample(g_sampler, );

	return float4(color);

	//return float4(1, 1, 1, 1);

}