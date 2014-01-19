#pragma once

#pragma warning( disable : 4067 )

#include <Windows.h>

#if defined(DEBUG) | defined(_DEBUG)

	#ifndef HRDEBUG
		
		#define HRDEBUG(hr)\
		if(FAILED(hr))\
		{\
			LPTSTR output;\
			FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS,\
			NULL,\
			hr,\
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),\
			(LPTSTR) &output,\
			0x0,\
			NULL);\
			MessageBox(NULL, output, "DirectX Error", MB_OK);\
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

	#define ReleaseCOM(x) { x->Release(); x = 0x0; }

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
	}
}