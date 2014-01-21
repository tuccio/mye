#pragma once

#include <mye/math/Math.h>


namespace mye
{

	namespace core
	{

		class Transform
		{

		public:

			static const Transform& Identity(void);

			Transform(void);
			Transform(const mye::math::Vector3f &scale,
				const mye::math::Quaternionf &rotation,
				const mye::math::Vector3f &translation);

			~Transform(void);

			mye::math::Matrix4f GetMatrix(void) const;

			Transform Combine(const Transform &transform) const;

			const mye::math::Vector3f& GetScale(void) const;
			void SetScale(const mye::math::Vector3f &scale);

			const mye::math::Quaternionf& GetRotation(void) const;
			void SetRotation(const mye::math::Quaternionf &rotation);

			const mye::math::Vector3f& GetTranslation(void) const;
			void SetTranslation(const mye::math::Vector3f &translation);

		private:

			static const Transform IDENTITY;

			mye::math::Quaternionf m_rotation;
			mye::math::Vector3f m_scale;
			mye::math::Vector3f m_translation;

		};

	}

}
