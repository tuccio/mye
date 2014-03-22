#pragma once

namespace mye
{

	namespace math
	{

		enum class VolumeType
		{
			AABBt,
			FRUSTUM
		};

		enum class VolumeSide
		{
			INSIDE,
			INTERSECT,
			OUTSIDE
		};

		template <typename T>
		class Volume
		{
			
		public:

			Volume(VolumeType type) :
				m_volumeType(type)
			{
			}

			virtual Volume* Clone(void) const = 0;

			virtual VolumeSide Intersect(const Volume &volume) const;

			virtual void TransformAffine(Volume &volume,
				const Matrix<T, 4, 4> &Transformt) const = 0;
			
			inline VolumeType GetVolumeType(void) const
			{
				return m_volumeType;
			}

		protected:

			VolumeType m_volumeType;

		};

	}

}