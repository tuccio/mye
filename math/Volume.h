#pragma once

namespace mye
{

	namespace math
	{

		enum class VolumeType
		{
			AABB,
			FRUSTUM
		};

		enum class VolumeSide
		{
			INSIDE,
			INTERSECT,
			OUTSIDE
		};

		class Volume
		{
			
		public:

			Volume(VolumeType type) :
				m_volumeType(type)
			{
			}
			
			inline VolumeType GetVolumeType(void) const
			{
				return m_volumeType;
			}

		protected:

			VolumeType m_volumeType;

		};

	}

}