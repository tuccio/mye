#pragma once

#include "DX11Device.h"

#include <mye/core/Resource.h>

#include <vector>

namespace mye
{

	namespace dx11
	{

		class DX11Shader :
			public mye::core::Resource
		{

		public:

			DX11Shader(mye::core::ResourceManager      * owner,
			           const mye::core::String         & name,
			           mye::core::ManualResourceLoader * manual      = nullptr,
			           bool                              precompiled = false);

			~DX11Shader(void);

			virtual void Use(void) = 0;
			virtual void Dispose(void) = 0;

			virtual void Destroy(void);

		protected:

			virtual bool LoadImpl(void);
			virtual void UnloadImpl(void);

			virtual size_t CalculateSizeImpl(void);

			std::vector<D3D_SHADER_MACRO> CreateDefinesVector(void) const;
			void FreeDefinesVector(std::vector<D3D_SHADER_MACRO> & defines) const;

			mye::core::String m_source;
			bool              m_precompiled;

		};

		typedef std::shared_ptr<DX11Shader> DX11ShaderPointer;

	}

}
