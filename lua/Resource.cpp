#include "Resource.h"

#include <mye/core/Resource.h>
#include <mye/core/ResourceTypeManager.h>
#include <mye/core/ResourceManager.h>
#include <mye/core/Parameters.h>
#include <mye/core/Font.h>

#include "ScriptResourceLoader.h"
#include "Types.h"
#include "Converters.h"

#include <boost/optional.hpp>
#include <boost/bind.hpp>

using namespace mye::core;

//namespace luabind
//{
//
//	template <>
//	struct default_converter<boost::optional<mye::lua::ScriptResourceLoaderPointer>> :
//		native_converter_base<boost::optional<mye::lua::ScriptResourceLoaderPointer>>
//	{
//
//		inline static int compute_score(lua_State *L, int index)
//		{
//			return (lua_type(L, index) == LUA_TNIL ||
//				object_cast_nothrow<mye::lua::ScriptResourceLoaderPointer>(object(from_stack(L, index))) ? 0 : -1);
//		}
//
//		inline static boost::optional<mye::lua::ScriptResourceLoaderPointer> from(lua_State* L, int index)
//		{
//			return object_cast_nothrow<mye::lua::ScriptResourceLoaderPointer>(object(from_stack(L, index)));
//		}
//
//		inline static void to(lua_State* L, const boost::optional<mye::lua::ScriptResourceLoaderPointer> &o)
//		{
//
//			if (o)
//			{
//				luabind::object x(L, o.get());
//				x.push(L);
//			}
//			else
//			{
//				lua_pushnil(L);
//			}
//
//		}
//
//
//	};
//
//};

using namespace luapp11;

namespace mye
{

	namespace lua
	{

		int __res_get_state(const Resource &r)
		{
			return static_cast<int>(r.GetState());
		}

		String __res_test(const Parameters &params)
		{

			String s;

			for (auto pair : params)
			{
				s += pair.first + "," + pair.second + ";";
			}

			return s;

		}

		bool __res_load(Resource &r)
		{
			return r.Load();
		}

		void __res_unload(Resource &r)
		{
			r.Unload();
		}

		ResourcePointer __res_create_resource(
			ResourceTypeManager &rtm,
			const mye::core::String &type,
			const mye::core::String &name,
			boost::optional<ScriptResourceLoaderPointer> manual,
			std::unordered_map<String, String> params)
		{
			return rtm.CreateResource(type, name, (manual ? manual.get().get() : nullptr), params);
		}

		void BindResources(State state)
		{

			state
			[

				Class<Parameters>(MYE_LUA_PARAMETERS),

				Enum<ResourceLoadState>(MYE_LUA_RESOURCELOADSTATE)

					("LOADED",     ResourceLoadState::LOADED)
					("LOADING",    ResourceLoadState::LOADING)
					("UNLOADING",  ResourceLoadState::UNLOADING)
					("FREED",      ResourceLoadState::FREED)
					("NOT_LOADED", ResourceLoadState::NOT_LOADED),

				Class<Resource>(MYE_LUA_RESOURCE).

					Function("__tostring", &Resource::GetName).

					Function("Load", &Resource::Load).
					Function("Load", __res_load).

					Function("Unload", &Resource::Unload).
					Function("Unload", &__res_unload).

					Function("Free", &Resource::Free).
					Function("CalculateSize", &Resource::CalculateSize).

					Property("size",  &Resource::GetSize).
					Property("state", &Resource::GetState).
					Property("name",  &Resource::GetName),

				Class<ResourceTypeManager>(MYE_LUA_RESOURCETYPEMANAGER),

				Class<ManualResourceLoader>(MYE_LUA_MANUALRESOURCELOADER),

				Class<ScriptResourceLoader, Resource>(MYE_LUA_SCRIPTRESOURCELOADER).
					Constructor<const String &>(),

				Class<Font, Resource>(MYE_LUA_FONT)

			];

			//module(L)
			//[

			//	class_<Parameters>(MYE_LUA_PARAMETERS).

			//		def(constructor<const std::unordered_map<String, String>&>()).

			//		def("Add", &Parameters::Add).
			//		def("Contains", &Parameters::Contains),

			//	class_<Resource>(MYE_LUA_RESOURCE).

			//		def("__tostring", &Resource::GetName).

			//		def("Load", &Resource::Load).
			//		def("Load", &__res_load).
			//		//def("Load", luabind::make_function(L, boost::bind(&Resource::Load, _1, false))).

			//		def("Unload", &Resource::Unload).
			//		def("Unload", &__res_unload).
			//		//def("Unload", luabind::make_function(L, boost::bind(&Resource::Unload, _1, false))).

			//		def("Free", &Resource::Free).
			//		def("CalculateSize", &Resource::CalculateSize).

			//		property("size", &Resource::GetSize).
			//		property("state", &__res_get_state).
			//		property("name", &Resource::GetName),

			//	class_<ResourceLoadState>(MYE_LUA_RESOURCELOADSTATE).

			//		enum_("ResourceLoadStateEnum")
			//		[
			//			value("LOADED",     static_cast<int>(ResourceLoadState::LOADED)),
			//			value("LOADING",    static_cast<int>(ResourceLoadState::LOADING)),
			//			value("UNLOADING",  static_cast<int>(ResourceLoadState::UNLOADING)),
			//			value("FREED",      static_cast<int>(ResourceLoadState::FREED)),
			//			value("NOT_LOADED", static_cast<int>(ResourceLoadState::NOT_LOADED))
			//		],

			//	class_<ResourceTypeManager>(MYE_LUA_RESOURCETYPEMANAGER).

			//		def("CreateResource", &__res_create_resource).
			//		def("GetResource", &ResourceTypeManager::GetResource),

			//	class_<ManualResourceLoader>(MYE_LUA_MANUALRESOURCELOADER),

			//	class_<ScriptResourceLoader, Resource>(MYE_LUA_SCRIPTRESOURCELOADER),

			//	class_<boost::optional<ScriptResourceLoaderPointer>>("__BoostOptionalScriptResourceLoaderPointer").

			//		def(constructor<ScriptResourceLoaderPointer>()).
			//		def(constructor<>()),

			//	class_<Font, Resource>(MYE_LUA_FONT)

			//];

		}

	}

}