struct VSOutput
{
	float4 positionCS : SV_position;
};

/* Main */

VSOutput main(float4 position : POSITION)
{

	VSOutput output;

	output.positionCS = position;

	return output;

}