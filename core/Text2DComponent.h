#pragma once

#include "Component.h"
#include "Font.h"
#include "GPUBuffer.h"
#include "ManualLambdaLoader.h"
#include "VertexData.h"
#include "VertexBuffer.h"

namespace mye
{

	namespace core
	{

		class Text2DComponent;

		class Text2DLoader :
			public ManualResourceLoader
		{

		public:

			Text2DLoader(Text2DComponent *t2dc);
			~Text2DLoader(void);

			bool Load(Resource *resource);
			void Unload(Resource *resource);

			

		private:

			Text2DComponent *m_t2dc;

		};

		class Text2DComponent :
			public Component
		{

		public:

			Text2DComponent(void);
			Text2DComponent(const mye::math::Vector2i &position, FontPointer font, const String &text);

			~Text2DComponent(void);

			const String& GetText(void) const;
			void SetText(const String &text);

			const mye::math::Vector2i& GetPosition(void) const;
			void SetPosition(const mye::math::Vector2i &position);

			FontPointer GetFont(void) const;
			void SetFont(const FontPointer &font);

			const mye::math::Vector2f& GetPointSize(void) const;
			void SetPointSize(const mye::math::Vector2f &pointSize);

			const mye::math::Vector4f& GetColor(void) const;
			void SetColor(const mye::math::Vector4f& color);

			Text2DComponent* Clone(void) const;

			VertexBufferPointer GetVertexBuffer(void);

			void UpdateText(void);


		private:

			friend class Text2DLoader;			

			String              m_text;
			mye::math::Vector2i m_position;
			mye::math::Vector2f m_pointSize;
			mye::math::Vector4f m_color;

			FontPointer         m_font;

			GPUBufferPointer    m_vb;
			String              m_vbName;
			
			Text2DLoader        m_vbLoader;

		};

	}

}
