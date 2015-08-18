#pragma once

#include <iterator>
#include <unordered_map>

#include "String.h"

namespace mye
{

	namespace core
	{

		struct GameObjectHandle
		{

			GameObjectHandle(void);
			GameObjectHandle(int id, int allocation);

			int id;
			int allocation;

		};

		inline bool operator== (const GameObjectHandle & a,
			const GameObjectHandle & b);

		class GameObjectHandleIDHasher
		{

		public:

			inline size_t operator()(const mye::core::GameObjectHandle &hObj) const;

		};

		class GameObject;
		class GameObjectsManager;

		typedef std::unordered_map<GameObjectHandle, GameObject *, GameObjectHandleIDHasher> ObjectsHashMap;
		typedef std::unordered_multimap<String, GameObjectHandle>                            ObjectsNamesMap;

		class ActiveGameObjectsIterator :
			public std::iterator<std::input_iterator_tag, GameObjectHandle>
		{

		public:

			ActiveGameObjectsIterator(GameObjectsManager * gom,
									  const ObjectsHashMap::iterator & it);

			ActiveGameObjectsIterator & operator++ (void);
			ActiveGameObjectsIterator   operator++ (int);

			bool operator == (const ActiveGameObjectsIterator &) const;
			bool operator != (const ActiveGameObjectsIterator &) const;

			GameObjectHandle operator * (void) const;

		private:

			ObjectsHashMap::iterator   m_it;
			GameObjectsManager       * m_gom;

		};

		class GameObjectsManager
		{

		public:

			typedef ActiveGameObjectsIterator Iterator;

			GameObjectsManager(void);
			~GameObjectsManager(void);

			GameObjectHandle Create(void);
			GameObjectHandle Create(const String& name);

			GameObjectHandle CreateEntity(const String & entity);
			GameObjectHandle CreateEntity(const String & entity,
				const String & name);

// 			void Rename(const GameObjectHandle &hObj,
// 				const String &name);

			void Destroy(const GameObjectHandle & hObj);

			inline GameObject * Get(const GameObjectHandle & hObj);

			void Rename(const GameObjectHandle & hObj, const String & name);

			GameObjectHandle Find(const String & name);

			Iterator begin(void);
			Iterator end(void);

			/*void Reset(void);*/

		protected:

			inline GameObjectHandle CreateHandle(void);
			inline void FreeHandle(const GameObjectHandle & hObj);

			

			ObjectsHashMap  m_objects;
			ObjectsNamesMap m_namedObjects;

			std::list<GameObjectHandle> m_freeHandles;
			int                         m_lastId;

		private:

			struct __ObjectInfo {
				GameObjectHandle handle;
				GameObject *object;
			};

			__ObjectInfo __InstantiateObject(const String & name);
			void         __InitObject(__ObjectInfo);

		};

	}

}

#include "GameObjectsManager.inl"