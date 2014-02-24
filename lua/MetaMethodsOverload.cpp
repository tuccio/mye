#include "MetaMethodsOverload.h"
#include "Math.h"
#include "Util.h"
#include "GameObjectHandle.h"
#include "Converters.h"

#include <mye/core/Components.h>
#include <mye/core/GameObjectsModule.h>
#include <mye/core/GameObject.h>
#include <mye/math/Math.h>

#include <lua.hpp>
#include <luabind/luabind.hpp>
#include <luabind/copy_policy.hpp>

#define VARIABLE_COMPONENT_CAST_GET_BEGIN(__Type)\
	if (type == typeid(__Type))\
		rvalue = object(L, boost::ref(static_cast<VariableComponent<__Type>*>(MYE_LUA_COMPONENT_UNWRAP(component))->Get()));

#define VARIABLE_COMPONENT_CAST_GET_BLOCK(__Type)\
	else if (type == typeid(__Type)) \
		rvalue = object(L, static_cast<VariableComponent<__Type>*>(MYE_LUA_COMPONENT_UNWRAP(component))->Get());

#define VARIABLE_COMPONENT_CAST_GET_ALIGNED(__Type)\
	else if (type == typeid(__Type)) \
		rvalue = object(L, boost::ref(static_cast<VariableComponent<__Type>*>(MYE_LUA_COMPONENT_UNWRAP(component))->Get()));

#define VARIABLE_COMPONENT_CAST_SET_BEGIN(__Type) \
	if (type == typeid(__Type))\
	{\
		auto value = object_cast<__Type>(object(from_stack(L, 3)));\
		static_cast<VariableComponent<__Type>*>(MYE_LUA_COMPONENT_UNWRAP(component))->Set(value);\
	}

#define VARIABLE_COMPONENT_CAST_SET_BLOCK(__Type) \
	else if (type == typeid(__Type))\
	{\
		auto value = object_cast<__Type>(object(from_stack(L, 3)));\
		static_cast<VariableComponent<__Type>*>(MYE_LUA_COMPONENT_UNWRAP(component))->Set(value);\
	}

#define VARIABLE_COMPONENT_CAST_SET_ALIGNED(__Type) \
	else if (type == typeid(__Type))\
	{\
		__Type aux;\
		memcpy(&aux, object_cast<__Type*>(object(from_stack(L, 3))), sizeof(__Type));\
		static_cast<VariableComponent<__Type>*>(MYE_LUA_COMPONENT_UNWRAP(component))->Set(aux);\
	}

using namespace luabind;
using namespace mye::core;
using namespace mye::math;


namespace mye
{

	namespace lua
	{

		int IndexOverload(lua_State *L)
		{

			int returnValues = 0;

			lua_pushvalue(L, lua_upvalueindex(1));
			lua_pushvalue(L, 1);
			lua_pushvalue(L, 2);

			lua_call(L, 2, 1);

			if (lua_isnil(L, 3))
			{

				const char *field = lua_tostring(L, 2);
				object obj(from_stack(L, 1));

				auto hObj = object_cast_nothrow<GameObjectHandle>(obj);

				if (hObj)
				{

					object wrappedComponent = __goh_getcomponent(hObj.get(), field);

					if (wrappedComponent)
					{

						auto component = object_cast<MYE_LUA_COMPONENT_WRAP_TYPE(Component)>(wrappedComponent);
						object rvalue = object(L, boost::ref(static_cast<VariableComponent<mye::math::Transformf>*>(component)->Get()));

						if (component)
						{

							switch (MYE_LUA_COMPONENT_UNWRAP(component)->GetComponentType())
							{

							case ComponentTypes::VARIABLE:

								{

									auto tmp = static_cast<VariableComponent<char>*>(MYE_LUA_COMPONENT_UNWRAP(component));
									std::type_index type = tmp->GetVariableType();

									VARIABLE_COMPONENT_CAST_GET_BEGIN(float)
									VARIABLE_COMPONENT_CAST_GET_BLOCK(int)
									VARIABLE_COMPONENT_CAST_GET_BLOCK(bool)
									VARIABLE_COMPONENT_CAST_GET_BLOCK(mye::core::String)
									VARIABLE_COMPONENT_CAST_GET_BLOCK(Vector3f)
									VARIABLE_COMPONENT_CAST_GET_BLOCK(Vector3i)
									VARIABLE_COMPONENT_CAST_GET_BLOCK(Transformf)

								}
								break;

							case ComponentTypes::TRANSFORM:

								rvalue = object(L, boost::ref(static_cast<VariableComponent<mye::math::Transformf>*>(MYE_LUA_COMPONENT_UNWRAP(component))->Get()));
								break;

							default:
								break;
							}

							lua_pop(L, 2);
							rvalue.push(L);
							returnValues = 1;

						}

					}
					else
					{

						object wrappedScript = __goh_getcomponent(hObj.get(), "script");

						if (wrappedScript)
						{

							auto script = object_cast<MYE_LUA_COMPONENT_WRAP_TYPE(ScriptComponent)>(wrappedScript);

							if (script)
							{
								int ref = *script->Script().GetReference();
								lua_rawgeti(L, LUA_REGISTRYINDEX, ref);
								lua_remove(L, -2);
								lua_getfield(L, -1, field);
								returnValues = 1;
							}

						}

					}

				}

			}
			else
			{
				returnValues = 1;
			}

			return returnValues;

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

				object wrappedComponent(from_stack(L, -1));
				auto component = object_cast<MYE_LUA_COMPONENT_WRAP_TYPE(Component)>(wrappedComponent);

				if (component)
				{

					switch (MYE_LUA_COMPONENT_UNWRAP(component)->GetComponentType())
					{

					case ComponentTypes::VARIABLE:
						{

							auto tmp = static_cast<VariableComponent<char>*>(MYE_LUA_COMPONENT_UNWRAP(component));
							std::type_index type = tmp->GetVariableType();

							VARIABLE_COMPONENT_CAST_SET_BEGIN(float)
							VARIABLE_COMPONENT_CAST_SET_BLOCK(int)
							VARIABLE_COMPONENT_CAST_SET_BLOCK(bool)
							VARIABLE_COMPONENT_CAST_SET_BLOCK(mye::core::String)
							VARIABLE_COMPONENT_CAST_SET_BLOCK(Vector3f)
							VARIABLE_COMPONENT_CAST_SET_BLOCK(Vector3i)
							VARIABLE_COMPONENT_CAST_SET_BLOCK(Transformf)

						}
						
						break;

// 					case COMPONENT_TRANSFORM:
// 						{
// 							auto value = object_cast<Transformf>(object(from_stack(L, 3)));
// 							static_cast<MYE_LUA_COMPONENT_WRAP_TYPE(TransformComponent)>(MYE_LUA_COMPONENT_UNWRAP(component))->Set(value);
// 							break;
// 						}

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