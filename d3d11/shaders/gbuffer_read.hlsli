#ifndef __MYE_GBUFFER_READ__
#define __MYE_GBUFFER_READ__

/* G-Buffer */

Texture2D g_gbuffer0 : register(__MYE_DX11_TEXTURE_SLOT_GBUFFER0);
Texture2D g_gbuffer1 : register(__MYE_DX11_TEXTURE_SLOT_GBUFFER1);

struct GBufferData
{
	int2   screenPosition;
	float3 position;
	float3 normal;
	float  specularPower;
};

GBufferData GBufferRead(in int2 screenPosition)
{

	GBufferData data;

	int3 texturePosition = int3(screenPosition.xy, 0);

	float4 gbuffer0 = g_gbuffer0.Load(texturePosition);
	float4 gbuffer1 = g_gbuffer1.Load(texturePosition);

	data.screenPosition = screenPosition;

	data.normal         = gbuffer0.xyz;
	data.specularPower  = gbuffer0.w;

	data.position       = gbuffer1.xyz;

	return data;

}

#endif