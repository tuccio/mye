#include "register_slots.hlsli"
#include "renderer_configuration.hlsli"
#include "rsm_read.hlsli"
#include "lpv.hlsli"
#include "quad_input.hlsli"


struct PSOutput
{
	float4 position : SV_Target0;
	float4 normal   : SV_Target1;
	float4 flux     : SV_Target2;
};

PSOutput main(QuadInput input)
{

	float3 maxLuminanceCell = { 0, 0, 0 };

	{

		float maxLuminance = 0.f;

		[unroll]
		for (int i = 0; i < 2; i++)
		{

			[unroll]
			for (int j = 0; j < 2; j++)
			{

				float2 texcoord = input.texcoord + float2(i, j) * r.shadowMapInvResolution;

				RSMTexel texel     = RSMRead(texcoord);
				float    luminance = RSMTexelLuminance(texel);

				if (luminance > maxLuminance)
				{
					maxLuminanceCell = LPVGetGridCell(texel.position);
					maxLuminance     = luminance;
				}


			}

		}

	}

	PSOutput output = (PSOutput) 0;

	int samples = 0;

	[unroll]
	for (int i = 0; i < 2; i++)
	{

		[unroll]
		for (int j = 0; j < 2; j++)
		{

			float2 texcoord = input.texcoord + float2(i, j) * r.shadowMapInvResolution;

			RSMTexel texel        = RSMRead(texcoord);
			float3   cellPosition = LPVGetGridCell(texel.position);
			float3   cellDistance = cellPosition - maxLuminanceCell;

			if (dot(cellDistance, cellDistance) < 3)
			{
				output.position += float4(texel.position, texel.area);
				output.normal   += float4(texel.normal, 1);
				output.flux     += float4(texel.flux, 1);
				samples++;
			}


		}

	}

	if (samples > 0)
	{

		float invSamples = 1.f / samples;

		output.position *= invSamples;
		output.normal   *= invSamples;
		output.flux     *= .25f;

		output.normal = normalize(output.normal);

	}

	return output;

}