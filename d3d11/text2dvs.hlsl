/*struct VertexInputType
{

	float2 position  : POSITION;
	float2 texcoords : TEXCOORD0;

};

struct PixelInputType
{
    float4 position  : SV_POSITION;
    float2 texcoords : TEXCOORD0;
};

PixelInputType main(VertexInputType input)
{

	PixelInputType output;

	output.position  = float4(input.position.x, input.position.y, 0, 1);
	output.texcoords = input.texcoords;

	return output;

}*/

struct VertexInputType
{
	float2 position : POSITION;
	float2 texcoord : TEXCOORD0;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD0;
};

PixelInputType main(VertexInputType input)
{

	PixelInputType output;

	output.position = float4(input.position.x, input.position.y, 0.0f, 1.0f);
	output.texcoord = input.texcoord;

	return output;

}