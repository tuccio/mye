#include "RenderComponent.h"

using namespace mye::core;
using namespace mye::math;

RenderComponent::RenderComponent(void) :
	Component(ComponentTypes::RENDER, "render"),
	m_visible(false),
	m_bounds(AABB::FromMinMax(Vector3(0), Vector3(0))),
	m_modelMatrix(1)
{
}

RenderComponent::RenderComponent(MeshPointer mesh, MaterialPointer material, const Matrix4 & modelMatrix) :
	Component(ComponentTypes::RENDER, "render"),
	m_visible(true),
	m_material(material),
	m_modelMatrix(modelMatrix)
{

	SetMesh(mesh);
	
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

mye::core::MeshPointer RenderComponent::GetMesh(void)
{
	return m_mesh;
}

void RenderComponent::SetMesh(MeshPointer mesh)
{

	m_mesh = mesh;

	if (m_mesh && m_mesh->Load())
	{
		auto minmax = mesh->GetMinMaxVertices();
		m_bounds = AABB::FromMinMax(minmax.first, minmax.second);
	}
	else
	{
		m_bounds = AABB::FromMinMax(Vector3(0), Vector3(0));
	}

}

const mye::math::AABB& RenderComponent::GetBounds(void) const
{
	return m_bounds;
}

void RenderComponent::SetBounds(const mye::math::AABB &bounds)
{
	m_bounds = bounds;
}

MaterialPointer RenderComponent::GetMaterial(void) const
{
	return m_material;
}

void RenderComponent::SetMaterial(const MaterialPointer & material)
{
	m_material = material;
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