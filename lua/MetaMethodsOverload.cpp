#include "MetaMethodsOverload.h"
#include "Math.h"
#include "Utils.h"
#include "GameObjectHandle.h"
#include "Converters.h"

#include <mye/core/Components.h>
#include <mye/core/Game.h>
#include <mye/core/GameObjectsModule.h>
#include <mye/core/GameObject.h>
#include <mye/math/Math.h>

#include <lua.hpp>
#include <luabind/luabind.hpp>
#include <luabind/copy_policy.hpp>
#include <luabind/shared_ptr_converter.hpp>

#include "Debug.h"

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

			//__SHOWLUASTACK(L);

			int returnValues = 0;

			lua_pushvalue(L, lua_upvalueindex(1));
			lua_pushvalue(L, 1);
			lua_pushvalue(L, 2);

			lua_call(L, 2, 1);

			if (lua_isnil(L, 3))
			{

				const char *field = lua_tostring(L, 2);

				if (strncmp(field, "__", 2)) // Ignore metamethods like __finalize, __init, ...
				{

					object obj(from_stack(L, 1));

					auto hObj = object_cast_nothrow<GameObjectHandle>(obj);

					if (hObj)
					{

						GameObject *go = Game::GetSingleton().GetGameObjectsModule()->Get(hObj.get());

						if (go)
						{

							Component *component = go->GetComponent(field);

							object rvalue;

							if (component)
							{

								switch (component->GetComponentType())
								{

								case ComponentTypes::VARIABLE:

									{

										auto tmp = static_cast<VariableComponent<void*>*>(component);
										std::type_index type = tmp->GetVariableType();

										VARIABLE_COMPONENT_CAST_GET_BEGIN(float)
										VARIABLE_COMPONENT_CAST_GET_BLOCK(int)
										VARIABLE_COMPONENT_CAST_GET_BLOCK(bool)
										VARIABLE_COMPONENT_CAST_GET_BLOCK(String)
										VARIABLE_COMPONENT_CAST_GET_BLOCK(Vector3f)
										VARIABLE_COMPONENT_CAST_GET_BLOCK(Vector3i)
										VARIABLE_COMPONENT_CAST_GET_BLOCK(Vector4f)
										VARIABLE_COMPONENT_CAST_GET_BLOCK(Vector4i)
										VARIABLE_COMPONENT_CAST_GET_BLOCK(Quaternionf)
										VARIABLE_COMPONENT_CAST_GET_BLOCK(GameObjectHandle)

									}
									break;

								case ComponentTypes::TRANSFORM:

									rvalue = object(L, boost::ref(*static_cast<TransformComponent*>(component)));
									break;

								case ComponentTypes::RIGIDBODY:

									rvalue = object(L, boost::ref(*static_cast<RigidBodyComponent*>(component)));
									break;

								case ComponentTypes::TEXT2D:

									rvalue = object(L, boost::ref(*static_cast<Text2DComponent*>(component)));
									break;

								case ComponentTypes::RENDER:

									rvalue = object(L, boost::ref(*static_cast<RenderComponent*>(component)));
									break;

								default:
									break;
								}

								//__SHOWLUASTACK(L);

								rvalue.push(L);
								lua_replace(L, 3);
								//lua_pop(L, 2);
								returnValues = 1;

								//__SHOWLUASTACK(L);

							}
							else
							{

								BehaviourComponent *behaviour = go->GetBehaviourComponent();

								if (behaviour)
								{								

									//__SHOWLUASTACK(L);

									int ref = behaviour->GetScript()->GetRegistryReference();
									lua_rawgeti(L, LUA_REGISTRYINDEX, ref);
									lua_getfield(L, -1, field);

									lua_replace(L, 3);
									lua_pop(L, 1);

									//__SHOWLUASTACK(L);

									returnValues = 1;

								}

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

			int top = lua_gettop(L);

			object obj1(from_stack(L, 1));

			const char *s = lua_tostring(L, 2);

			Component *component = nullptr;

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
				component = object_cast<Component*>(wrappedComponent);

				switch (component->GetComponentType())
				{

				case ComponentTypes::VARIABLE:
					{

						auto tmp = static_cast<VariableComponent<char>*>(component);
						std::type_index type = tmp->GetVariableType();

						VARIABLE_COMPONENT_CAST_SET_BEGIN(float)
						VARIABLE_COMPONENT_CAST_SET_BLOCK(int)
						VARIABLE_COMPONENT_CAST_SET_BLOCK(bool)
						VARIABLE_COMPONENT_CAST_SET_BLOCK(String)
						VARIABLE_COMPONENT_CAST_SET_BLOCK(Vector3f)
						VARIABLE_COMPONENT_CAST_SET_BLOCK(Vector3i)
						VARIABLE_COMPONENT_CAST_SET_BLOCK(Vector4f)
						VARIABLE_COMPONENT_CAST_SET_BLOCK(Vector4i)
						VARIABLE_COMPONENT_CAST_SET_BLOCK(Quaternionf)
						VARIABLE_COMPONENT_CAST_SET_BLOCK(GameObjectHandle)

					}

					break;

				case ComponentTypes::TRANSFORM:

					{

						auto value = object_cast<TransformComponent>(object(from_stack(L, 3)));
						*static_cast<TransformComponent*>(component) = value;

						break;
					}

				case ComponentTypes::RIGIDBODY:

					{

						auto value = object_cast<RigidBodyComponent>(object(from_stack(L, 3)));
						*static_cast<RigidBodyComponent*>(component) = value;

						break;

					}

				case ComponentTypes::TEXT2D:

					{

						auto value = object_cast<Text2DComponent>(object(from_stack(L, 3)));
						*static_cast<Text2DComponent*>(component) = value;

						break;

					}

				case ComponentTypes::RENDER:

					{

						auto value = object_cast<RenderComponent>(object(from_stack(L, 3)));
						*static_cast<RenderComponent*>(component) = value;

						break;

					}

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
			else
			{

				//lua_pop(L, 2);

				lua_pushvalue(L, lua_upvalueindex(1));
				lua_pushvalue(L, 1);
				lua_pushvalue(L, 2);
				lua_pushvalue(L, 3);

				lua_call(L, 3, 0);


			}

			lua_settop(L, top);

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