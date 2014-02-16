#pragma once

#include "Component.h"
#include <mye/math/Geometry.h>

namespace mye
{

	namespace core
	{

		class TransformComponent :
			public Component
		{

		public:

			TransformComponent(void);
			~TransformComponent(void);

			TransformComponent* Clone(void) const;

			const mye::math::Vector3f& GetPosition(void) const;
			void SetPosition(const mye::math::Vector3f &position);

			const mye::math::Vector3f& GetScale(void) const;
			void SetScale(const mye::math::Vector3f &scale);

			const mye::math::Quaternionf& GetOrientation(void) const;
			void SetOrientation(const mye::math::Quaternionf &orientation);

			const mye::math::Matrix4f& GetWorldMatrix(void);

			mye::math::Vector3f Up(void) const;
			mye::math::Vector3f Right(void) const;
			mye::math::Vector3f Forward(void) const;

		private:

			mye::math::Transformf m_transform;
			mye::math::Matrix4f m_matrix;

		};

	}
}

