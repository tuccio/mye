#ifndef __MYE_MATERIAL__
#define __MYE_MATERIAL__

struct Material
{
	float4 diffuseColor;
	float4 specularColor;
	float  specular;
	float  roughness;
	float  metallic;
};

#endif