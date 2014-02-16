namespace mye
{

	namespace core
	{

		/* Loose Octree Node */

		MYE_DEFINE_TEMPLATE_POOL_ALLOCATOR(template <typename T>, LooseOctreeNode<T>)

		template <typename T>
		LooseOctreeNode<T>::LooseOctreeNode(LooseOctreeNode<T> *parent,
			const mye::math::AABBf& bounds) :
			m_parent(parent),
			m_bounds(bounds)
		{
			memset(m_children, 0, sizeof(void*) * 8);
		}
		
		template <typename T>
		LooseOctreeNode<T>::~LooseOctreeNode(void)
		{
			Clear();
		}

		template <typename T>
		void LooseOctreeNode<T>::Clear(void)
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

		template <typename T>
		mye::math::AABBf LooseOctreeNode<T>::GetBounds(void) const
		{
			return m_bounds;
		}

		template <typename T>
		LooseOctreeNode<T>* LooseOctreeNode<T>::GetChild(OctreeChild child)
		{
			return m_children[static_cast<int>(child)];
		}
		
		template <typename T>
		unsigned int LooseOctreeNode<T>::GetObjectsCount(void) const
		{
			return m_objects.size();
		}

		template <typename T>
		void LooseOctreeNode<T>::AddObject(const T &object)
		{
			m_objects.push_back(object);
		}

		template <typename T>
		typename LooseOctreeNode<T>::Iterator LooseOctreeNode<T>::FindObject(const T &object)
		{
			return Iterator(std::find(m_objects.begin(), m_objects.end(), object));
		}

		template <typename T>
		typename LooseOctreeNode<T>::Iterator LooseOctreeNode<T>::RemoveObject(const Iterator &it)
		{
			return Iterator(m_objects.erase(it.m_it));
		}

		template <typename T>
		typename LooseOctreeNode<T>::Iterator LooseOctreeNode<T>::begin(void)
		{
			return Iterator(m_objects.begin());
		}

		template <typename T>
		typename LooseOctreeNode<T>::Iterator LooseOctreeNode<T>::end(void)
		{
			return Iterator(m_objects.end());
		}

		/* Loose Octree Node Iterator */

		template <typename T>
		LooseOctreeNode<T>::Iterator::Iterator(const Iterator &it) :
			m_it(it.m_it)
		{
		}

		template <typename T>
		LooseOctreeNode<T>::Iterator::Iterator(typename const std::list<T>::iterator &it) :
			m_it(it)
		{
		}

		template <typename T>
		T& LooseOctreeNode<T>::Iterator::operator* (void) const
		{
			return *m_it;
		}

		template <typename T>
		bool LooseOctreeNode<T>::Iterator::operator!= (const Iterator &it) const
		{
			return m_it != it.m_it;
		}

		template <typename T>
		typename LooseOctreeNode<T>::Iterator& LooseOctreeNode<T>::Iterator::operator++ (void)
		{
			m_it++;
			return *this;
		}

		/* Loose Octree */

		template <typename T>
		LooseOctree<T>::LooseOctree(mye::math::Vector3f center,
			float size,
			unsigned int maxdepth,
			unsigned int looseness) :
		m_root(nullptr, mye::math::AABBf::FromCenterHalfExtents(center, mye::math::Vector3f(size * 0.5f * looseness))),
			m_bounds(mye::math::AABBf::FromCenterHalfExtents(center, mye::math::Vector3f(size * 0.5f))),
			m_maxdepth(maxdepth),
			m_looseness(looseness)
		{
		}

		template <typename T>
		LooseOctree<T>::~LooseOctree(void)
		{
			m_root.Clear();
		}

		template <typename T>
		bool LooseOctree<T>::Insert(const T &object, const mye::math::AABBf &aabb)
		{

			LooseOctreeNode<T> *node;
			OctreeChild child;
			unsigned int depth;
			bool needToAllocate;

			bool found = Find(aabb, &node, &child, &depth, &needToAllocate);

			if (!found && !needToAllocate)
			{
				return false;
			}

			if (needToAllocate)
			{
				AllocateChildren(aabb, &node, &child, &depth);
			}

			node->AddObject(object);

			return true;

		}

		template <typename T>
		LooseOctreeNode<T>* LooseOctree<T>::Find(const mye::math::AABBf &aabb)
		{

			LooseOctreeNode<T> *node;
			unsigned int depth = 0;
			OctreeChild child;
			bool needToAllocate;

			return Find(aabb, &node, &child, &depth, &needToAllocate) ? node : nullptr;

		}

		template <typename T>
		LooseOctreeNode<T>* LooseOctree<T>::FindFirst(const mye::math::AABBf &aabb)
		{

			LooseOctreeNode<T> *node;
			unsigned int depth = 0;
			OctreeChild child;
			bool needToAllocate;

			Find(aabb, &node, &child, &depth, &needToAllocate);

			return node;

		}

		template <typename T>
		bool LooseOctree<T>::Find(
			const mye::math::AABBf &aabb,
			LooseOctreeNode<T> **node,
			OctreeChild *child,
			unsigned int *depth,
			bool *needToAllocate)
		{

			*depth          = 0;
			*needToAllocate = false;
			*node           = &m_root;

			if (!m_bounds.Contains(aabb))
			{
				return false;
			}

			bool done = false;

			do
			{

				*child = ChooseChild(*node, aabb);
				LooseOctreeNode<T> *childNode = (*node)->m_children[static_cast<int>(*child)];

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
					*needToAllocate = ChildBounds(*node, *child).ContainsStrict(aabb) && *depth < m_maxdepth;
				}

			} while (!done && *depth < m_maxdepth);

			return *needToAllocate ? false : true;

		}

		template <typename T>
		void LooseOctree<T>::AllocateChildren(const mye::math::AABBf &aabb,
			LooseOctreeNode<T> **node,
			OctreeChild *child,
			unsigned int *depth)
		{

			mye::math::AABBf childBounds = ChildBounds(*node, *child);

			do
			{

				(*node)->m_children[static_cast<int>(*child)] = new LooseOctreeNode<T>(*node, childBounds);
				(*node)                                      = (*node)->m_children[static_cast<int>(*child)];

				*child       = ChooseChild(*node, aabb);
				childBounds  = ChildBounds(*node, *child);

				(*depth)++;

			}
			while (childBounds.ContainsStrict(aabb) && *depth < m_maxdepth);

		}

		template <typename T>
		bool LooseOctree<T>::Remove(const T &object, const mye::math::AABBf &aabb)
		{

			LooseOctreeNode<T>* node = Find(aabb);

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

		template <typename T>
		bool LooseOctree<T>::Relocate(const T &object, const mye::math::AABBf &oldAABB, const mye::math::AABBf &newAABB)
		{

			LooseOctreeNode<T>* oldNode = Find(oldAABB);

			if (!oldNode)
			{
				return false;
			}

			LooseOctreeNode<T> *newNode;
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
		
		template <typename T>
		LooseOctreeNode<T>* LooseOctree<T>::GetRoot(void)
		{
			return &m_root;
		}

		template <typename T>
		mye::math::AABBf LooseOctree<T>::ChildBounds(LooseOctreeNode<T> *parent, OctreeChild child) const
		{

			mye::math::Vector3f parentCenter = parent->m_bounds.GetCenter();
			mye::math::Vector3f nonLooseHalfExtents = parent->m_bounds.GetHalfExtents() * 0.5f / m_looseness;

			mye::math::Vector3f center;
			mye::math::Vector3f halfExtents = nonLooseHalfExtents * m_looseness;

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

			return mye::math::AABBf::FromCenterHalfExtents(center, halfExtents);

		}

		template <typename T>
		OctreeChild LooseOctree<T>::ChooseChild(LooseOctreeNode<T> *parent, const mye::math::AABBf &aabb) const
		{

			mye::math::Vector3f center = aabb.GetCenter();
			mye::math::Vector3f parentCenter = parent->m_bounds.GetCenter();

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

		template <typename T>
		LooseOctreeTraverser<T>::LooseOctreeTraverser(LooseOctree<T> &octree) :
			m_octree(&octree)
		{
		}

		template <typename T>
		LooseOctreeTraverser<T>::~LooseOctreeTraverser(void)
		{
		}

		template <typename T>
		LooseOctreeNode<T>* LooseOctreeTraverser<T>::GetCurrent(void)
		{
			return (!m_path.size() ? &(m_octree->m_root) : m_path.front());
		}

		template <typename T>
		LooseOctreeNode<T>* LooseOctreeTraverser<T>::GetParent(void)
		{
			return (!m_path.size() ? nullptr : m_path.front());
		}

		template <typename T>
		void LooseOctreeTraverser<T>::MoveToParent(void)
		{
			if (m_path.size() > 0)
			{
				m_path.pop_front();
			}
		}

		template <typename T>
		bool LooseOctreeTraverser<T>::MoveToChild(OctreeChild child)
		{

			LooseOctreeNode *node = m_octree->m_children[child];

			if (node)
			{
				m_path.push_front(node);
				return true;
			}

			return false;

		}

		template <typename T>
		void LooseOctreeTraverser<T>::MoveToRoot(void)
		{
			m_path.clear();
		}

		template <typename T>
		unsigned int LooseOctreeTraverser<T>::GetDepth(void) const
		{
			return m_path.size();
		}

	}

}