#include "GameObjectHandle.h"

#include "Types.h"
#include "Util.h"
#include "MetaMethodsOverload.h"

#include <mye/core/Components.h>

#include <Eigen/Eigen>
#include <luabind/luabind.hpp>

#define LPGAMEOBJECT(__hObj) (Game::GetSingleton().GetGameObjectsModule()->Get(__hObj))

using namespace mye::core;
using namespace luabind;

namespace mye
{

	namespace lua
	{

		void BindGameObjectHandle(lua_State *L)
		{

			module(L)
			[
				class_<Component>("Component")
			];

			module(L)
			[

				class_<GameObjectHandle>(MYE_LUA_GAMEOBJECTHANDLE).

					def("GetName", &__goh_getname).

					def("GetComponent", &__goh_getcomponent).
					def("AddComponent", &__goh_addcomponent).

					def("GetParent", &__goh_getparent).
					def("SetParent", &__goh_setparent).
					def("GetChildren", &__goh_getchildren).

					def("__tostring", &__goh_tostring)

			];

			// Overload the __newindex and __index methods to access components as properties

			OverloadLuabindMetamethod(L, typeid(GameObjectHandle), "__newindex", &NewIndexOverload);
			OverloadLuabindMetamethod(L, typeid(GameObjectHandle), "__index", &IndexOverload);

		}

		std::string __goh_getname(const GameObjectHandle &hObj)
		{
			return LPGAMEOBJECT(hObj)->GetName();
		}

		object __goh_getcomponent(const GameObjectHandle &hObj, const std::string &name)
		{

			Component *component = LPGAMEOBJECT(hObj)->GetComponent(name);

			if (component)
			{
				switch (component->GetComponentType())
				{

				case VARIABLE_COMPONENT:

					{

						VariableComponent<char> *vc = (VariableComponent<char>*) component;

						std::type_index type = vc->GetVariableType();

						lua_State *L = LPLUASTATE;

#define VARIABLE_COMPONENT_CAST_BEGIN(__Type) if (type == typeid(__Type)) return object(L, (VariableComponent<__Type>*) component);
#define VARIABLE_COMPONENT_CAST(__Type) else if (type == typeid(__Type)) return object(L, (VariableComponent<__Type>*) component);

						VARIABLE_COMPONENT_CAST_BEGIN(float)
						VARIABLE_COMPONENT_CAST(int)
						VARIABLE_COMPONENT_CAST(bool)
						VARIABLE_COMPONENT_CAST(std::string)
						VARIABLE_COMPONENT_CAST(Eigen::Vector3f)
						VARIABLE_COMPONENT_CAST(Eigen::Vector3i)

					}

					break;

				default:
					break;

				}

			}

			return object();

		}

		void __goh_addcomponent(const GameObjectHandle &hObj,
			const std::string &name,
			const Component &component)
		{
			LPGAMEOBJECT(hObj)->AddComponent(name, component);
		}

		luabind::object __goh_getparent(const GameObjectHandle &hObj)
		{

			GameObject *parent = LPGAMEOBJECT(hObj)->GetParent();

			if (parent)
			{
				return object(LPLUASTATE, parent->GetHandle());
			}
			else
			{
				return object();
			}

		}

		void __goh_setparent(const GameObjectHandle &hObj,
			const luabind::object &oHandle)
		{

			GameObject *parent;

			if (type(oHandle) != LUA_TNIL)
			{
				GameObjectHandle handle = object_cast<GameObjectHandle>(oHandle);
				parent = LPGAMEOBJECT(handle);
			}
			else
			{
				parent = NULL;
			}

			LPGAMEOBJECT(hObj)->SetParent(parent);
			

		}

		luabind::object __goh_getchildren(const GameObjectHandle &hObj)
		{

			const GameObject::ChildrenList& children = LPGAMEOBJECT(hObj)->GetChildren();

			object table = newtable(LPLUASTATE);

			int i = 1;

			for (auto child : children)
			{
				table[i++] = object(table.interpreter(), child->GetHandle());
			}

			return table;

		}

		std::string __goh_tostring(const GameObjectHandle &hObj)
		{

			std::string name = LPGAMEOBJECT(hObj)->GetName();

			if (name.empty())
			{
				return std::string("<Unnamed object [") +
					std::to_string(hObj.id) +
					std::string(", ") +
					std::to_string(hObj.allocation) +
					std::string("]>");
			}

			return name;

		}

	}

}