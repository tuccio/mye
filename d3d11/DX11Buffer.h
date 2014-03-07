#pragma once

#include "DX11Device.h"

#include <mye/core/Resource.h>

namespace mye
{

	namespace dx11
	{

		class DX11Buffer :
			public mye::core::Resource
		{

		public:

			DX11Buffer(mye::core::ResourceManager *owner,
				const mye::core::String &name,
				mye::core::ManualResourceLoader *manual,
				DX11Device &device);

			~DX11Buffer(void);

			virtual void Destroy(void);

			ID3D11Buffer* GetBuffer(void);

		protected:

			DX11Device &m_device;
			ID3D11Buffer *m_buffer;

		};

		typedef boost::shared_ptr<DX11Buffer> DX11BufferPointer;

	}
}

