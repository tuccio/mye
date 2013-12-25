#pragma once

#include <vector>
#include <list>
#include <string>
#include <map>

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

		/* Requires T to have a default constructor,
		   a SetName(const std::string&) and a Clear(void) method */

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
			std::multimap<std::string, OpaqueObjectHandle<T>> _names;

		};

		#include "OpaqueObjectsManager.inl"

	}

}
