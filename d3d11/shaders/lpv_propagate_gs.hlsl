struct GSInput
{
	float4 positionCS : SV_Position;
	uint   depth      : SV_InstanceID;
	//float3 cell       : CELL;
};

struct GSOutput
{
	float4 positionCS : SV_Position;
	uint   depth      : SV_RenderTargetArrayIndex;
	//float3 cell       : CELL;
};

//[maxvertexcount(1)]
//void main(point GSInput input[1],
//          inout PointStream<GSOutput> output)
//{
//
//
//	GSOutput element;
//
//	element.positionCS = input[0].positionCS;
//	element.depth      = input[0].cell.z;
//	element.cell       = input[0].cell;
//
//	output.Append(element);
//
//}

[maxvertexcount(3)]
void main(triangle GSInput input[3],
          inout TriangleStream<GSOutput> output)
{


	GSOutput element;
	
	element.depth = input[0].depth;

	[unroll]
	for (int i = 0; i < 3; i++)
	{

		element.positionCS = input[i].positionCS;

		output.Append(element);

	}


}