/*
struct PixelInputType
{
    float4 position  : SV_POSITION;
    float2 texcoords : TEXCOORD0;
};

Texture2D shaderTexture;
SamplerState sampleType;

float4 main(PixelInputType input) : SV_TARGET
{
	
	return float4(1, 1, 1, 1);

}*/

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD0;
};

Texture2D fontTexture    : register(t0);
SamplerState fontSampler : register(s0);

cbuffer colorBuffer
{
	float4 color;
};

float4 main(PixelInputType input) : SV_TARGET
{
	
	//return float4(fontTexture.Sample(fontSampler, input.texcoord).aaa, 1);
	return float4(color.rgb, color.a * fontTexture.Sample(fontSampler, input.texcoord).a);

}