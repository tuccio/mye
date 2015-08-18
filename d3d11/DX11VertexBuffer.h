#pragma once

#include "DX11Device.h"
#include "DX11Buffer.h"

#include <mye/core/VertexDeclaration.h>
#include <mye/core/Mesh.h>
#include <mye/core/Model.h>

namespace mye
{

	namespace dx11
	{

		class DX11VertexBuffer :
			public DX11Buffer
		{

		public:

			DX11VertexBuffer(void);

			DX11VertexBuffer(mye::core::ResourceManager      * owner,
			                 const mye::core::String         & name,
			                 mye::core::ManualResourceLoader * manual);

			~DX11VertexBuffer(void);

			bool Create(void * data, size_t n, const mye::core::VertexDeclaration & vDecl);

			bool Create(mye::core::Mesh  * mesh);
			bool Create(mye::core::Model * model);

			void Clear(void);

			void Bind(unsigned int slot = 0);
			void Unbind(void);

			size_t GetVerticesCount(void) const;

		protected:

			bool   LoadImpl(void);
			void   UnloadImpl(void);
			size_t CalculateSizeImpl(void);

		private:

			size_t m_stride;
			size_t m_vertices;

		};

		typedef std::shared_ptr<DX11VertexBuffer> DX11VertexBufferPointer;

	}
}
