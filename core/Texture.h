#pragma once

#include "Resource.h"

namespace mye
{

	namespace core
	{

		class Texture :
			public Resource
		{

		public:

			Texture(ResourceManager *owner,
				const String &name,
				ManualResourceLoader *manual);

			~Texture(void);

			inline int GetImageHeight(void) const
			{
				return m_imageHeight;
			}

			inline int GetImageWidth(void) const
			{
				return m_imageWidth;
			}

		protected:

			int m_imageWidth;
			int m_imageHeight;

		};

		typedef boost::shared_ptr<Texture> TexturePointer;

	}

}

