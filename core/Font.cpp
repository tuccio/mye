#include "Font.h"
#include "ResourceTypeManager.h"
#include "FileInputStream.h"

#include <rapidxml.hpp>
#include <rapidxml_print.hpp>
#include <rapidxml_utils.hpp>

#include <regex>

using namespace mye::core;

Font::Font(ResourceManager *owner,
		   const String &name,
		   ManualResourceLoader *manual) :
	Resource(owner, name, manual)
{

	m_texture = ResourceTypeManager::GetSingleton().CreateResource<Texture>("Texture", "./fonts/" + m_name + ".png");

}


Font::~Font(void)
{
}

int Font::GetHeight(void) const
{
	return m_height;
}

int Font::GetSize(void) const
{
	return m_size;
}

TexturePointer Font::GetTexture(void)
{
	return m_texture;
}

const CharacterInfo& Font::GetCharacterInfo(unsigned char c) const
{
	return m_info[c];
}

const CharacterInfo& Font::operator[] (unsigned char c) const
{
	return m_info[c];
}

CharacterInfo& Font::operator[] (unsigned char c)
{
	return m_info[c];
}

bool Font::LoadImpl(void)
{

	memset(m_info, 0, sizeof(CharacterInfo) * 256);

	bool loaded = false;

	if (m_texture->Load())
	{

		int imageWidth  = m_texture->GetImageWidth();
		int imageHeight = m_texture->GetImageHeight();

		String file = "./fonts/" + m_name + ".xml";

		rapidxml::file<> xmlFile(file.CString());

		rapidxml::xml_document<> document;
		document.parse<0>(xmlFile.data());

		auto fontNode = document.first_node("Font");

		if (fontNode)
		{

			auto heightAttr = fontNode->first_attribute("height");

			m_height = ParseType<int>(heightAttr->value());

			for (auto charNode = fontNode->first_node("Char");
				charNode != nullptr;
				charNode = charNode->next_sibling("Char"))
			{

				auto widthAttr  = charNode->first_attribute("width");
				auto offsetAttr = charNode->first_attribute("offset");
				auto rectAttr   = charNode->first_attribute("rect");
				auto codeAttr   = charNode->first_attribute("code");

				if (widthAttr && offsetAttr && rectAttr && codeAttr)
				{

					CharacterInfo info;

					mye::math::Vector2i offset = ParseVector2<int>(offsetAttr->value());
					mye::math::Vector4i rect   = ParseVector4<int>(rectAttr->value());

					info.width   = ParseType<int>(widthAttr->value());
					info.height  = rect.w();

					info.xoffset = offset.x();
					info.yoffset = offset.y();

					info.left    = (float) (rect.x() - 1) / imageWidth;
					info.top     = (float) (rect.y() - 1) / imageHeight;

					info.right   = (float) (rect.x() + rect.z()) / imageWidth;
					info.bottom  = (float) (rect.y() + rect.w()) / imageHeight;

					char *code = codeAttr->value();

					if (code && code[0] && !code[1])
					{
						m_info[code[0]] = info;
					}

				}

			}

			loaded = true;

		}

	}

	return loaded;

}

void Font::UnloadImpl(void)
{

	m_texture->Unload();

}

size_t Font::CalculateSizeImpl(void)
{

	return m_texture->GetSize() + sizeof(CharacterInfo) * 256;

}

void Font::Use(void)
{

}