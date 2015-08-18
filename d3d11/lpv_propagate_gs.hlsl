struct GSInput
{
	float4 positionCS : SV_Position;
	float3 cell       : CELL;
};

struct GSOutput
{
	float4 positionCS : SV_Position;
	uint   depth      : SV_RenderTargetArrayIndex;
	float3 cell       : CELL;
};

[maxvertexcount(1)]
void main(point GSInput input[1],
          inout PointStream<GSOutput> output)
{


	GSOutput element;

	element.positionCS = input[0].positionCS;
	element.depth      = input[0].cell.z;
	element.cell       = input[0].cell;

	output.Append(element);

}