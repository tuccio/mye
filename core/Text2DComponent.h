#pragma once

#include "AlignedAllocator.h"
#include "Component.h"
#include "Font.h"
#include "GPUBuffer.h"
#include "ManualLambdaLoader.h"
#include "VertexData.h"

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

			Text2DComponent * m_t2dc;

		};

		class Text2DComponent :
			public Component
		{

		public:

			__MYE_DECLARE_ALIGNED_16

			Text2DComponent(void);

			Text2DComponent(const mye::math::Vector2i & position,
							//const mye::math::Vector4 color,
							FontPointer font,
							const String & text);

			~Text2DComponent(void);

			const String & GetText(void) const;
			void SetText(const String & text);

			const mye::math::Vector2i & GetPosition(void) const;
			void SetPosition(const mye::math::Vector2i & position);

			FontPointer GetFont(void) const;
			void SetFont(const FontPointer & font);

			const mye::math::Vector2 & GetPointSize(void) const;
			void SetPointSize(const mye::math::Vector2 & pointSize);

			const mye::math::Vector4 & GetColor(void) const;
			void SetColor(const mye::math::Vector4 & color);

			Text2DComponent * Clone(void) const;

			GPUBufferPointer GetVertexBuffer(void);

			void UpdateText(void);


		private:

			friend class Text2DLoader;			

			String              m_text;
			mye::math::Vector2i m_position;
			mye::math::Vector2  m_pointSize;
			mye::math::Vector4  m_color;

			FontPointer         m_font;

			GPUBufferPointer    m_vb;
			String              m_vbName;
			
			Text2DLoader        m_vbLoader;

		};

	}

}
