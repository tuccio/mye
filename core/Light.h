#pragma once

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

			PointLight(const mye::math::Vector3 &color,
			           const mye::math::Vector3 &intensity,
			           const mye::math::Vector3 &position,
			           mye::math::Real range) :
				color(color),
				intensity(intensity),
				position(position),
				range(range)
			{
			}

			mye::math::Vector3 color;
			mye::math::Vector3 intensity;
			mye::math::Vector3 position;

			mye::math::Real range;

		};

		class Light
		{

		public:

			Light(void);

			Light(
				LightType type,
				const mye::math::Vector3 &color,
				const mye::math::Vector3 &intensity,
				const mye::math::Vector3 &position,
				const mye::math::Vector3 &direction,
				mye::math::Real spotAngles,
				mye::math::Real range);

			Light(const PointLight &pointlight);

			~Light(void);

			LightType GetType(void) const;
			void SetType(LightType type);

			const mye::math::Vector3& GetColor(void) const;
			void SetColor(const mye::math::Vector3 &color);

			const mye::math::Vector3& GetIntensity(void) const;
			void SetIntensity(const mye::math::Vector3 &intensity);

			const mye::math::Vector3& GetPosition(void) const;
			void SetPosition(const mye::math::Vector3 &position);

			const mye::math::Vector3& GetDirection(void) const;
			void SetDirection(const mye::math::Vector3 &direction);

			mye::math::Real GetSpotAngle(void) const;
			void SetSpotAngle(mye::math::Real spotAngle);

			mye::math::Real GetRange(void) const;
			void SetRange(mye::math::Real range);

		private:
			
			LightType m_type;

			mye::math::Vector3 m_color;
			mye::math::Vector3 m_intensity;

			mye::math::Vector3 m_position;
			mye::math::Vector3 m_direction;

			mye::math::Real    m_spotAngle;
			mye::math::Real    m_range;


		};

	}

}



