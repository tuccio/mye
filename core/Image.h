#pragma once

#include "Resource.h"

#include <IL/il.h>
#include <mye/math/Math.h>

namespace mye
{

	namespace core
	{

		class Image :
			public mye::core::Resource
		{

		public:

			Image(ResourceManager * owner = nullptr,
			      const String & name = "",
			      ManualResourceLoader * manual = nullptr);

			Image(const Image & image);
			Image(Image && image);

			~Image(void);

			mye::math::Vector4f operator() (unsigned int width, unsigned int height);

			int GetWidth(void) const;
			int GetHeight(void) const;

			const void * GetData(void) const;

			void Destroy(void);

		protected:

			bool LoadImpl(void);
			void UnloadImpl(void);

			size_t CalculateSizeImpl(void);

		private:

			ILuint m_id;

		};

		typedef std::shared_ptr<Image> ImagePointer;

	}

}

#include "Image.inl"