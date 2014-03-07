#include "RenderComponent.h"

using namespace mye::core;
using namespace mye::math;

RenderComponent::RenderComponent(void) :
	Component(ComponentTypes::RENDER, "render"),
	m_visible(true),
	m_bounds(AABBf::FromMinMax(Vector3f(0), Vector3f(0)))
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

mye::core::ResourceHandle RenderComponent::GetVertexData(void)
{
	return m_vertexData;
}

mye::core::ModelPointer RenderComponent::GetModel(void)
{
	return m_model;
}

void RenderComponent::SetModel(ModelPointer model)
{
	m_model = model;
}

const mye::math::AABBf& RenderComponent::GetBounds(void) const
{
	return m_bounds;
}

void RenderComponent::SetBounds(const mye::math::AABBf &bounds)
{
	m_bounds = bounds;
}

RenderComponent* RenderComponent::Clone(void) const
{
	return new RenderComponent(*this);
}