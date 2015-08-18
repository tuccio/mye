#ifndef __MYE_MATERIAL__
#define __MYE_MATERIAL__

struct Material
{
	float3 diffuseColor;
	float3 specularColor;
	float  specular;
	float  roughness;
	float  metallic;
};

#endif