cmake_minimum_required(VERSION 3.0)

project (myelua)

option ( LUA_32BITS "Check if lua and lua++11 were built with the LUA_32BITS flag" ON )

if ( LUA_32BITS )
	add_definitions ( -DLUA_32BITS )
endif ( )

add_library ( myelua BehaviourScript.cpp BehaviourScript.h Bullet.cpp Bullet.h Converters.h Debug.h DirectX11.cpp DirectX11.h Game.cpp Game.h GameObjectHandle.cpp GameObjectHandle.h InputModule.cpp InputModule.h LuaModule.cpp LuaModule.h Math.cpp Math.h MathWrappers.h MathWrappers.inl MetaMethodsOverload.cpp MetaMethodsOverload.h Physics.cpp Physics.h ProcedureScript.cpp ProcedureScript.h Resource.cpp Resource.h Scene.cpp Scene.h Scripting.cpp Scripting.h ScriptResourceLoader.cpp ScriptResourceLoader.h Types.h Utils.cpp Utils.h VariableComponent.cpp VariableComponent.h WindowsFunctions.cpp WindowsFunctions.h )
