#pragma pack_matrix(row_major)

struct GSInput
{

	float4 positionCS : SV_Position;

	float2 texcoord   : TEXCOORD;

	float3 normalWS   : NORMALWS;
	float3 positionWS : POSITIONWS;

	uint   slice      : SV_InstanceID;

};

struct GSOutput
{

	float4 positionCS : SV_Position;

	float2 texcoord   : TEXCOORD;

	float3 normalWS   : NORMALWS;
	float3 positionWS : POSITIONWS;

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

		element.normalWS   = input[i].normalWS;
		element.positionWS = input[i].positionWS;

		output.Append(element);

	}

	output.RestartStrip();

}