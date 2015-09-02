#pragma once

#pragma warning( disable : 4067 )

#include <Windows.h>
#include <d3d11.h>

#include <vector>
#include <cassert>

#include <mye/core/VertexDeclaration.h>
#include "DX11Defines.h"

namespace mye
{

	namespace dx11
	{

		enum class DX11PipelineStage
		{
			INPUT_ASSEMBLER,
			VERTEX_SHADER,
			GEOMETRY_SHADER,
			STREAM_OUT_SHADER,
			RASTERIZER,
			PIXEL_SHADER,
			OUTPUT_MERGER,
			COMPUTE_SHADER
		};

		inline DXGI_FORMAT GetDXGIFormat(mye::core::DataFormat type)
		{

			DXGI_FORMAT format;

			switch (type)
			{

			case mye::core::DataFormat::FLOAT:
				format = DXGI_FORMAT_R32_FLOAT;
				break;

			case mye::core::DataFormat::FLOAT2:
				format = DXGI_FORMAT_R32G32_FLOAT;
				break;

			case mye::core::DataFormat::FLOAT3:
				format = DXGI_FORMAT_R32G32B32_FLOAT;
				break;

			case mye::core::DataFormat::FLOAT4:
				format = DXGI_FORMAT_R32G32B32A32_FLOAT;
				break;

			case mye::core::DataFormat::HALF:
				format = DXGI_FORMAT_R16_FLOAT;
				break;

			case mye::core::DataFormat::HALF2:
				format = DXGI_FORMAT_R16G16_FLOAT;
				break;

			case mye::core::DataFormat::HALF4:
				format = DXGI_FORMAT_R16G16B16A16_FLOAT;
				break;

			case mye::core::DataFormat::INT:
				format = DXGI_FORMAT_R32_SINT;
				break;

			case mye::core::DataFormat::INT2:
				format = DXGI_FORMAT_R32G32_SINT;
				break;

			case mye::core::DataFormat::INT3:
				format = DXGI_FORMAT_R32G32B32_SINT;
				break;

			case mye::core::DataFormat::INT4:
				format = DXGI_FORMAT_R32G32B32A32_SINT;
				break;

			case mye::core::DataFormat::BYTE:
				format = DXGI_FORMAT_R8_UINT;
				break;

			case mye::core::DataFormat::BYTE2:
				format = DXGI_FORMAT_R8G8_UINT;
				break;

			case mye::core::DataFormat::BYTE3:
				format = DXGI_FORMAT_R8G8B8A8_UINT;
				break;

			case mye::core::DataFormat::BYTE4:
				format = DXGI_FORMAT_R8G8B8A8_UINT;
				break;

			case mye::core::DataFormat::RGBA32:
				format = DXGI_FORMAT_R8G8B8A8_UNORM;
				break;

			case mye::core::DataFormat::RGB96:
				format = DXGI_FORMAT_R32G32B32_FLOAT;
				break;

			case mye::core::DataFormat::RGBA128:
				format = DXGI_FORMAT_R32G32B32A32_FLOAT;
				break;

			case mye::core::DataFormat::sRGBA32:
				format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
				break;

			default:
				format = DXGI_FORMAT_UNKNOWN;
				break;

			}

			return format;

		}

		inline std::vector<D3D11_INPUT_ELEMENT_DESC> MakeInputElementVector(const mye::core::VertexDeclaration &vd)
		{

			std::vector<D3D11_INPUT_ELEMENT_DESC> v;
			size_t offset = 0;

			for (mye::core::VertexAttribute a : vd)
			{

				D3D11_INPUT_ELEMENT_DESC vDesc;

				switch (a.semantic)
				{
					
				case mye::core::VertexAttributeSemantic::POSITION:
					vDesc.SemanticName         = "POSITION";
					vDesc.SemanticIndex        = 0;	
					break;

				case mye::core::VertexAttributeSemantic::TEXCOORD0:
					vDesc.SemanticName         = "TEXCOORD";
					vDesc.SemanticIndex        = 0;
					break;

				case mye::core::VertexAttributeSemantic::TEXCOORD1:
					vDesc.SemanticName         = "TEXCOORD";
					vDesc.SemanticIndex        = 1;
					break;

				case mye::core::VertexAttributeSemantic::TEXCOORD2:
					vDesc.SemanticName         = "TEXCOORD";
					vDesc.SemanticIndex        = 2;
					break;

				case mye::core::VertexAttributeSemantic::TEXCOORD3:
					vDesc.SemanticName         = "TEXCOORD";
					vDesc.SemanticIndex        = 3;
					break;

				case mye::core::VertexAttributeSemantic::TEXCOORD4:
					vDesc.SemanticName         = "TEXCOORD";
					vDesc.SemanticIndex        = 4;
					break;

				case mye::core::VertexAttributeSemantic::TEXCOORD5:
					vDesc.SemanticName         = "TEXCOORD";
					vDesc.SemanticIndex        = 5;
					break;

				case mye::core::VertexAttributeSemantic::TEXCOORD6:
					vDesc.SemanticName         = "TEXCOORD";
					vDesc.SemanticIndex        = 6;
					break;

				case mye::core::VertexAttributeSemantic::TEXCOORD7:
					vDesc.SemanticName         = "TEXCOORD";
					vDesc.SemanticIndex        = 7;
					break;

				case mye::core::VertexAttributeSemantic::NORMAL:
					vDesc.SemanticName         = "NORMAL";
					vDesc.SemanticIndex        = 0;
					break;

				case mye::core::VertexAttributeSemantic::TANGENT:
					vDesc.SemanticName         = "TANGENT";
					vDesc.SemanticIndex        = 0;
					break;

				case mye::core::VertexAttributeSemantic::BITANGENT:
					vDesc.SemanticName         = "BITANGENT";
					vDesc.SemanticIndex        = 0;
					break;

				case mye::core::VertexAttributeSemantic::DIFFUSE:
					vDesc.SemanticName         = "DIFFUSE";
					vDesc.SemanticIndex        = 0;
					break;

				case mye::core::VertexAttributeSemantic::SPECULAR:
					vDesc.SemanticName         = "SPECULAR";
					vDesc.SemanticIndex        = 0;
					break;

				default:
					break;

				}

				vDesc.Format = GetDXGIFormat(a.type);

				vDesc.InputSlot            = 0;
				vDesc.AlignedByteOffset    = offset;
				vDesc.InputSlotClass       = D3D11_INPUT_PER_VERTEX_DATA;
				vDesc.InstanceDataStepRate = 0;

				v.push_back(vDesc);

				offset += mye::core::GetDataTypeSize(a.type);

			}

			return v;

		}

	}

}