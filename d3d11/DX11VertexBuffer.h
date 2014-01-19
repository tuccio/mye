#pragma once

#include "DX11Device.h"
#include "DX11Buffer.h"

#include <mye/core/Mesh.h>

namespace mye
{

	namespace dx11
	{

		class DX11VertexBuffer :
			public DX11Buffer
		{

		public:

			DX11VertexBuffer(mye::core::ResourceManager *owner,
				const std::string &name,
				mye::core::ManualResourceLoader *manual,
				DX11Device &device);

			~DX11VertexBuffer(void);

			bool Create(mye::core::Mesh *mesh);

			void Bind(void);

		protected:

			bool LoadImpl(void);
			void UnloadImpl(void);
			size_t CalculateSizeImpl(void);

		private:

			UINT m_stride;

		};

	}
}
