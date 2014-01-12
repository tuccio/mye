#include "SceneGraphNode.h"

using namespace mye::core;

SceneGraphNode::SceneGraphNode(void) :
	m_type(SGNODE_NONE)
{
}


SceneGraphNode::~SceneGraphNode(void)
{
}

SceneGraphNode::Type SceneGraphNode::GetType(void) const
{
	return m_type;
}

int SceneGraphNode::GetParentsCount(void) const
{
	return m_parents.size();
}

SceneGraphNode::ParentsList SceneGraphNode::GetParentsList(void)
{
	return m_parents;
}

GroupGraphNode* SceneGraphNode::CastToGroup(void)
{

	if (m_type == SGNODE_GROUP)
	{
		return static_cast<GroupGraphNode*>(this);
	}

	return NULL;

}

WorldGraphNode* SceneGraphNode::CastToWorld(void)
{

	if (m_type == SGNODE_WORLD)
	{
		return static_cast<WorldGraphNode*>(this);
	}

	return NULL;

}

/* Group Node */

GroupGraphNode::GroupGraphNode(void)
{
	m_type = SGNODE_GROUP;
}

void GroupGraphNode::AddChildren(SceneGraphNode *node)
{

	if (node->GetType() != SGNODE_WORLD)
	{
		m_children.push_back(node);
		node->m_parents.push_back(this);
	}
	
}

SceneGraphNode* GroupGraphNode::RemoveChildren(int i)
{

	ChildrenList::iterator it = m_children.begin() + i;

	if (it != m_children.end())
	{
		
		SceneGraphNode *node = *it;

		ParentsList::iterator myself = std::find(
			node->m_parents.begin(),
			node->m_parents.end(),
			this);

		node->m_parents.erase(myself);
		m_children.erase(it);

		return node;

	}

	return NULL;
	
}

void GroupGraphNode::InsertChildren(int i, SceneGraphNode *node)
{

	if (node->GetType() != SGNODE_WORLD)
	{
		m_children.insert(m_children.begin() + i, node);
		node->m_parents.push_back(this);
	}

}

SceneGraphNode* GroupGraphNode::GetChildren(int i)
{
	return m_children[i];
}

int GroupGraphNode::GetChildrenCount(void) const
{
	return m_children.size();
}

const GroupGraphNode::ChildrenList& GroupGraphNode::GetChildrenList(void)
{
	return m_children;
}

/* World */

WorldGraphNode::WorldGraphNode(void)
{
	m_type = SGNODE_WORLD;
}

/* Transform */

TransformGraphNode::TransformGraphNode(void)
{
	m_type = SGNODE_TRANSFORM;
}

Transform TransformGraphNode::GetTransform(void) const
{
	return m_transform;
}

void TransformGraphNode::SetTransform(const Transform &transform)
{
	m_transform = transform;
}