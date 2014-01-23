#include <lua.hpp>

#include <mye/core/ColorRGB.h>
#include <mye/core/ColorRGBA.h>

namespace mye
{

	namespace lua
	{

		void BindCoreTypes(lua_State *L);

		float __color_rgb_get_r(const mye::core::ColorRGB &color);
		float __color_rgb_get_g(const mye::core::ColorRGB &color);
		float __color_rgb_get_b(const mye::core::ColorRGB &color);

		float __color_rgba_get_r(const mye::core::ColorRGBA &color);
		float __color_rgba_get_g(const mye::core::ColorRGBA &color);
		float __color_rgba_get_b(const mye::core::ColorRGBA &color);
		float __color_rgba_get_a(const mye::core::ColorRGBA &color);

		void __color_rgb_set_r(mye::core::ColorRGB &color, float v);
		void __color_rgb_set_g(mye::core::ColorRGB &color, float v);
		void __color_rgb_set_b(mye::core::ColorRGB &color, float v);

		void __color_rgba_set_r(mye::core::ColorRGBA &color, float v);
		void __color_rgba_set_g(mye::core::ColorRGBA &color, float v);
		void __color_rgba_set_b(mye::core::ColorRGBA &color, float v);
		void __color_rgba_set_a(mye::core::ColorRGBA &color, float v);

	}

}