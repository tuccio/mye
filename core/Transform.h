#pragma once

#include <Eigen/Eigen>


namespace mye
{

	namespace core
	{

		class __declspec(align(16)) Transform
		{

		public:

			static const Transform& Identity(void);

			Transform(void);
			Transform(const Eigen::Vector3f &scale,
				const Eigen::Quaternionf &rotation,
				const Eigen::Vector3f &translation);

			~Transform(void);

			Eigen::Matrix4f GetMatrix(void) const;

			Transform Combine(const Transform &transform) const;

			const Eigen::Vector3f& GetScale(void) const;
			void SetScale(const Eigen::Vector3f &scale);

			const Eigen::Quaternionf& GetRotation(void) const;
			void SetRotation(const Eigen::Quaternionf &rotation);

			const Eigen::Vector3f& GetTranslation(void) const;
			void SetTranslation(const Eigen::Vector3f &translation);

			EIGEN_MAKE_ALIGNED_OPERATOR_NEW

		private:

			static const Transform IDENTITY;

			Eigen::Quaternionf m_rotation;
			Eigen::Vector3f m_scale;
			Eigen::Vector3f m_translation;

		};

	}

}
