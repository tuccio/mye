#pragma once

#include "AlignedAllocator.h"
#include "Component.h"

#include <mye/math/Geometry.h>

namespace mye
{

	namespace core
	{

		class __MYE_ALIGNED(16) TransformComponent :
			public Component
		{

		public:

			__MYE_DECLARE_ALIGNED_16

			TransformComponent(void);

			TransformComponent(const mye::math::Vector3    & position,
			                   const mye::math::Quaternion & orientation,
			                   const mye::math::Vector3    & scale);

			~TransformComponent(void);

			TransformComponent * Clone(void) const;

			inline const mye::math::Vector3 & GetPosition(void) const
			{
				return m_transform.GetPosition();
			}

			inline void SetPosition(const mye::math::Vector3 & position)
			{
				return m_transform.SetPosition(position);
			}

			inline mye::math::Vector3 & Position(void)
			{
				return m_transform.Position();
			}

			inline const mye::math::Vector3 & GetScale(void) const
			{
				return m_transform.GetScale();
			}

			inline void SetScale(const mye::math::Vector3 &scale)
			{
				m_transform.SetScale(scale);
			}

			inline mye::math::Vector3 & Scale(void)
			{
				return m_transform.Scale();
			}

			inline const mye::math::Quaternion & GetOrientation(void) const
			{
				return m_transform.GetOrientation();
			}

			inline void SetOrientation(const mye::math::Quaternion & orientation)
			{
				return m_transform.SetOrientation(orientation);
			}

			inline mye::math::Quaternion & Orientation(void)
			{
				return m_transform.Orientation();
			}

			mye::math::Matrix4 GetWorldMatrix(void);

			inline mye::math::Vector3 Up(void) const
			{
				return m_transform.GetOrientation().Rotate(mye::math::Vector3(0, 1, 0));
			}

			inline mye::math::Vector3 Right(void) const
			{
				return m_transform.GetOrientation().Rotate(mye::math::Vector3(1, 0, 0));
			}

			inline mye::math::Vector3 Forward(void) const
			{
				return m_transform.GetOrientation().Rotate(mye::math::Vector3(0, 0, 1));
			}

		private:

			mye::math::Transform m_transform;

		};

	}
}

