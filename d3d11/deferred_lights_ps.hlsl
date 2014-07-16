#pragma pack_matrix(row_major)

/* Constant buffers */

#define PI 3.14159265
#define GAMMA 1.8

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

void ReadGBuffer(in int2 screenPosition, out float3 position, out float3 normal, out float3 diffuseAlbedo, out float3 specularAlbedo, out float specularPower)
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

float3 Gamma(float3 color)
{
	return pow(color, GAMMA);
	//return color;
}

float SchlickFresnel(float u)
{
	float m = clamp(1 - u, 0, 1);
	float m2 = m * m;
	return m2 * m2 * m;
}

float sqr(float a)
{
	return a * a;
}

float GTR2_aniso(float NdotH, float HdotX, float HdotY, float ax, float ay)
{
	return 1 / (PI * ax * ay * sqr(sqr(HdotX / ax) + sqr(HdotY / ay) + NdotH * NdotH));
}

float smithG_GGX(float Ndotv, float alphaG)
{
	float a = alphaG * alphaG;
	float b = Ndotv * Ndotv;
	return 1 / (Ndotv + sqrt(a + b - a * b));
}

float3 BRDF(in float3 L, in float3 V, in float3 N, in float3 diffuseColor, in float3 specularColor, in float roughness, in float metallic)
{

	float NdotL = dot(N, L);
	float NdotV = dot(N, V);

	if (NdotL < 0 || NdotV < 0)
	{
		return float3(0, 0, 0);
	}

	float3 H = normalize(L + V);

	float NdotH = dot(N, H);
	float LdotH = dot(L, H);

	float3 diffuseColorLinear = Gamma(diffuseColor);

	float FL = SchlickFresnel(NdotL);
	float FV = SchlickFresnel(NdotV);

	float Fd90 = 0.5 + 2 * LdotH * LdotH * roughness;
	float Fd = lerp(1, Fd90, FL) * lerp(1, Fd90, FV);

	/*float aspect = sqrt(1 - anisotropic * .9);
	float ax = max(.001, sqr(roughness) / aspect);
	float ay = max(.001, sqr(roughness)*aspect);
	float Ds = GTR2_aniso(NdotH, dot(H, X), dot(H, Y), ax, ay);*/

	return Fd * diffuseColorLinear;

}

float3 BRDF_(in float3 L, in float3 V, in float3 N, in float3 diffuseColor, in float3 specularColor, in float roughness, in float metallic)
{


	return Gamma(diffuseColor) * max(0, dot(N, L));

}

void ComputeLighting(in float3 p, in float3 N, in float3 diffuseAlbedo, in float3 specularAlbedo, in float specularPower, out float4 color)
{

	// Cook-Torrance for specular and Lambert for diffuse

	float3 L = (lightPosition.xyz - p);
	float3 V = normalize(cameraPosition.xyz - p);

	float lightDistance = length(L);

	L /= lightDistance;

	float lightAttenuation = max(0, 1 - (lightDistance / lightRange));

	color = float4(Gamma(lightColor.xyz) * lightAttenuation * BRDF(L, V, N, diffuseAlbedo, specularAlbedo, 0.5, 0.0), 1);


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

	return float4(color);

}