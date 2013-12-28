#include "GameObjectHandle.h"
#include "Types.h"
#include "Util.h"
#include "MetaMethodsOverload.h"

#include <mye/core/Components.h>

#include <Eigen/Eigen>
#include <luabind/luabind.hpp>

#define LPGAMEOBJECT(__L, __hObj) (object_cast<GameObjectsModule>(globals(__L)["GameObjects"]).Get(__hObj))

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
					def("AddComponent", &__goh_addcomponent)

			];

			// Overload the __newindex and __index methods to access components as properties

			OverloadLuabindMetamethod(L, typeid(GameObjectHandle), "__newindex", &NewIndexOverload);
			OverloadLuabindMetamethod(L, typeid(GameObjectHandle), "__index", &IndexOverload);

		}

		std::string __goh_getname(const object &o)
		{
			GameObjectHandle hObj = object_cast<GameObjectHandle>(o);
			return LPGAMEOBJECT(o.interpreter(), hObj)->GetName();
		}

		object __goh_getcomponent(const object &o, const std::string &name)
		{

			GameObjectHandle hObj = object_cast<GameObjectHandle>(o);
			Component *component = LPGAMEOBJECT(o.interpreter(), hObj)->GetComponent(name);

			if (component)
			{
				switch (component->GetComponentType())
				{

				case VARIABLE_COMPONENT:

					{

						VariableComponent<char> *vc = (VariableComponent<char>*) component;

						std::type_index type = vc->GetVariableType();

#define VARIABLE_COMPONENT_CAST_BEGIN(__Type) if (type == typeid(__Type)) return object(o.interpreter(), (VariableComponent<__Type>*) component);
#define VARIABLE_COMPONENT_CAST(__Type) else if (type == typeid(__Type)) return object(o.interpreter(), (VariableComponent<__Type>*) component);

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

		void __goh_addcomponent(const object &o, const std::string &name, const Component &component)
		{
			GameObjectHandle hObj = object_cast<GameObjectHandle>(o);
			LPGAMEOBJECT(o.interpreter(), hObj)->AddComponent(name, component);
		}

	}

}