#pragma once

#include "Resource.h"
#include "Image.h"
#include "Texture.h"

#include <mye/math/Vector2.h>

#include <map>
#include <unordered_map>

namespace mye
{

	namespace core
	{

		struct CharacterInfo
		{

			int xoffset;
			int yoffset;

			int width;
			int height;

			float top;
			float left;

			float bottom;
			float right;

		};

		class Font :
			public Resource
		{

		public:

			Font(ResourceManager *owner,
				const String &name,
				ManualResourceLoader *manual);

			~Font(void);

			int GetHeight(void) const;
			int GetSize(void) const;

			TexturePointer GetTexture(void);
			
			const CharacterInfo& GetCharacterInfo(unsigned char c) const;

			const CharacterInfo& operator[] (unsigned char c) const;
			CharacterInfo& operator[] (unsigned char c);

			virtual void Bind(void);
			virtual void Unbind(void);

		protected:

			TexturePointer m_texture;
			CharacterInfo m_info[256];

			int m_height;
			int m_size;

			bool LoadImpl(void);
			void UnloadImpl(void);

			size_t CalculateSizeImpl(void);

		};

		typedef boost::shared_ptr<Font> FontPointer;

	}

}
