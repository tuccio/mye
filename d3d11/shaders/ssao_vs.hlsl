struct VSOutput
{
	float4 positionCS : SV_position;
	float2 texcoord   : TEXCOORD;
};

/* Main */

VSOutput main(float4 position : POSITION)
{

	float2 texcoord = { .5f * position.x + .5f, 1.f - (.5f * position.y + .5f) };

	VSOutput output;

	output.positionCS = position;
	output.texcoord   = texcoord;

	return output;

}