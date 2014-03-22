namespace mye
{

	namespace core
	{

		/* Loose Octree Node */

		MYE_DEFINE_TEMPLATE_POOL_ALLOCATOR(LooseOctreeNode, typename T, typename BoundingVolume)

		template <typename T, typename BoundingVolume>
		LooseOctreeNode<T, BoundingVolume>::LooseOctreeNode(LooseOctreeNode<T, BoundingVolume> *parent,
			const mye::math::AABB& bounds) :
			m_parent(parent),
			m_bounds(bounds)
		{
			memset(m_children, 0, sizeof(void*) * 8);
		}
		
		template <typename T, typename BoundingVolume>
		LooseOctreeNode<T, BoundingVolume>::~LooseOctreeNode(void)
		{
			Clear();
		}

		template <typename T, typename BoundingVolume>
		void LooseOctreeNode<T, BoundingVolume>::Clear(void)
		{

			if (m_children[0])
			{
				delete m_children[0];
			}

			if (m_children[1])
			{
				delete m_children[1];
			}

			if (m_children[2])
			{
				delete m_children[2];
			}

			if (m_children[3])
			{
				delete m_children[3];
			}

			if (m_children[4])
			{
				delete m_children[4];
			}

			if (m_children[5])
			{
				delete m_children[5];
			}

			if (m_children[6])
			{
				delete m_children[6];
			}

			if (m_children[7])
			{
				delete m_children[7];
			}

			memset(m_children, 0, sizeof(void*) * 8);
			m_objects.clear();

		}

		template <typename T, typename BoundingVolume>
		mye::math::AABB LooseOctreeNode<T, BoundingVolume>::GetBounds(void) const
		{
			return m_bounds;
		}

		template <typename T, typename BoundingVolume>
		LooseOctreeNode<T, BoundingVolume>* LooseOctreeNode<T, BoundingVolume>::GetChild(OctreeChild child)
		{
			return m_children[static_cast<int>(child)];
		}
		
		template <typename T, typename BoundingVolume>
		unsigned int LooseOctreeNode<T, BoundingVolume>::GetObjectsCount(void) const
		{
			return m_objects.size();
		}

		template <typename T, typename BoundingVolume>
		void LooseOctreeNode<T, BoundingVolume>::AddObject(const T &object)
		{
			m_objects.push_back(object);
		}

		template <typename T, typename BoundingVolume>
		typename LooseOctreeNode<T, BoundingVolume>::Iterator LooseOctreeNode<T, BoundingVolume>::FindObject(const T &object)
		{
			return Iterator(std::find(m_objects.begin(), m_objects.end(), object));
		}

		template <typename T, typename BoundingVolume>
		typename LooseOctreeNode<T, BoundingVolume>::Iterator LooseOctreeNode<T, BoundingVolume>::RemoveObject(const Iterator &it)
		{
			return Iterator(m_objects.erase(it.m_it));
		}

		template <typename T, typename BoundingVolume>
		typename LooseOctreeNode<T, BoundingVolume>::Iterator LooseOctreeNode<T, BoundingVolume>::begin(void)
		{
			return Iterator(m_objects.begin());
		}

		template <typename T, typename BoundingVolume>
		typename LooseOctreeNode<T, BoundingVolume>::Iterator LooseOctreeNode<T, BoundingVolume>::end(void)
		{
			return Iterator(m_objects.end());
		}

		/* Loose Octree Node Iterator */

		template <typename T, typename BoundingVolume>
		LooseOctreeNode<T, BoundingVolume>::Iterator::Iterator(const Iterator &it) :
			m_it(it.m_it)
		{
		}

		template <typename T, typename BoundingVolume>
		LooseOctreeNode<T, BoundingVolume>::Iterator::Iterator(typename const std::list<T>::iterator &it) :
			m_it(it)
		{
		}

		template <typename T, typename BoundingVolume>
		T& LooseOctreeNode<T, BoundingVolume>::Iterator::operator* (void) const
		{
			return *m_it;
		}

		template <typename T, typename BoundingVolume>
		bool LooseOctreeNode<T, BoundingVolume>::Iterator::operator!= (const Iterator &it) const
		{
			return m_it != it.m_it;
		}

		template <typename T, typename BoundingVolume>
		typename LooseOctreeNode<T, BoundingVolume>::Iterator& LooseOctreeNode<T, BoundingVolume>::Iterator::operator++ (void)
		{
			m_it++;
			return *this;
		}

		/* Loose Octree */

		template <typename T, typename BoundingVolume>
		LooseOctree<T, BoundingVolume>::LooseOctree(mye::math::Vector3 center,
			float size,
			unsigned int maxdepth,
			unsigned int looseness) :
		m_root(nullptr, mye::math::AABB::FromCenterHalfExtents(center, mye::math::Vector3(size * 0.5f * looseness))),
			m_bounds(mye::math::AABB::FromCenterHalfExtents(center, mye::math::Vector3(size * 0.5f))),
			m_maxdepth(maxdepth),
			m_looseness(looseness)
		{
		}

		template <typename T, typename BoundingVolume>
		LooseOctree<T, BoundingVolume>::~LooseOctree(void)
		{
			m_root.Clear();
		}

		template <typename T, typename BoundingVolume>
		bool LooseOctree<T, BoundingVolume>::Insert(const T &object, const BoundingVolume &boundingVolume)
		{

			LooseOctreeNode<T, BoundingVolume> *node;
			OctreeChild child;
			unsigned int depth;
			bool needToAllocate;

			bool found = Find(boundingVolume, &node, &child, &depth, &needToAllocate);

			if (!found && !needToAllocate)
			{
				return false;
			}

			if (needToAllocate)
			{
				AllocateChildren(boundingVolume, &node, &child, &depth);
			}

			node->AddObject(object);

			return true;

		}

		template <typename T, typename BoundingVolume>
		LooseOctreeNode<T, BoundingVolume>* LooseOctree<T, BoundingVolume>::Find(const BoundingVolume &boundingVolume)
		{

			LooseOctreeNode<T, BoundingVolume> *node;
			unsigned int depth = 0;
			OctreeChild child;
			bool needToAllocate;

			return Find(boundingVolume, &node, &child, &depth, &needToAllocate) ? node : nullptr;

		}

		template <typename T, typename BoundingVolume>
		LooseOctreeNode<T, BoundingVolume>* LooseOctree<T, BoundingVolume>::FindFirst(const BoundingVolume &boundingVolume)
		{

			LooseOctreeNode<T, BoundingVolume> *node;
			unsigned int depth = 0;
			OctreeChild child;
			bool needToAllocate;

			Find(boundingVolume, &node, &child, &depth, &needToAllocate);

			return node;

		}

		template <typename T, typename BoundingVolume>
		bool LooseOctree<T, BoundingVolume>::Find(
			const BoundingVolume &boundingVolume,
			LooseOctreeNode<T, BoundingVolume> **node,
			OctreeChild *child,
			unsigned int *depth,
			bool *needToAllocate)
		{

			*depth          = 0;
			*needToAllocate = false;
			*node           = &m_root;

			mye::math::AABB aabb = mye::math::BoundingAABB(boundingVolume);

			if (!m_bounds.Contains(aabb))
			{
				return false;
			}

			bool done = false;

			do
			{

				*child = ChooseChild(*node, boundingVolume);
				LooseOctreeNode<T, BoundingVolume> *childNode = (*node)->m_children[static_cast<int>(*child)];

				if (childNode)
				{
					if (childNode->m_bounds.ContainsStrict(aabb))
					{
						*node = childNode;
						(*depth)++;
					}
					else
					{
						done = true;
					}
				}
				else
				{
					done = true;
					*needToAllocate = ChildBounds(*node, *child).ContainsStrict(boundingVolume) && *depth < m_maxdepth;
				}

			} while (!done && *depth < m_maxdepth);

			return *needToAllocate ? false : true;

		}

		template <typename T, typename BoundingVolume>
		void LooseOctree<T, BoundingVolume>::AllocateChildren(const BoundingVolume &boundingVolume,
			LooseOctreeNode<T, BoundingVolume> **node,
			OctreeChild *child,
			unsigned int *depth)
		{

			mye::math::AABB childBounds = ChildBounds(*node, *child);

			do
			{

				(*node)->m_children[static_cast<int>(*child)] = new LooseOctreeNode<T, BoundingVolume>(*node, childBounds);
				(*node)                                      = (*node)->m_children[static_cast<int>(*child)];

				*child       = ChooseChild(*node, boundingVolume);
				childBounds  = ChildBounds(*node, *child);

				(*depth)++;

			}
			while (childBounds.ContainsStrict(boundingVolume) && *depth < m_maxdepth);

		}

		template <typename T, typename BoundingVolume>
		bool LooseOctree<T, BoundingVolume>::Remove(const T &object, const BoundingVolume &boundingVolume)
		{

			LooseOctreeNode<T, BoundingVolume>* node = Find(mye::math::BoundingAABB(boundingVolume));

			if (node)
			{

				auto it = node->FindObject(object);

				if (it != node->end())
				{
					node->RemoveObject(it);
					return true;
				}

			}

			return false;

		}

		template <typename T, typename BoundingVolume>
		bool LooseOctree<T, BoundingVolume>::Relocate(const T &object, const BoundingVolume &oldBoundingVolume,
			const BoundingVolume &newBoundingVolume)
		{

			mye::math::AABB oldAABB = mye::math::BoundingAABB(oldBoundingVolume);
			mye::math::AABB newAABB = mye::math::BoundingAABB(newBoundingVolume);

			LooseOctreeNode<T, BoundingVolume>* oldNode = Find(oldAABB);

			if (!oldNode)
			{
				return false;
			}

			LooseOctreeNode<T, BoundingVolume> *newNode;
			OctreeChild child;
			unsigned int depth;
			bool needToAllocate;

			bool found = Find(newAABB, &newNode, &child, &depth, &needToAllocate);

			if (!found && !needToAllocate)
			{
				return false;
			}
			
			if (needToAllocate)
			{
				AllocateChildren(newAABB, &newNode, &child, &depth);
			}

			if (oldNode != newNode)
			{
				oldNode->RemoveObject(oldNode->FindObject(object));
				newNode->AddObject(object);
			}

			return true;

		}
		
		template <typename T, typename BoundingVolume>
		LooseOctreeNode<T, BoundingVolume>* LooseOctree<T, BoundingVolume>::GetRoot(void)
		{
			return &m_root;
		}

		template <typename T, typename BoundingVolume>
		void LooseOctree<T, BoundingVolume>::Clear(void)
		{
			m_root.Clear();
		}

		template <typename T, typename BoundingVolume>
		mye::math::AABB LooseOctree<T, BoundingVolume>::ChildBounds(LooseOctreeNode<T, BoundingVolume> *parent, OctreeChild child) const
		{

			mye::math::Vector3 parentCenter = parent->m_bounds.GetCenter();
			mye::math::Vector3 nonLooseHalfExtents = parent->m_bounds.GetHalfExtents() * 0.5f / m_looseness;

			mye::math::Vector3 center;
			mye::math::Vector3 halfExtents = nonLooseHalfExtents * m_looseness;

			switch (child)
			{

			case OctreeChild::LEFT_BOTTOM_NEAR:

				center.x() = parentCenter.x() - nonLooseHalfExtents.x();
				center.y() = parentCenter.y() - nonLooseHalfExtents.y();
				center.z() = parentCenter.z() - nonLooseHalfExtents.z();
				break;
				

			case OctreeChild::LEFT_BOTTOM_FAR:
				center.x() = parentCenter.x() - nonLooseHalfExtents.x();
				center.y() = parentCenter.y() - nonLooseHalfExtents.y();
				center.z() = parentCenter.z() + nonLooseHalfExtents.z();
				break;

			case OctreeChild::LEFT_TOP_NEAR:
				center.x() = parentCenter.x() - nonLooseHalfExtents.x();
				center.y() = parentCenter.y() + nonLooseHalfExtents.y();
				center.z() = parentCenter.z() - nonLooseHalfExtents.z();
				break;

			case OctreeChild::LEFT_TOP_FAR:
				center.x() = parentCenter.x() - nonLooseHalfExtents.x();
				center.y() = parentCenter.y() + nonLooseHalfExtents.y();
				center.z() = parentCenter.z() + nonLooseHalfExtents.z();
				break;

			case OctreeChild::RIGHT_BOTTOM_NEAR:
				center.x() = parentCenter.x() + nonLooseHalfExtents.x();
				center.y() = parentCenter.y() - nonLooseHalfExtents.y();
				center.z() = parentCenter.z() - nonLooseHalfExtents.z();
				break;

			case OctreeChild::RIGHT_BOTTOM_FAR:
				center.x() = parentCenter.x() + nonLooseHalfExtents.x();
				center.y() = parentCenter.y() - nonLooseHalfExtents.y();
				center.z() = parentCenter.z() + nonLooseHalfExtents.z();
				break;

			case OctreeChild::RIGHT_TOP_NEAR:
				center.x() = parentCenter.x() + nonLooseHalfExtents.x();
				center.y() = parentCenter.y() + nonLooseHalfExtents.y();
				center.z() = parentCenter.z() - nonLooseHalfExtents.z();
				break;

			case OctreeChild::RIGHT_TOP_FAR:
				center.x() = parentCenter.x() + nonLooseHalfExtents.x();
				center.y() = parentCenter.y() + nonLooseHalfExtents.y();
				center.z() = parentCenter.z() + nonLooseHalfExtents.z();
				break;

			default:
				assert(false && "LooseOctree: Bad child");
				break;

			}

			return mye::math::AABB::FromCenterHalfExtents(center, halfExtents);

		}

		template <typename T, typename BoundingVolume>
		OctreeChild LooseOctree<T, BoundingVolume>::ChooseChild(LooseOctreeNode<T, BoundingVolume> *parent,
			const BoundingVolume &boundingVolume) const
		{

			mye::math::AABB AABBt = mye::math::BoundingAABB(boundingVolume);

			mye::math::Vector3 center = AABBt.GetCenter();
			mye::math::Vector3 parentCenter = parent->m_bounds.GetCenter();

			if (center.x() < parentCenter.x())
			{

				if (center.y() < parentCenter.y())
				{

					if (center.z() < parentCenter.z())
					{
						return OctreeChild::LEFT_BOTTOM_NEAR;
					}
					else
					{
						return OctreeChild::LEFT_BOTTOM_FAR;
					}

				}
				else
				{

					if (center.z() < parentCenter.z())
					{
						return OctreeChild::LEFT_TOP_NEAR;
					}
					else
					{
						return OctreeChild::LEFT_TOP_FAR;
					}

				}

			}
			else
			{

				if (center.y() < parentCenter.y())
				{

					if (center.z() < parentCenter.z())
					{
						return OctreeChild::RIGHT_BOTTOM_NEAR;
					}
					else
					{
						return OctreeChild::RIGHT_BOTTOM_FAR;
					}

				}
				else
				{

					if (center.z() < parentCenter.z())
					{
						return OctreeChild::RIGHT_TOP_NEAR;
					}
					else
					{
						return OctreeChild::RIGHT_TOP_FAR;
					}

				}

			}

			assert(false);

			return OctreeChild::NONE;

		}

		/* Loose Octree Traverser */

		template <typename T, typename BoundingVolume>
		LooseOctreeTraverser<T, BoundingVolume>::LooseOctreeTraverser(LooseOctree<T, BoundingVolume> &octree) :
			m_octree(&octree)
		{
		}

		template <typename T, typename BoundingVolume>
		LooseOctreeTraverser<T, BoundingVolume>::~LooseOctreeTraverser(void)
		{
		}

		template <typename T, typename BoundingVolume>
		LooseOctreeNode<T, BoundingVolume>* LooseOctreeTraverser<T, BoundingVolume>::GetCurrent(void)
		{
			return (!m_path.size() ? &(m_octree->m_root) : m_path.front());
		}

		template <typename T, typename BoundingVolume>
		LooseOctreeNode<T, BoundingVolume>* LooseOctreeTraverser<T, BoundingVolume>::GetParent(void)
		{
			return (!m_path.size() ? nullptr : m_path.front());
		}

		template <typename T, typename BoundingVolume>
		void LooseOctreeTraverser<T, BoundingVolume>::MoveToParent(void)
		{
			if (m_path.size() > 0)
			{
				m_path.pop_front();
			}
		}

		template <typename T, typename BoundingVolume>
		bool LooseOctreeTraverser<T, BoundingVolume>::MoveToChild(OctreeChild child)
		{

			LooseOctreeNode *node = m_octree->m_children[child];

			if (node)
			{
				m_path.push_front(node);
				return true;
			}

			return false;

		}

		template <typename T, typename BoundingVolume>
		void LooseOctreeTraverser<T, BoundingVolume>::MoveToRoot(void)
		{
			m_path.clear();
		}

		template <typename T, typename BoundingVolume>
		unsigned int LooseOctreeTraverser<T, BoundingVolume>::GetDepth(void) const
		{
			return m_path.size();
		}

	}

}