#pragma pack_matrix(row_major)

struct PSInput
{
	float4 position : SV_Position;
	float4 color    : DIFFUSE;
};

float4 main(PSInput input) : SV_TARGET
{
	return input.color;
}