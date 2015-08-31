#include "light.hlsli"

void ComputeDirectLighting(in float3 position, in float3 normal, in float specularPower, in Light light, in float3 cameraPosition, out float4 lightBuffer)
{

	float3 L = light.position - position;
	float distance = length(L);
	float attenuation = max(0, 1 - (distance / light.range));

	L /= distance;

	float NdotL = saturate(dot(normal, L));
	float3 diffuse = NdotL * light.color * attenuation;

	float3 V = cameraPosition - position;
	float3 H = normalize(L + V);
	float specular = pow(saturate(dot(normal, H)), specularPower) * attenuation * NdotL;

	lightBuffer = float4(diffuse, specular);

}