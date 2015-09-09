struct GSInput
{
	float4 positionCS : SV_Position;
	float2 texcoord   : TEXCOORD;
	uint   slice      : SV_InstanceID;
};

struct GSOutput
{
	float4 positionCS : SV_Position;
	float2 texcoord   : TEXCOORD;
	uint   target     : SV_RenderTargetArrayIndex;
};

[maxvertexcount(3)]
void main(triangle GSInput input[3],
          inout TriangleStream<GSOutput> output)
{

	GSOutput element;

	element.target = input[0].slice;

	for (int i = 0; i < 3; i++)
	{

		element.positionCS = input[i].positionCS;
		element.texcoord   = input[i].texcoord;
		
		output.Append(element);

	}

	output.RestartStrip();

}