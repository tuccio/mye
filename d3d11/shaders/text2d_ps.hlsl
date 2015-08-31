#pragma pack_matrix(row_major)

struct PSInput
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD0;
};

struct PSOutput
{
	float4 color;
};

Texture2D fontTexture    : register(t0);
SamplerState fontSampler : register(s0);

cbuffer colorBuffer
{
	float4 color;
};

PSOutput main(PSInput input) : SV_TARGET
{
	
	PSOutput output;
	output.color = float4(color.rgb, color.a * fontTexture.Sample(fontSampler, input.texcoord).a);

	return output;

}