#include "Text2DComponent.h"

#include "VertexBuffer.h"
#include "VertexData.h"
#include "ResourceTypeManager.h"
#include "Game.h"

#include <mye/core/Utils.h>

using namespace mye::core;
using namespace mye::math;

#define __MYE_TEXT2D_BUFFER_NAME ("__MYE_TEXT2D_BUFFER_" + PointerToString(this))

Text2DComponent::Text2DComponent(void) :
	Component(ComponentTypes::TEXT2D, "text2d"),
	m_vbLoader(this),
	m_pointSize(1.0f / 800.0f, 1.0f / 600.0f),
	m_color(1, 1, 1, 1)
{

	Parameters params;
	params["type"] = "vertex";

	m_vb = ResourceTypeManager::GetSingleton().CreateResource<GPUBuffer>("GPUBuffer", __MYE_TEXT2D_BUFFER_NAME, &m_vbLoader, params);

}

Text2DComponent::Text2DComponent(const Vector2i &position, FontPointer font, const String &text) :
	Component(ComponentTypes::TEXT2D, "text2d"),
	m_position(position),
	m_font(font),
	m_text(text),
	m_vbLoader(this),
	m_pointSize(1.0f / 800.0f, 1.0f / 600.0f),
	m_color(1, 1, 1, 1)
{

	Parameters params;
	params["type"] = "vertex";

	m_vb = ResourceTypeManager::GetSingleton().CreateResource<GPUBuffer>("GPUBuffer", __MYE_TEXT2D_BUFFER_NAME, &m_vbLoader, params);

}


Text2DComponent::~Text2DComponent(void)
{

}

const String& Text2DComponent::GetText(void) const
{
	return m_text;
}

void Text2DComponent::SetText(const String &text)
{
	m_text    = text;
	m_vb->Unload();
}

const Vector2i& Text2DComponent::GetPosition(void) const
{
	return m_position;
}

void Text2DComponent::SetPosition(const Vector2i &position)
{
	m_position = position;
	m_vb->Unload();
}

FontPointer Text2DComponent::GetFont(void) const
{
	return m_font;
}

void Text2DComponent::SetFont(const FontPointer &font)
{
	m_font    = font;
	m_vb->Unload();
}

const mye::math::Vector2f& Text2DComponent::GetPointSize(void) const
{
	return m_pointSize;
}

void Text2DComponent::SetPointSize(const mye::math::Vector2f &pointSize)
{
	m_pointSize = pointSize;
	m_vb->Unload();
}

const mye::math::Vector4f& Text2DComponent::GetColor(void) const
{
	return m_color;
}

void Text2DComponent::SetColor(const mye::math::Vector4f& color)
{
	m_color = color;
}

VertexBufferPointer Text2DComponent::GetVertexBuffer(void)
{
	return Resource::DynamicCast<VertexBuffer>(m_vb);
}

Text2DComponent* Text2DComponent::Clone(void) const
{

	return new Text2DComponent(m_position, m_font, m_text);

}

void Text2DComponent::UpdateText(void)
{

	int n = 6 * m_text.Length();

	VertexDeclaration vDecl;
	vDecl.AddAttribute(VertexAttributeSemantic::POSITION, VertexAttributeType::FLOAT2);
	vDecl.AddAttribute(VertexAttributeSemantic::TEXCOORD0, VertexAttributeType::FLOAT2);

	VertexData data(vDecl);

	data.Allocate(n);

	int i = 0;

	float x = m_position.x() * m_pointSize.x() - 1;
	float y = 1 - m_position.y() * m_pointSize.y();	

	for (unsigned char c : m_text)
	{

		const CharacterInfo& info = m_font->GetCharacterInfo(c);

		float bottom = y - (info.height + info.yoffset) * m_pointSize.y();

		Vector2f offset(info.xoffset * m_pointSize.x(), info.yoffset * m_pointSize.y());
		//Vector2f offset(0, 0);

		Vector2f p1(x + offset.x(), y - offset.y());
		Vector2f p2(p1.x(), bottom);

		x += (info.width + info.xoffset) * m_pointSize.x();

		Vector2f p3(x, p1.y());
		Vector2f p4(x, bottom);

		Vector2f t1(info.left,  info.top);
		Vector2f t2(info.left,  info.bottom);
		Vector2f t3(info.right, info.top);
		Vector2f t4(info.right, info.bottom);

		data.SetVertexAttribute(i,     VertexAttributeSemantic::POSITION,  VertexAttributeType::FLOAT2, &p1);
		data.SetVertexAttribute(i,     VertexAttributeSemantic::TEXCOORD0, VertexAttributeType::FLOAT2, &t1);

		data.SetVertexAttribute(i + 1, VertexAttributeSemantic::POSITION,  VertexAttributeType::FLOAT2, &p2);
		data.SetVertexAttribute(i + 1, VertexAttributeSemantic::TEXCOORD0, VertexAttributeType::FLOAT2, &t2);

		data.SetVertexAttribute(i + 2, VertexAttributeSemantic::POSITION,  VertexAttributeType::FLOAT2, &p3);
		data.SetVertexAttribute(i + 2, VertexAttributeSemantic::TEXCOORD0, VertexAttributeType::FLOAT2, &t3);

		data.SetVertexAttribute(i + 3, VertexAttributeSemantic::POSITION,  VertexAttributeType::FLOAT2, &p3);
		data.SetVertexAttribute(i + 3, VertexAttributeSemantic::TEXCOORD0, VertexAttributeType::FLOAT2, &t3);

		data.SetVertexAttribute(i + 4, VertexAttributeSemantic::POSITION,  VertexAttributeType::FLOAT2, &p2);
		data.SetVertexAttribute(i + 4, VertexAttributeSemantic::TEXCOORD0, VertexAttributeType::FLOAT2, &t2);

		data.SetVertexAttribute(i + 5, VertexAttributeSemantic::POSITION,  VertexAttributeType::FLOAT2, &p4);
		data.SetVertexAttribute(i + 5, VertexAttributeSemantic::TEXCOORD0, VertexAttributeType::FLOAT2, &t4);

		i += 6;

	}

	VertexBuffer *vb = dynamic_cast<VertexBuffer*>(m_vb.get());
	vb->Create(data.GetData(), n, vDecl);

}

Text2DLoader::Text2DLoader(Text2DComponent *t2dc) :
	m_t2dc(t2dc)
{

}

Text2DLoader::~Text2DLoader(void)
{
}

bool Text2DLoader::Load(Resource *resource)
{

	bool loaded = false;

	if (m_t2dc->m_font && m_t2dc->m_font->Load())
	{

		m_t2dc->UpdateText();
		loaded = true;

	}

	return loaded;

}

void Text2DLoader::Unload(Resource *resource)
{

	VertexBuffer *vb = dynamic_cast<VertexBuffer*>(m_t2dc->m_vb.get());
	vb->Clear();

}