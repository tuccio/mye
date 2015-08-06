struct GSInput
{
	float4 cell   : SV_Position;
	float3 normal : NORMAL;
	float3 flux   : FLUX;
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
          inout PointStream<GSOutput> output
)
{

	GSOutput element;

	element.depth      = input[0].cell.z;
	element.positionCS = float4(input[0].cell.xy, 0.f, 1.f);

	element.normal     = input[0].normal;
	element.flux       = input[0].flux;

	output.Append(element);

}