#pragma once

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

		inline bool operator== (const GameObjectHandle& a,
			const GameObjectHandle& b);

		class GameObjectHandleIDHasher
		{

		public:

			inline size_t operator()(const mye::core::GameObjectHandle &hObj) const;

		};

		class GameObject;

		class GameObjectsManager
		{

		public:

			GameObjectsManager(void);
			~GameObjectsManager(void);

			GameObjectHandle Create(void);
			GameObjectHandle Create(const String& name);

			GameObjectHandle CreateEntity(const String &entity);
			GameObjectHandle CreateEntity(const String &entity,
				const String &name);

// 			void Rename(const GameObjectHandle &hObj,
// 				const String &name);

			void Destroy(const GameObjectHandle &hObj);

			inline GameObject* Get(const GameObjectHandle &hObj);

			void Rename(const GameObjectHandle &hObj, const String &name);

			GameObjectHandle Find(const String &name);

			/*void Reset(void);*/

		protected:

			inline GameObjectHandle CreateHandle(void);
			inline void FreeHandle(const GameObjectHandle &hObj);

			typedef std::unordered_map<GameObjectHandle, GameObject*, GameObjectHandleIDHasher> ObjectsHashMap;
			typedef std::unordered_multimap<String, GameObjectHandle> ObjectsNamesMap;

			ObjectsHashMap m_objects;
			ObjectsNamesMap m_namedObjects;

			std::list<GameObjectHandle> m_freeHandles;
			int m_lastId;

		};

	}

}

#include "GameObjectsManager.inl"