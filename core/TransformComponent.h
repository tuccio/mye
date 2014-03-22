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

			const mye::math::Vector3& GetPosition(void) const;
			void SetPosition(const mye::math::Vector3 &position);

			const mye::math::Vector3& GetScale(void) const;
			void SetScale(const mye::math::Vector3 &scale);

			const mye::math::Quaternion& GetOrientation(void) const;
			void SetOrientation(const mye::math::Quaternion &orientation);

			const mye::math::Matrix4& GetWorldMatrix(void);

			mye::math::Vector3 Up(void) const;
			mye::math::Vector3 Right(void) const;
			mye::math::Vector3 Forward(void) const;

			void Preupdate(void);
			bool Postupdate(mye::math::Matrix4 &oldTransform);

		private:

			mye::math::Transform m_transform;
			mye::math::Matrix4   m_matrix;

			bool m_changed;

		};

	}
}

