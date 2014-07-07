#pragma once

#pragma warning( disable : 4067 )

#include <Windows.h>
#include <d3d11.h>

#include <vector>
#include <cassert>

#include <mye/core/VertexDeclaration.h>

#if defined(DEBUG) | defined(_DEBUG)

	#ifndef HRDEBUG
		
		#define HRDEBUG(hr)\
		if(FAILED(hr))\
		{\
			LPTSTR output;\
			FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS,\
			nullptr,\
			hr,\
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),\
			(LPTSTR) &output,\
			0x0,\
			nullptr);\
			MessageBox(nullptr, output, "DirectX Error", MB_OK);\
		}
	#endif

namespace mye
{
	namespace dx11
	{
		bool HRTestFailed(HRESULT hr);		
	}
}

	#ifndef HRTESTFAILED(hr)
		#define HRTESTFAILED(hr) mye::dx11::HRTestFailed(hr)
	#endif

#else

	#ifndef HRDEBUG
		#define HRDEBUG(x)
	#endif

	#ifndef HRTESTFAILED(hr)
		#define HRTESTFAILED(hr) FAILED(hr)
	#endif

#endif

#ifndef ReleaseCOM(x)

	#define ReleaseCOM(x) { x->Release(); x = nullptr; }
	#define ReleaseCOMOptional(x) { if (x) ReleaseCOM(x); }

#endif

namespace mye
{

	namespace dx11
	{

		enum PipelineStages
		{
			PIPELINE_INPUT_ASSEMBLER,
			PIPELINE_VERTEX_SHADER,
			PIPELINE_GEOMETRY_SHADER,
			PIPELINE_STREAM_OUT_SHADER,
			PIPELINE_RASTERIZER,
			PIPELINE_PIXEL_SHADER,
			PIPELINE_OUTPUT_MERGER
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