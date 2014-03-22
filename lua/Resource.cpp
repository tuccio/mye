#include "Resource.h"

#include <mye/core/Resource.h>
#include <mye/core/ResourceTypeManager.h>
#include <mye/core/ResourceManager.h>

#include "ScriptResourceLoader.h"
#include "Types.h"
#include "Converters.h"

using namespace mye::core;
using namespace luabind;

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

		bool __res_load(Resource &r)
		{
			return r.Load();
		}

		ResourcePointer __res_create_resource(
			const mye::core::String &type,
			const mye::core::String &name);

		void BindResources(lua_State *L)
		{

			module(L)
			[

				def("test", &__res_test),

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

					def("CreateResource", &ResourceTypeManager::CreateResource),

				class_<ManualResourceLoader>(MYE_LUA_MANUALRESOURCELOADER),

				class_<ScriptResourceLoader, Resource>(MYE_LUA_SCRIPTRESOURCELOADER)

			];

		}

	}

}