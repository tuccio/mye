#pragma once

#include "DX11ShaderProgram.h"
#include "DX11Texture.h"

namespace mye
{

	namespace dx11
	{

		class DX11VarianceShadowMap
		{

		public:

			DX11VarianceShadowMap(void);

			bool Create(void);
			void Destroy(void);

			void Render(void);

			DX11ShaderResource & GetVSM(void);

			int  GetResolution(void) const;
			void SetResolution(int resolution);

			int  GetSplits(void) const;
			void SetSplits(int splits);

			void SetExponentialTest(bool exponentialTest);
			bool GetExponentialTest(void) const;

			void SetBlurKernelSize(int blur);
			int  GetBlurKernelSize(void) const;

		private:

			bool                     m_initialized;

			int                      m_resolution;
			int                      m_splits;
			bool                     m_exponentialTest;
			int                      m_blurKernelSize;

			DX11ShaderProgramPointer m_vsmGeneration;
			DX11ShaderProgramPointer m_blur[2];

			DX11Texture              m_vsmDepth[2];

			bool __CreateShaders(void);
			bool __CreateRenderTargets(void);
			void __DestroyRenderTargets(void);

		};

	}

}