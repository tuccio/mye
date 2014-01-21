#include "AABB.h"

using namespace mye::core;

AABB::AABB(void) :
	m_min(-0.5f, -0.5f, -0.5f),
	m_max(0.5f, 0.5f, 0.5f)
{
}

AABB::AABB(const mye::math::Vector3f &min,
		   const mye::math::Vector3f &max) :
	m_min(min),
	m_max(max)
{
}

AABB::~AABB(void)
{
}

mye::math::Vector3f AABB::GetCenter(void) const
{
	return (m_max + m_min) * 0.5f;
}

mye::math::Vector3f AABB::GetHalfExtents(void) const
{
	return (m_max - m_min) * 0.5f;
}

mye::math::Vector3f AABB::GetMinimum(void) const
{
	return m_min;
}

mye::math::Vector3f AABB::GetMaximum(void) const
{
	return m_max;
}

std::vector<mye::math::Vector3f> AABB::GetCorners(void) const
{

	std::vector<mye::math::Vector3f> corners(8);

	corners[FAR_LEFT_BOTTOM] = m_min;

	corners[FAR_RIGHT_BOTTOM].x() = m_max.x();
	corners[FAR_RIGHT_BOTTOM].y() = m_min.y();
	corners[FAR_RIGHT_BOTTOM].z() = m_min.z();

	corners[FAR_RIGHT_TOP].x() = m_max.x();
	corners[FAR_RIGHT_TOP].y() = m_max.y();
	corners[FAR_RIGHT_TOP].z() = m_min.z();

	corners[FAR_LEFT_TOP].x() = m_min.x();
	corners[FAR_LEFT_TOP].y() = m_max.y();
	corners[FAR_LEFT_TOP].z() = m_min.z();

	corners[NEAR_LEFT_BOTTOM].x() = m_min.x();
	corners[NEAR_LEFT_BOTTOM].y() = m_min.y();
	corners[NEAR_LEFT_BOTTOM].z() = m_max.z();

	corners[NEAR_RIGHT_BOTTOM].x() = m_max.x();
	corners[NEAR_RIGHT_BOTTOM].y() = m_min.y();
	corners[NEAR_RIGHT_BOTTOM].z() = m_max.z();

	corners[NEAR_RIGHT_TOP] = m_max;

	corners[NEAR_LEFT_TOP].x() = m_min.x();
	corners[NEAR_LEFT_TOP].y() = m_max.y();
	corners[NEAR_LEFT_TOP].z() = m_max.z();

	return corners;

}

AABB AABB::TransformAffine(const mye::math::Matrix4f &t)
{
	
	mye::math::Vector3f center = GetCenter();
	mye::math::Vector3f halfExtents = GetHalfExtents();

	mye::math::Vector3f transformedCenter = 
		t * center;

	mye::math::Vector3f transformedHalfExtents = 
		(t * halfExtents).CwiseAbs();

	return AABB(transformedCenter - transformedHalfExtents,
		transformedCenter + transformedHalfExtents);

}