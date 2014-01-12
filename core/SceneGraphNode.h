#pragma once

#include <vector>

#include "Transform.h"

namespace mye
{

	namespace core
	{

		class GroupGraphNode;
		class WorldGraphNode;
		class TransformGraphNode;
		class GeometryGraphNode;
		class TerrainGraphNode;
		class GameObjectGraphNode;

		class SceneGraphNode
		{

		public:

			enum Type
			{
				SGNODE_NONE,
				SGNODE_GROUP,
				SGNODE_WORLD,
				SGNODE_TRANSFORM,
				SGNODE_GEOMETRY,
				SGNODE_TERRAIN,
				SGNODE_GAMEOBJECT
			};

			typedef std::vector<GroupGraphNode*> ParentsList;

			SceneGraphNode(void);
			virtual ~SceneGraphNode(void) = 0;

			Type GetType(void) const;

			int GetParentsCount(void) const;
			ParentsList GetParentsList(void);

			GroupGraphNode* CastToGroup(void);
			WorldGraphNode* CastToWorld(void);

		protected:

			friend class GroupGraphNode;

			Type m_type;
			ParentsList m_parents;

		};

		class GroupGraphNode :
			public SceneGraphNode
		{

		public:

			typedef std::vector<SceneGraphNode*> ChildrenList;

			GroupGraphNode(void);

			void AddChildren(SceneGraphNode *node);
			SceneGraphNode* RemoveChildren(int i);

			SceneGraphNode* GetChildren(int i);
			void InsertChildren(int i, SceneGraphNode *node);

			int GetChildrenCount(void) const;
			const ChildrenList& GetChildrenList(void);

		private:

			ChildrenList m_children;

		};

		class WorldGraphNode :
			public GroupGraphNode
		{

		public:

			WorldGraphNode(void);

		};

		class TransformGraphNode :
			public GroupGraphNode
		{

		public:

			TransformGraphNode(void);

			Transform GetTransform(void) const;
			void SetTransform(const Transform &transform);

		private:

			Transform m_transform;

		};

	}

}
