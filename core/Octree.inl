namespace mye
{

	namespace core
	{

		/* Octree Node */

		/*MYE_DEFINE_TEMPLATE_POOL_ALLOCATOR(template <typename T>, GameObject<T>)*/

		OctreeNode::OctreeNode(bool leaf)
		{
			m_leaf = leaf;
		}

		bool OctreeNode::IsLeaf(void) const
		{
			return m_leaf;
		}

		/* Octree Position Grabber */

		template <typename T>
		mye::math::Vector3f OctreePositionGrabber<T>::operator() (T &o)
		{
			assert(false && "OctreePositionGrabber<T> specialization not found");
			throw;
			return mye::math::Vector3f();
		}

		mye::math::Vector3f OctreePositionGrabber<mye::math::Vector3f>::operator() (mye::math::Vector3f &v)
		{
			return v;
		}

		mye::math::Vector3f OctreePositionGrabber<mye::math::Transformf>::operator() (mye::math::Transformf &t)
		{
			return t.GetPosition();
		}

		/* Octree Leaf */

		template <typename T>
		OctreeLeaf<T>::OctreeLeaf(void) :
			OctreeNode(true)
		{
		}

		template <typename T>
		typename std::vector<T>::iterator OctreeLeaf<T>::begin(void)
		{
			return m_objects.begin();
		}

		template <typename T>
		typename std::vector<T>::iterator OctreeLeaf<T>::end(void)
		{
			return m_objects.end();
		}

// 		template <typename T>
// 		void OctreeLeaf<T>::AddObject(T &object)
// 		{
// 			m_objects.push_back(object);
// 		}
// 
// 		template <typename T>
// 		void OctreeLeaf<T>::RemoveObject(T &object)
// 		{
// 			m_objects.remove_if(object, std::equal(object));
// 		}

		/* Octree Internal Node */

		OctreeInternalNode::OctreeInternalNode(void) :
			OctreeNode(false)
		{
			memset(m_children, 0, sizeof(void*) * 8);
			m_childrenCount = 0;
		}

		OctreeInternalNode::~OctreeInternalNode(void)
		{
			Clear();
		}

		void OctreeInternalNode::Clear(void)
		{

			for (int i = 0; i < 8; i++)
			{

				if (m_children[i])
				{
					delete m_children[i];
					m_children[i] = nullptr;
				}

			}

			m_childrenCount = 0;

		}

		OctreeNode* OctreeInternalNode::GetChild(Children child)
		{
			return m_children[child];
		}

		const OctreeNode* OctreeInternalNode::GetChild(Children child) const
		{
			return m_children[child];
		}

		/* Octree */

		template <typename T>
		Octree<T>::Octree(const mye::math::Vector3f &center,
			const mye::math::Vector3f &size,
			unsigned int maxDepth) :
		m_maxDepth(maxDepth)
		{
			m_bounds = mye::math::AABB::FromCenterHalfExtents(center, size * 0.5f);
		}

		template <typename T>
		Octree<T>::~Octree(void)
		{
			Clear();
		}

		template <typename T>
		void Octree<T>::Clear(void)
		{
			m_root.Clear();
		}

		template <typename T>
		void Octree<T>::Insert(T &object)
		{
			
			mye::math::Vector3f x = OctreePositionGrabber<T>()(object);

			OctreeTraverser<T> traverser(*this);

			if (!traverser.Traverse(x))
			{
				return;
			}

			AABB bounds = traverser.GetBounds();

			OctreeInternalNode *previous = traverser.GetParent();
			OctreeNode *current = traverser.GetCurrent();

			if (!current->IsLeaf())
			{

				OctreeInternalNode::Children child = __PickSide(traverser.GetBounds(), x);
				OctreeLeaf<T>* leaf = new OctreeLeaf<T>;
				OctreeInternalNode *node = static_cast<OctreeInternalNode*>(current);
				node->m_children[child] = leaf;
				node->m_childrenCount++;
				leaf->m_objects.push_back(object);

			}
			else
			{

				OctreeLeaf<T> *leaf = static_cast<OctreeLeaf<T>*>(current);
				mye::math::Vector3f leafX = OctreePositionGrabber<T>()(leaf->m_objects.front());
				 
				if (leafX == x || traverser.GetDepth() == m_maxDepth)
				{
					leaf->m_objects.push_back(object);
				}
				else
				{

					OctreeInternalNode::Children child = traverser.GetChildSide();

					current = static_cast<OctreeNode*>(new OctreeInternalNode);
					previous->m_children[child] = current;

					OctreeInternalNode::Children newSide;
					OctreeInternalNode::Children oldLeafSide;

					do 
					{

						newSide = __PickSide(bounds, x);
						oldLeafSide = __PickSide(bounds, leafX);

						previous = static_cast<OctreeInternalNode*>(current);

						if (newSide == oldLeafSide)
						{

							current = static_cast<OctreeNode*>(new OctreeInternalNode);
							previous->m_children[newSide] = current;
							previous->m_childrenCount = 1;

							bounds = __SplitAABB(bounds, newSide);

						}
						else
						{

							OctreeInternalNode *finalNode = static_cast<OctreeInternalNode*>(current);

							OctreeLeaf<T> *newLeaf = new OctreeLeaf<T>();
							newLeaf->m_objects.push_back(object);

							finalNode->m_childrenCount = 2;
							finalNode->m_children[newSide] = newLeaf;
							finalNode->m_children[oldLeafSide] = leaf;

							break;

						}
						

					} while (true);

				}

			}

		}

		template <typename T>
		void Octree<T>::Relocate(T &object,
			const mye::math::Vector3f &xOld)
		{
		}

		template <typename T>
		bool Octree<T>::Remove(T &object)
		{
			return Remove(object, OctreePositionGrabber<T>()(object));
		}

		template <typename T>
		bool Octree<T>::Remove(T &object, const mye::math::Vector3f &x)
		{

			bool removed = false;
			
			OctreeTraverser<T> traverser(*this);

			if (traverser.Traverse(x) &&
				traverser.GetCurrent()->IsLeaf())
			{
				
				OctreeLeaf<T> *leaf = static_cast<OctreeLeaf<T>*>(traverser.GetCurrent());
				unsigned int elements = leaf->m_objects.size();

				leaf->m_objects.erase(
					std::remove_if(
						leaf->m_objects.begin(),
						leaf->m_objects.end(),
						[object](T& o)->bool
						{
							return object == o;
						}),
					leaf->m_objects.end()
				);

				unsigned int newSize = leaf->m_objects.size();

				if (elements != newSize)
				{

					removed = true;

					if (newSize == 0)
					{

						// Find the first anchestor we need to destroy

						OctreeInternalNode *parent = traverser.GetParent();
						OctreeInternalNode::Children childSide = traverser.GetChildSide();

						while (parent && parent->m_childrenCount == 1)
						{

							traverser.MoveToParent();
							parent = traverser.GetParent();
							childSide = traverser.GetChildSide();

						}

						if (!parent)
						{
							Clear();
						}
						else
						{
							delete parent->m_children[childSide];
							parent->m_children[childSide] = nullptr;
							parent->m_childrenCount--;
						}

					}

				}
				

			}
			
			return removed;

		}

		template <typename T>
		unsigned int Octree<T>::GetMaxDepth(void) const
		{
			return m_maxDepth;
		}

		template <typename T>
		const OctreeInternalNode* Octree<T>::GetRoot(void) const
		{
			return &m_root;
		}

		template <typename T>
		OctreeInternalNode* Octree<T>::GetRoot(void)
		{
			return &m_root;
		}

		template <typename T>
		const mye::math::AABB& Octree<T>::GetBounds(void) const
		{
			return m_bounds;
		}		

		/* Octree Traverser */

		template <typename T>
		OctreeTraverser<T>::OctreeTraverser(Octree<T> &octree) :
			m_octree(&octree),
			m_bounds(octree.GetBounds())			
		{

			NodeInfo info =
			{
				static_cast<OctreeNode*>(m_octree->GetRoot())
			};

			m_path.push_front(info);

		}

		template <typename T>
		Octree<T>& OctreeTraverser<T>::GetOctree(void)
		{
			return *m_octree;
		}

		template <typename T>
		const Octree<T>& OctreeTraverser<T>::GetOctree(void) const
		{
			return *m_octree;
		}

		template <typename T>
		const mye::math::AABB& OctreeTraverser<T>::GetBounds(void) const
		{
			return m_bounds;
		}

		template <typename T>
		unsigned int OctreeTraverser<T>::GetDepth(void) const
		{
			return m_path.size() - 1;
		}

		template <typename T>
		OctreeNode* OctreeTraverser<T>::GetCurrent(void)
		{
			return static_cast<OctreeNode*>(m_path.front().node);
		}

		template <typename T>
		const OctreeNode* OctreeTraverser<T>::GetCurrent(void) const
		{
			return static_cast<OctreeNode*>(m_path.front().node);
		}

		template <typename T>
		OctreeInternalNode* OctreeTraverser<T>::GetParent(void)
		{
			if (m_path.size() == 1)
			{
				return nullptr;
			}
			else
			{
				return static_cast<OctreeInternalNode*>(m_path[1].node);
			}
		}

		template <typename T>
		const OctreeInternalNode* OctreeTraverser<T>::GetParent(void) const
		{
			if (m_path.size() == 1)
			{
				return nullptr;
			}
			else
			{
				return static_cast<const OctreeInternalNode*>(m_path[1].node);
			}
		}

		template <typename T>
		OctreeInternalNode::Children OctreeTraverser<T>::GetChildSide(void) const
		{
			return m_path.front().child;
		}

		template <typename T>
		bool OctreeTraverser<T>::Traverse(const mye::math::Vector3f &x)
		{
			
			if (!m_octree->GetBounds().Contains(x))
			{
				return false;
			}

			MoveToRoot();

			OctreeInternalNode::Children child;

			do 
			{
				child = __PickSide(m_bounds, x);
			} while (MoveToChild(child));

			return true;

		}

		template <typename T>
		bool OctreeTraverser<T>::MoveToChild(OctreeInternalNode::Children child)
		{
			
			if (!m_path.front().node->IsLeaf())
			{

				NodeInfo info =
				{
					static_cast<OctreeNode*>(static_cast<OctreeInternalNode*>(m_path.front().node)->GetChild(child)),
					child
				};

				if (info.node)
				{
					m_path.push_front(info);
					m_bounds = __SplitAABB(GetBounds(), child);
					return true;
				}

			}

			return false;

		}

		template <typename T>
		OctreeInternalNode* OctreeTraverser<T>::MoveToParent(void)
		{

			if (m_path.size() > 1)
			{
				m_bounds = __InverseSplitAABB(GetBounds(), m_path.front().child);
				m_path.pop_front();
				return static_cast<OctreeInternalNode*>(m_path.front().node);
			}
			
			return nullptr;

		}

		template <typename T>
		OctreeInternalNode* OctreeTraverser<T>::MoveToRoot(void)
		{

			m_bounds = m_octree->GetBounds();
			m_path.clear();

			NodeInfo info =
			{
				static_cast<OctreeNode*>(m_octree->GetRoot())
			};

			m_path.push_front(info);

			return static_cast<OctreeInternalNode*>(info.node);

		}

		OctreeInternalNode::Children __PickSide(
			const mye::math::AABB &bounds,
			const mye::math::Vector3f &x)
		{

			OctreeInternalNode::Children child;
			mye::math::Vector3f center = bounds.GetCenter();

			if (mye::math::EpsilonLessEqual(x.x(), center.x())) // left
			{

				if (mye::math::EpsilonLessEqual(x.y(), center.y())) // bottom
				{

					if (mye::math::EpsilonLessEqual(x.z(), center.z())) // front
					{
						child = OctreeInternalNode::FRONT_LEFT_BOTTOM;
					}
					else // back
					{
						child = OctreeInternalNode::BACK_LEFT_BOTTOM;
					}

				}
				else // top
				{

					if (mye::math::EpsilonLessEqual(x.z(), center.z())) // front
					{
						child = OctreeInternalNode::FRONT_LEFT_TOP;
					}
					else // back
					{
						child = OctreeInternalNode::BACK_LEFT_TOP;
					}

				}

			}
			else // right
			{

				if (mye::math::EpsilonLessEqual(x.y(), center.y())) // bottom
				{

					if (mye::math::EpsilonLessEqual(x.z(), center.z())) // front
					{
						child = OctreeInternalNode::FRONT_RIGHT_BOTTOM;
					}
					else // back
					{
						child = OctreeInternalNode::BACK_RIGHT_BOTTOM;
					}

				}
				else // top
				{

					if (mye::math::EpsilonLessEqual(x.z(), center.z())) // front
					{
						child = OctreeInternalNode::FRONT_RIGHT_TOP;
					}
					else // back
					{
						child = OctreeInternalNode::BACK_RIGHT_TOP;
					}

				}

			}

			return child;

		}

		mye::math::AABB __SplitAABB(const mye::math::AABB &bounds, OctreeInternalNode::Children child)
		{

			mye::math::AABB aabb;

			mye::math::Vector3f oldCenter = bounds.GetCenter();
			mye::math::Vector3f halfExtents = bounds.GetHalfExtents() * 0.5f;

			switch (child)
			{

			case OctreeInternalNode::FRONT_LEFT_BOTTOM:
				aabb = mye::math::AABB::FromCenterHalfExtents(
					mye::math::Vector3f(
					oldCenter.x() - halfExtents.x(),
					oldCenter.y() - halfExtents.y(),
					oldCenter.z() - halfExtents.z()),
					halfExtents);
				break;
			case OctreeInternalNode::FRONT_RIGHT_BOTTOM:
				aabb = mye::math::AABB::FromCenterHalfExtents(
					mye::math::Vector3f(
					oldCenter.x() + halfExtents.x(),
					oldCenter.y() - halfExtents.y(),
					oldCenter.z() - halfExtents.z()),
					halfExtents);
				break;
			case OctreeInternalNode::FRONT_RIGHT_TOP:
				aabb = mye::math::AABB::FromCenterHalfExtents(
					mye::math::Vector3f(
					oldCenter.x() + halfExtents.x(),
					oldCenter.y() + halfExtents.y(),
					oldCenter.z() - halfExtents.z()),
					halfExtents);
				break;
			case OctreeInternalNode::FRONT_LEFT_TOP:
				aabb = mye::math::AABB::FromCenterHalfExtents(
					mye::math::Vector3f(
					oldCenter.x() - halfExtents.x(),
					oldCenter.y() + halfExtents.y(),
					oldCenter.z() - halfExtents.z()),
					halfExtents);
				break;
			case OctreeInternalNode::BACK_LEFT_TOP:
				aabb = mye::math::AABB::FromCenterHalfExtents(
					mye::math::Vector3f(
					oldCenter.x() - halfExtents.x(),
					oldCenter.y() + halfExtents.y(),
					oldCenter.z() + halfExtents.z()),
					halfExtents);
				break;
			case OctreeInternalNode::BACK_RIGHT_TOP:
				aabb = mye::math::AABB::FromCenterHalfExtents(
					mye::math::Vector3f(
					oldCenter.x() + halfExtents.x(),
					oldCenter.y() + halfExtents.y(),
					oldCenter.z() + halfExtents.z()),
					halfExtents);
				break;
			case OctreeInternalNode::BACK_RIGHT_BOTTOM:
				aabb = mye::math::AABB::FromCenterHalfExtents(
					mye::math::Vector3f(
					oldCenter.x() + halfExtents.x(),
					oldCenter.y() - halfExtents.y(),
					oldCenter.z() + halfExtents.z()),
					halfExtents);
				break;
			case OctreeInternalNode::BACK_LEFT_BOTTOM:
				aabb = mye::math::AABB::FromCenterHalfExtents(
					mye::math::Vector3f(
					oldCenter.x() - halfExtents.x(),
					oldCenter.y() - halfExtents.y(),
					oldCenter.z() + halfExtents.z()),
					halfExtents);
				break;

			}

			return aabb;

		}

		mye::math::AABB __InverseSplitAABB(const mye::math::AABB &bounds, OctreeInternalNode::Children child)
		{

			mye::math::AABB aabb;

			mye::math::Vector3f oldCenter = bounds.GetCenter();
			mye::math::Vector3f dCenter = bounds.GetHalfExtents();
			mye::math::Vector3f halfExtents = bounds.GetHalfExtents() * 2.0f;

			switch (child)
			{

			case OctreeInternalNode::FRONT_LEFT_BOTTOM:
				aabb = mye::math::AABB::FromCenterHalfExtents(
					mye::math::Vector3f(
					oldCenter.x() + dCenter.x(),
					oldCenter.y() + dCenter.y(),
					oldCenter.z() + dCenter.z()),
					halfExtents);
				break;
			case OctreeInternalNode::FRONT_RIGHT_BOTTOM:
				aabb = mye::math::AABB::FromCenterHalfExtents(
					mye::math::Vector3f(
					oldCenter.x() - dCenter.x(),
					oldCenter.y() + dCenter.y(),
					oldCenter.z() + dCenter.z()),
					halfExtents);
				break;
			case OctreeInternalNode::FRONT_RIGHT_TOP:
				aabb = mye::math::AABB::FromCenterHalfExtents(
					mye::math::Vector3f(
					oldCenter.x() - dCenter.x(),
					oldCenter.y() - dCenter.y(),
					oldCenter.z() + dCenter.z()),
					halfExtents);
				break;
			case OctreeInternalNode::FRONT_LEFT_TOP:
				aabb = mye::math::AABB::FromCenterHalfExtents(
					mye::math::Vector3f(
					oldCenter.x() + dCenter.x(),
					oldCenter.y() - dCenter.y(),
					oldCenter.z() + dCenter.z()),
					halfExtents);
				break;
			case OctreeInternalNode::BACK_LEFT_TOP:
				aabb = mye::math::AABB::FromCenterHalfExtents(
					mye::math::Vector3f(
					oldCenter.x() + dCenter.x(),
					oldCenter.y() - dCenter.y(),
					oldCenter.z() - dCenter.z()),
					halfExtents);
				break;
			case OctreeInternalNode::BACK_RIGHT_TOP:
				aabb = mye::math::AABB::FromCenterHalfExtents(
					mye::math::Vector3f(
					oldCenter.x() - dCenter.x(),
					oldCenter.y() - dCenter.y(),
					oldCenter.z() - dCenter.z()),
					halfExtents);
				break;
			case OctreeInternalNode::BACK_RIGHT_BOTTOM:
				aabb = mye::math::AABB::FromCenterHalfExtents(
					mye::math::Vector3f(
					oldCenter.x() - dCenter.x(),
					oldCenter.y() + dCenter.y(),
					oldCenter.z() - dCenter.z()),
					halfExtents);
				break;
			case OctreeInternalNode::BACK_LEFT_BOTTOM:
				aabb = mye::math::AABB::FromCenterHalfExtents(
					mye::math::Vector3f(
					oldCenter.x() + dCenter.x(),
					oldCenter.y() + dCenter.y(),
					oldCenter.z() - dCenter.z()),
					halfExtents);
				break;

			}

			return aabb;

		}

	}
}
