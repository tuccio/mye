struct GSInput
{
	float4 positionCS : SV_Position;
	float3 cell       : CELL;
	float3 normal     : NORMAL;
	float3 flux       : FLUX;
};

struct GSOutput
{
	float4 positionCS : SV_Position;
	uint   depth      : SV_RenderTargetArrayIndex;
	float3 normal     : NORMAL;
	float3 flux       : FLUX;
};

[maxvertexcount(1)]
void main(point GSInput input[1],
          inout PointStream<GSOutput> output)
{

	GSOutput element;

	element.depth      = input[0].cell.z;
	element.positionCS = input[0].positionCS;

	element.normal     = input[0].normal;
	element.flux       = input[0].flux;

	output.Append(element);

}