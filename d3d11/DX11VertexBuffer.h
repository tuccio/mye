#pragma once

#include "DX11Device.h"
#include "DX11Buffer.h"

#include <mye/core/VertexBuffer.h>

namespace mye
{

	namespace dx11
	{

		class DX11VertexBuffer :
			public DX11Buffer,
			public mye::core::VertexBuffer
		{

		public:

			DX11VertexBuffer(mye::core::ResourceManager *owner,
				const mye::core::String &name,
				mye::core::ManualResourceLoader *manual,
				DX11Device &device);

			~DX11VertexBuffer(void);

			bool Create(void *data, size_t n, const mye::core::VertexDeclaration &vDecl);
			bool Create(mye::core::Mesh *mesh);
			bool Create(mye::core::Model *model);

			void Clear(void);

			void Bind(void);
			void Unbind(void);

			size_t GetVerticesCount(void) const;

		protected:

			bool LoadImpl(void);
			void UnloadImpl(void);
			size_t CalculateSizeImpl(void);

		private:

			UINT m_stride;
			size_t m_vertices;

		};

		typedef boost::shared_ptr<DX11VertexBuffer> DX11VertexBufferPointer;

	}
}
