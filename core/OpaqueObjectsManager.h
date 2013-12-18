#pragma once

#include <vector>
#include <list>

namespace mye
{

	namespace core
	{

		template <typename T>
		struct OpaqueObjectHandle
		{

			int id;
			int allocation;

			OpaqueObjectHandle(void);
			OpaqueObjectHandle(int id, int allocation);

		};

		/* Requires T to have a default constructor and a Clear() method */

		template <typename T>
		class OpaqueObjectsManager
		{

		public:

			OpaqueObjectsManager(void);
			~OpaqueObjectsManager(void);

			OpaqueObjectHandle<T> Create(void);
			T* Get(const OpaqueObjectHandle<T> &hObj);
			void Destroy(const OpaqueObjectHandle<T> &hObj);

		private:

			struct Allocation
			{

				T* object;
				int allocation;

				Allocation(void);
				Allocation(T *object, int allocation);

			};

			std::vector<Allocation> _objects;
			std::list<int> _free;

		};

		#include "OpaqueObjectsManager.inl"

	}

}
