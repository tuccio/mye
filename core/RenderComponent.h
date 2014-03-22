#pragma once

#include "Component.h"
#include "Model.h"

#include <mye/math/Geometry.h>

namespace mye
{

	namespace core
	{

		class RenderComponent :
			public Component
		{

		public:

			RenderComponent(void);
			~RenderComponent(void);

			bool IsVisible(void) const;
			void SetVisible(bool visible);

			//VertexDataPointer GetVertexData(void);

			ModelPointer GetModel(void);
			void SetModel(ModelPointer model);

			const mye::math::AABB& GetBounds(void) const;
			void SetBounds(const mye::math::AABB &bounds);

			RenderComponent* Clone(void) const;
			
		private:

			bool m_visible;
			
			//VertexDataPointer m_vertexData;
			ModelPointer m_model;
			
			mye::math::AABB m_bounds;
			
		};

	}

}

