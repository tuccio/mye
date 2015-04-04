#ifndef __MYE_BRDF__
#define __MYE_BRDF__

#include "Material.hlsli"

void BRDFDiffuse(in float3 x,
				 in float3 N,
				 in Material material,
				 in float3 L,
				 in float3 intensity,
				 out float4 diffuse)
{
	float NdotL = saturate(dot(N, L));
	diffuse = float4(material.diffuseColor * NdotL, 1);
}

void BRDFSpecular(in float3 eye,
				  in float3 x,
				  in float3 N,
				  in Material material,
				  in float3 L,
				  in float3 intensity,
				  out float4 specular)
{
	specular = float4(0, 0, 0, 0);
}

void BRDF(in float3 eye,
		  in float3 x,
		  in float3 N,
		  in Material material,
		  in float3 L,
		  in float3 intensity,
		  out float4 color)
{

	float4 fd, fs;

	BRDFDiffuse(x, N, material, L, intensity, fd);
	BRDFSpecular(eye, x, N, material, L, intensity, fs);

	color = saturate(fd + fs);

}

#endif