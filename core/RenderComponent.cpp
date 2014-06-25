#include "RenderComponent.h"

using namespace mye::core;
using namespace mye::math;

RenderComponent::RenderComponent(void) :
	Component(ComponentTypes::RENDER, "render"),
	m_visible(true),
	m_bounds(AABB::FromMinMax(Vector3(0), Vector3(0))),
	m_modelMatrix(1)
{
}


RenderComponent::~RenderComponent(void)
{
}

bool RenderComponent::IsVisible(void) const
{
	return m_visible;
}

void RenderComponent::SetVisible(bool val)
{
	m_visible = val;
}

/*VertexDataPointer RenderComponent::GetVertexData(void)
{
	return m_vertexData;
}*/

mye::core::ModelPointer RenderComponent::GetModel(void)
{
	return m_model;
}

void RenderComponent::SetModel(ModelPointer model)
{
	m_model = model;
}

const mye::math::AABB& RenderComponent::GetBounds(void) const
{
	return m_bounds;
}

void RenderComponent::SetBounds(const mye::math::AABB &bounds)
{
	m_bounds = bounds;
}

RenderComponent* RenderComponent::Clone(void) const
{
	return new RenderComponent(*this);
}

const mye::math::Matrix4f& RenderComponent::GetModelMatrix(void) const
{
	return m_modelMatrix;
}

void RenderComponent::SetModelMatrix(const mye::math::Matrix4f &matrix)
{
	m_modelMatrix = matrix;
}