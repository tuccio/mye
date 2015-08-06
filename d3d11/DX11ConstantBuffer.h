#pragma once

#include "DX11Device.h"
#include "DX11Buffer.h"
#include "DX11Utils.h"

namespace mye
{

	namespace dx11
	{

		class DX11ConstantBuffer :
			public DX11Buffer
		{

		public:

			DX11ConstantBuffer(void);

			DX11ConstantBuffer(mye::core::ResourceManager     * owner,
			                  const mye::core::String         & name,
			                  mye::core::ManualResourceLoader * manual);

			~DX11ConstantBuffer(void);

			bool Create(size_t size,
				const void *initiationData = nullptr);

			void Bind(DX11PipelineStage stage, int index);

			void GetData(void *data) const;
			void SetData(const void *data);

		protected:

			virtual bool LoadImpl(void);
			virtual void UnloadImpl(void);
			virtual size_t CalculateSizeImpl(void);

		private:

			size_t m_size;

		};

	}

}
