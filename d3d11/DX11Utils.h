#pragma once

#pragma warning( disable : 4067 )

#include <Windows.h>
#include <vector>

#include <mye/core/VertexDeclaration.h>
#include <d3d11.h>

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
	#define ReleaseCOMIf(x) { if (x) ReleaseCOM(x); }

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

		inline std::vector<D3D11_INPUT_ELEMENT_DESC> MakeInputElementVector(const mye::core::VertexDeclaration &vd)
		{

			std::vector<D3D11_INPUT_ELEMENT_DESC> v;

			for (mye::core::VertexDeclaration::Attribute a : vd)
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

				default:
					break;

				}

				switch (a.type)
				{

					case mye::core::VertexAttributeType::FLOAT:
						vDesc.Format               = DXGI_FORMAT_R32_FLOAT;
						break;

					case mye::core::VertexAttributeType::FLOAT2:
						vDesc.Format               = DXGI_FORMAT_R32G32_FLOAT;
						break;

					case mye::core::VertexAttributeType::FLOAT3:
						vDesc.Format               = DXGI_FORMAT_R32G32B32_FLOAT;
						break;

					case mye::core::VertexAttributeType::FLOAT4:
						vDesc.Format               = DXGI_FORMAT_R32G32B32A32_FLOAT;
						break;

					case mye::core::VertexAttributeType::INT:
						vDesc.Format               = DXGI_FORMAT_R32_SINT;
						break;

					case mye::core::VertexAttributeType::INT2:
						vDesc.Format               = DXGI_FORMAT_R32G32_SINT;
						break;

					case mye::core::VertexAttributeType::INT3:
						vDesc.Format               = DXGI_FORMAT_R32G32B32_SINT;
						break;

					case mye::core::VertexAttributeType::INT4:
						vDesc.Format               = DXGI_FORMAT_R32G32B32A32_SINT;
						break;
						
					default:
						break;

				}

				vDesc.InputSlot            = 0;
				vDesc.AlignedByteOffset    = D3D11_APPEND_ALIGNED_ELEMENT;
				vDesc.InputSlotClass       = D3D11_INPUT_PER_VERTEX_DATA;
				vDesc.InstanceDataStepRate = 0;

				v.push_back(vDesc);

			}

			return v;
		}

	}

}