#pragma once

#include "Model.h"
#include "Mesh.h"

namespace mye
{

	namespace core
	{

		class VertexBuffer
		{

		public:

			VertexBuffer(void);
			~VertexBuffer(void);

			virtual bool Create(void * data, size_t n, const VertexDeclaration & vDecl) = 0;
			virtual bool Create(Mesh * mesh) = 0;
			virtual bool Create(Model * model) = 0;

			virtual void Clear(void) = 0;


		};

		typedef std::shared_ptr<VertexBuffer> VertexBufferPointer;

	}

}

