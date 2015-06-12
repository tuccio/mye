#pragma once

#include "AlignedAllocator.h"

#include <mye/math/Math.h>

namespace mye
{

	namespace core
	{

		// TODO: Use PoolAllocator for Light

		enum class LightType
		{
			DIRECTIONAL,
			POINTLIGHT,
			SPOTLIGHT,
		};

		struct PointLight
		{

			__MYE_DECLARE_ALIGNED_16

			PointLight(void) { }

			PointLight(const mye::math::Vector3 & color,
			           const mye::math::Real intensity,
			           const mye::math::Vector3 & position,
			           mye::math::Real range) :
				color(color),
				intensity(intensity),
				position(position),
				range(range)
			{
			}

			mye::math::Vector3 color;
			mye::math::Real    intensity;
			mye::math::Vector3 position;

			mye::math::Real    range;

		};

		class Light
		{

		public:

			__MYE_DECLARE_ALIGNED_16

			Light(void);

			Light(LightType type,
			      const mye::math::Vector3 & color,
			      const mye::math::Real      intensity,
			      const mye::math::Vector3 & position,
			      const mye::math::Vector3 & direction,
			      mye::math::Real spotAngle,
			      mye::math::Real range);

			Light(const PointLight & pointlight);

			~Light(void);

			LightType GetType(void) const;
			void SetType(LightType type);

			const mye::math::Vector3 & GetColor(void) const;
			void SetColor(const mye::math::Vector3 & color);

			mye::math::Real GetIntensity(void) const;
			void SetIntensity(mye::math::Real intensity);

			const mye::math::Vector3 & GetPosition(void) const;
			void SetPosition(const mye::math::Vector3 & position);

			const mye::math::Vector3 & GetDirection(void) const;
			void SetDirection(const mye::math::Vector3 & direction);

			mye::math::Real GetSpotAngle(void) const;
			void SetSpotAngle(mye::math::Real spotAngle);

			mye::math::Real GetRange(void) const;
			void SetRange(mye::math::Real range);

			virtual mye::math::Matrix4 GetWorldMatrix(void) const
			{
				return mye::math::Matrix4(1);
			}

		private:
			
			LightType m_type;

			mye::math::Vector3 m_color;
			mye::math::Real    m_intensity;

			mye::math::Vector3 m_position;
			mye::math::Vector3 m_direction;

			mye::math::Real    m_spotAngle;
			mye::math::Real    m_range;


		};

	}

}



