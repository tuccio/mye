#include "Resource.h"

#include <mye/core/Resource.h>
#include <mye/core/ResourceTypeManager.h>
#include <mye/core/ResourceManager.h>

#include "ScriptResourceLoader.h"
#include "Types.h"
#include "Converters.h"

#include <boost/optional.hpp>

using namespace mye::core;
using namespace luabind;

namespace luabind
{

	template <>
	struct default_converter<boost::optional<mye::lua::ScriptResourceLoaderPointer>> :
		native_converter_base<boost::optional<mye::lua::ScriptResourceLoaderPointer>>
	{

		inline static int compute_score(lua_State *L, int index)
		{
			return (lua_type(L, index) == LUA_TNIL ||
				object_cast_nothrow<mye::lua::ScriptResourceLoaderPointer>(object(from_stack(L, index))) ? 0 : -1);
		}

		inline static boost::optional<mye::lua::ScriptResourceLoaderPointer> from(lua_State* L, int index)
		{
			return object_cast_nothrow<mye::lua::ScriptResourceLoaderPointer>(object(from_stack(L, index)));
		}

		inline static void to(lua_State* L, const boost::optional<mye::lua::ScriptResourceLoaderPointer> &o)
		{

			if (o)
			{
				luabind::object x(L, o.get());
				x.push(L);
			}
			else
			{
				lua_pushnil(L);
			}

		}


	};

};

namespace mye
{

	namespace lua
	{

		int __res_get_state(const Resource &r)
		{
			return static_cast<int>(r.GetState());
		}

		String __res_test(const Resource::ParametersList &params)
		{

			String s;

			for (auto pair : params)
			{
				s += pair.first + "," + pair.second + ";";
			}

			return s;

		}

		String sup(boost::optional<ScriptResourceLoaderPointer> x)
		{
			return (x ? x.get()->GetName() : "nil");
		}

		bool __res_load(Resource &r)
		{
			return r.Load();
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

		void BindResources(lua_State *L)
		{

			module(L)
			[

				class_<Resource>(MYE_LUA_RESOURCE).

					def("Load", &Resource::Load).
					def("Load", &__res_load).
					def("Unload", &Resource::Unload).
					def("Free", &Resource::Free).
					def("CalculateSize", &Resource::CalculateSize).

					def("GetSize", &Resource::GetSize).
					def("GetState", &__res_get_state),

				class_<ResourceLoadState>(MYE_LUA_RESOURCELOADSTATE).

					enum_("ResourceLoadStateEnum")
					[
						value("LOADED",     static_cast<int>(ResourceLoadState::LOADED)),
						value("LOADING",    static_cast<int>(ResourceLoadState::LOADING)),
						value("UNLOADING",  static_cast<int>(ResourceLoadState::UNLOADING)),
						value("FREED",      static_cast<int>(ResourceLoadState::FREED)),
						value("NOT_LOADED", static_cast<int>(ResourceLoadState::NOT_LOADED))
					],

				class_<ResourceTypeManager>(MYE_LUA_RESOURCETYPEMANAGER).

					def("CreateResource", &__res_create_resource),

				class_<ManualResourceLoader>(MYE_LUA_MANUALRESOURCELOADER),

				class_<ScriptResourceLoader, Resource>(MYE_LUA_SCRIPTRESOURCELOADER),

				class_<boost::optional<ScriptResourceLoaderPointer>>("__BoostOptionalScriptResourceLoaderPointer").

					def(constructor<ScriptResourceLoaderPointer>()).
					def(constructor<>()),

				def("sup", &sup)

			];

		}

	}

}