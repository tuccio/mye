#pragma once

#include "DX11Device.h"

#include <mye/core/Resource.h>

#include <d3d11.h>
#include <Effects11\d3dx11effect.h>
#include <string>

namespace mye
{

	namespace dx11
	{

		class DX11Shader :
			public mye::core::Resource
		{

		public:

			DX11Shader(mye::core::ResourceManager *owner,
				const std::string &name,
				mye::core::ManualResourceLoader *manual = nullptr);

			~DX11Shader(void);

			virtual void Use(void);

			virtual void Destroy(void);

		protected:

			virtual bool LoadImpl(void);
			virtual void UnloadImpl(void);

			virtual size_t CalculateSizeImpl(void);

			std::string m_source;

		};

	}

}
