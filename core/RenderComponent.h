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

			mye::core::ModelPointer GetModel(void);
			void SetModel(ModelPointer model);

			const mye::math::AABBf& GetBounds(void) const;
			void SetBounds(const mye::math::AABBf &bounds);

			RenderComponent* Clone(void) const;
			
		private:

			bool m_visible;
			
			ResourceHandle m_vertexData;
			ModelPointer m_model;
			
			mye::math::AABBf m_bounds;
			
		};

	}

}

