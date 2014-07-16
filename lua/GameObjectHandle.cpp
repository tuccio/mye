#include "GameObjectHandle.h"

#include "Alignment.h"
#include "Converters.h"
#include "MetaMethodsOverload.h"
#include "Types.h"
#include "Utils.h"

#include <mye/core/Components.h>
#include <mye/core/Game.h>

#include <mye/math/Math.h>

#include <luabind/luabind.hpp>
#include <luabind/operator.hpp>

#define LPGAMEOBJECT(__hObj) (Game::GetSingleton().GetGameObjectsModule()->Get(__hObj))

#define VARIABLE_COMPONENT_GET_BEGIN
#define VARIABLE_COMPONENT_GET_END { }

#define VARIABLE_COMPONENT_GET(__Type)\
if (type == typeid(__Type))\
	{\
	VariableComponent<__Type>* vComponent = static_cast<VariableComponent<__Type>*>(MYE_LUA_COMPONENT_UNWRAP(component)); \
	rvalue = object(L, vComponent)["value"]; \
	}\
	else

#define VARIABLE_COMPONENT_SET_BEGIN
#define VARIABLE_COMPONENT_SET_END { }

#define VARIABLE_COMPONENT_SET(__Type) \
if (type == typeid(__Type))\
	{\
	auto value = object_cast<__Type>(object(from_stack(L, 3))); \
	static_cast<VariableComponent<__Type>*>(MYE_LUA_COMPONENT_UNWRAP(component))->Set(value); \
	}\
else

using namespace luabind;
using namespace mye::core;
using namespace mye::math;

namespace mye
{

	namespace lua
	{

		int GameObjectHandleIndex(lua_State *L);
		int GameObjectHandleNewIndex(lua_State *L);

		void BindGameObjectHandle(lua_State *L)
		{

			module(L)
			[

				class_<GameObjectHandle>(MYE_LUA_GAMEOBJECTHANDLE).

				def("GetName", &__goh_getname).

				def("GetComponent", &__goh_getcomponent).
				def("AddComponent", &__goh_addcomponent).

				def("Exists", &__goh_exists).
				def("Destroy", &__goh_destroy).

				def("__tostring", &__goh_tostring).

				def(const_self == const_self).

				property("name", &__goh_getname)

			];

			// Overload the __newindex and __index methods to access components as properties

			RegisterIndexOverload<GameObjectHandle>(&GameObjectHandleIndex);
			RegisterNewIndexOverload<GameObjectHandle>(&GameObjectHandleNewIndex);

		}

		mye::core::String __goh_getname(const GameObjectHandle &hObj)
		{
			return LPGAMEOBJECT(hObj)->GetName();
		}

		object __goh_getcomponent(const GameObjectHandle &hObj, const mye::core::String &name)
		{
			Component *component = LPGAMEOBJECT(hObj)->GetComponent(name);
			return object(LPLUASTATE, MYE_LUA_COMPONENT_WRAP(component));
		}

		void __goh_addcomponent(const GameObjectHandle &hObj,
			const Component &component)
		{
			LPGAMEOBJECT(hObj)->AddComponent(component);
		}

		void __goh_destroy(const mye::core::GameObjectHandle &hObj)
		{
			Game::GetSingleton().GetGameObjectsModule()->PostDestroy(hObj);
		}

		bool __goh_exists(const mye::core::GameObjectHandle &hObj)
		{
			return LPGAMEOBJECT(hObj) != nullptr;
		}

		mye::core::String __goh_tostring(const GameObjectHandle &hObj)
		{

			mye::core::String result;
			GameObject *go = LPGAMEOBJECT(hObj);

			if (go)
			{

				mye::core::String name = go->GetName();

				if (name.IsEmpty())
				{
					result = mye::core::String("<Unnamed object [") +
						mye::core::ToString(hObj.id) +
						mye::core::String(", ") +
						mye::core::ToString(hObj.allocation) +
						mye::core::String("]>");
				}
				else
				{
					result = mye::core::String("<" + name + " [") +
						mye::core::ToString(hObj.id) +
						mye::core::String(", ") +
						mye::core::ToString(hObj.allocation) +
						mye::core::String("]>");
				}

			}
			else
			{
				result = mye::core::String("<Invalid object>");
			}

			return result;

		}

		int GameObjectHandleIndex(lua_State *L)
		{

			int returnValues = 0;

			object obj(from_stack(L, 1));
			const char *field = lua_tostring(L, 2);

			GameObjectHandle hObj = object_cast<GameObjectHandle>(obj);

			GameObject *go = Game::GetSingleton().GetGameObjectsModule()->Get(hObj);

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

						VARIABLE_COMPONENT_GET_BEGIN
						VARIABLE_COMPONENT_GET(mye::math::Real)
						VARIABLE_COMPONENT_GET(int)
						VARIABLE_COMPONENT_GET(bool)
						VARIABLE_COMPONENT_GET(String)
						VARIABLE_COMPONENT_GET(Vector3)
						VARIABLE_COMPONENT_GET(Vector3i)
						VARIABLE_COMPONENT_GET(Vector4)
						VARIABLE_COMPONENT_GET(Vector4i)
						VARIABLE_COMPONENT_GET(Quaternion)
						VARIABLE_COMPONENT_GET(GameObjectHandle)
						VARIABLE_COMPONENT_GET_END

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

					case ComponentTypes::LIGHT:

						rvalue = object(L, boost::ref(*static_cast<LightComponent*>(component)));
						break;

					default:
						break;
					}

					//__SHOWLUASTACK(L);

					rvalue.push(L);
					//lua_replace(L, 3);
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

						//lua_replace(L, 3);
						//lua_pop(L, 1);

						//__SHOWLUASTACK(L);

						returnValues = 1;

					}

				}

			}

			return returnValues;

		}

		int GameObjectHandleNewIndex(lua_State *L)
		{

			LuaStackCleaner stackCleaner(L);
			Component *component = nullptr;

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

				VARIABLE_COMPONENT_SET_BEGIN
				VARIABLE_COMPONENT_SET(mye::math::Real)
				VARIABLE_COMPONENT_SET(int)
				VARIABLE_COMPONENT_SET(bool)
				VARIABLE_COMPONENT_SET(String)
				VARIABLE_COMPONENT_SET(Vector3)
				VARIABLE_COMPONENT_SET(Vector3i)
				VARIABLE_COMPONENT_SET(Vector4)
				VARIABLE_COMPONENT_SET(Vector4i)
				VARIABLE_COMPONENT_SET(Quaternion)
				VARIABLE_COMPONENT_SET(GameObjectHandle)
				VARIABLE_COMPONENT_SET_END

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

			case ComponentTypes::LIGHT:

			{

				auto value = object_cast<LightComponent>(object(from_stack(L, 3)));
				*static_cast<LightComponent*>(component) = value;

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

			return 0;

		}

	}

}