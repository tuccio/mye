#pragma once

#include <vector>
#include <list>
#include <string>
#include <map>

#include "INamedObject.h"

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

			bool IsValid(void) const
			{
				return id >= 0 && allocation >= 0;
			}

		};

		template <typename T>
		bool operator== (const OpaqueObjectHandle<T> &a, const OpaqueObjectHandle<T> &b)
		{
			return a.id == b.id && a.allocation == b.allocation;
		}

		/*
		
			Requires T to have:
				- Default constructor
				- Accessible SetName(const std::string&)
				- Accessible OnCreation(OpaqueObjectsManager*, OpaqueObjectHandle)
				- Accessible OnDestruction(void)

		*/

		template <typename T>
		class OpaqueObjectsManager
		{

		public:

			OpaqueObjectsManager(void);
			~OpaqueObjectsManager(void);

			OpaqueObjectHandle<T> Create(void);
			OpaqueObjectHandle<T> Create(const std::string& name);

			void Destroy(const OpaqueObjectHandle<T> &hObj);

			T* Get(const OpaqueObjectHandle<T> &hObj);
			OpaqueObjectHandle<T> Find(const std::string &name);

		protected:

			struct Allocation
			{

				T* object;
				int allocation;

				Allocation(void);
				Allocation(T *object, int allocation);

			};

			std::vector<Allocation> m_objects;
			std::list<int> m_freeIds;
			std::multimap<std::string, OpaqueObjectHandle<T>> m_names;

		};

		#include "OpaqueObjectsManager.inl"

	}

}
