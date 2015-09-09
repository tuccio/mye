struct VSInput
{
	float4 position : POSITION;
	uint   slice    : SV_InstanceID;
};

struct VSOutput
{
	float4 positionCS : SV_Position;
	float2 texcoord   : TEXCOORD;
#ifdef MYE_BOX_BLUR_ARRAY
	uint   slice      : SV_InstanceID;
#endif
};

VSOutput main(VSInput input)
{

	VSOutput output;

	output.positionCS = input.position;
	output.texcoord   = float2(input.position.x * .5f + .5f, 1.f - (input.position.y * .5f + .5f));

#ifdef MYE_BOX_BLUR_ARRAY
	output.slice      = input.slice;
#endif
	
	return output;

}