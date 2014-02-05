#pragma once

#include "Component.h"
#include "Model.h"
#include "ResourceHandle.h"

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

			mye::core::ResourceHandle GetVertexData(void);

			mye::core::ResourceHandle GetModel(void);
			void SetModel(ResourceHandle model);

			const mye::math::AABB& GetBounds(void) const;
			void SetBounds(const mye::math::AABB &bounds);

			RenderComponent* Clone(void) const;
			
		private:

			bool m_visible;
			
			ResourceHandle m_vertexData;
			ResourceHandle m_model;
			
			mye::math::AABB m_bounds;
			
		};

	}

}

