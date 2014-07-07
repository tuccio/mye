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

			inline int GetHeight(void) const
			{
				return m_height;
			}

			inline int GetWidth(void) const
			{
				return m_width;
			}

		protected:

			int m_width;
			int m_height;

		};

		typedef boost::shared_ptr<Texture> TexturePointer;

	}

}

