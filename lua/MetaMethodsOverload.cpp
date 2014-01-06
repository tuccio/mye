#include "MetaMethodsOverload.h"
#include "Util.h"
#include "GameObjectHandle.h"

#include <mye/core/Components.h>
#include <mye/core/GameObjectsModule.h>
#include <mye/core/GameObject.h>
#include <Eigen/Eigen>

#include <lua.hpp>
#include <luabind/luabind.hpp>
#include <luabind/copy_policy.hpp>

using namespace luabind;
using namespace mye::core;

namespace mye
{

	namespace lua
	{

		int IndexOverload(lua_State *L)
		{

			lua_pushvalue(L, lua_upvalueindex(1));
			lua_pushvalue(L, 1);
			lua_pushvalue(L, 2);

			lua_call(L, 2, 1);

			if (lua_isnil(L, 3))
			{

				const char *field = lua_tostring(L, 2);
				object obj(from_stack(L, 1));

				if (object_cast_nothrow<GameObjectHandle>(obj))
				{

					object r = __goh_getcomponent(obj, field);

					if (r)
					{

						auto component = object_cast<Component*>(r);
						object rvalue;

						if (component)
						{

							switch (component->GetComponentType())
							{

							case VARIABLE_COMPONENT:
								{

									VariableComponent<char> *tmp = (VariableComponent<char>*) component;
									auto type = tmp->GetVariableType();

#define VARIABLE_COMPONENT_CAST_BEGIN(__Type) if (type == typeid(__Type)) rvalue = object(L, ((VariableComponent<__Type>*) component)->Get());
#define VARIABLE_COMPONENT_CAST(__Type) else if (type == typeid(__Type)) rvalue = object(L, ((VariableComponent<__Type>*) component)->Get());

									VARIABLE_COMPONENT_CAST_BEGIN(float)
									VARIABLE_COMPONENT_CAST(int)
									VARIABLE_COMPONENT_CAST(bool)
									VARIABLE_COMPONENT_CAST(std::string)
									VARIABLE_COMPONENT_CAST(Eigen::Vector3f)
									VARIABLE_COMPONENT_CAST(Eigen::Vector3i)

#undef VARIABLE_COMPONENT_CAST_BEGIN
#undef VARIABLE_COMPONENT_CAST

								}

								break;
							default:
								break;
							}

							lua_pop(L, 2);
							rvalue.push(L);

						}

					}

				}

			}

			return 1;

		}

		int NewIndexActualOverload(lua_State *L)
		{

			int n = lua_gettop(L);

			return 0;

		}

		int NewIndexOverload(lua_State *L)
		{

			object obj1(from_stack(L, 1));

			const char *s = lua_tostring(L, 2);

			if (object_cast_nothrow<GameObjectHandle>(obj1))
			{

				auto reg = detail::class_registry::get_registry(L);
				auto regentry = reg->find_class(typeid(GameObjectHandle));

				regentry->get_table(L);

				lua_getfield(L, -1, "GetComponent");
				lua_pushvalue(L, 1);
				lua_pushvalue(L, 2);

				lua_call(L, 2, 1);

				object cObj(from_stack(L, -1));
				auto component = object_cast<Component*>(cObj);

				if (component)
				{

					switch (component->GetComponentType())
					{
					case VARIABLE_COMPONENT:
						{

							VariableComponent<char> *tmp = (VariableComponent<char>*) component;
							auto type = tmp->GetVariableType();

#define VARIABLE_COMPONENT_CAST_BEGIN(__Type) if (type == typeid(__Type)) { auto value = object_cast<__Type>(object(from_stack(L, 3))); ((VariableComponent<__Type>*) component)->Set(value); }
#define VARIABLE_COMPONENT_CAST(__Type) else if (type == typeid(__Type)) { auto value = object_cast<__Type>(object(from_stack(L, 3))); ((VariableComponent<__Type>*) component)->Set(value); }

							VARIABLE_COMPONENT_CAST_BEGIN(float)
							VARIABLE_COMPONENT_CAST(int)
							VARIABLE_COMPONENT_CAST(bool)
							VARIABLE_COMPONENT_CAST(std::string)
							VARIABLE_COMPONENT_CAST(Eigen::Vector3f)
							VARIABLE_COMPONENT_CAST(Eigen::Vector3i)

#undef VARIABLE_COMPONENT_CAST_BEGIN
#undef VARIABLE_COMPONENT_CAST

						}
						
						break;
					default:
						break;
					}

				}

				lua_pop(L, 2);

			}
			else
			{

				lua_pushvalue(L, lua_upvalueindex(1));
				lua_pushvalue(L, 1);
				lua_pushvalue(L, 2);
				lua_pushvalue(L, 3);

				lua_call(L, 3, 0);

			}

			return 0;

		}

		bool OverloadLuabindMetamethod(lua_State *L,
			const std::type_info &classtype,
			const char *metamethod,
			int (*f) (lua_State*))
		{

			using namespace luabind;

			auto reg = detail::class_registry::get_registry(L);
			auto regentry = reg->find_class(classtype);

			if (regentry)
			{

				int ref = regentry->metatable_ref();

				// Stack : -

				lua_pushinteger(L, ref);
				lua_gettable(L, LUA_REGISTRYINDEX);

				// Stack : metatable

				lua_pushstring(L, metamethod);

				lua_pushstring(L, metamethod);
				lua_rawget(L, -3);

				// Stack : metatable metamethod function

				lua_pushcclosure(L, f, 1);

				// Stack : metatable metamethod closure

				lua_rawset(L, -3);

				return true;

			}

			return false;

		}
	}
}