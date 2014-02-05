
cbuffer cbPerObject
{
	float4 g_color;
};

float4 main() : SV_TARGET
{
	return g_color;
}