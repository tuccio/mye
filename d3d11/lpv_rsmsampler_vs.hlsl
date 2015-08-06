struct VSInput
{
	float4 position : POSITION;
};

struct VSOutput
{
	float4 position : SV_Position;
	float2 texcoord : TEXCOORD0;
};

VSOutput main(VSInput input)
{
	
	VSOutput output;

	output.position = input.position;
	output.texcoord = float2(.5f + .5f * input.position.x, .5f - .5f * input.position.y);

	return output;

}