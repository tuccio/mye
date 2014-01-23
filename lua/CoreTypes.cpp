#include "CoreTypes.h"
#include "Types.h"

#include <mye/core/ColorRGB.h>
#include <mye/core/ColorRGBA.h>

#include <luabind/luabind.hpp>
#include <luabind/operator.hpp>

using namespace mye::core;
using namespace luabind;

namespace mye
{

	namespace lua
	{

		void BindCoreTypes(lua_State *L)
		{

			module(L)
			[

				class_<ColorRGB>(MYE_LUA_COLORRGB).

					def(constructor<>()).
					def(constructor<float, float, float>()).

					property("r", &__color_rgb_get_r, &__color_rgb_set_r).
					property("g", &__color_rgb_get_g, &__color_rgb_set_g).
					property("b", &__color_rgb_get_b, &__color_rgb_set_b),


				class_<ColorRGBA>(MYE_LUA_COLORRGBA).

					def(constructor<>()).
					def(constructor<float, float, float, float>()).

					def(const_self + const_self).

					property("r", &__color_rgba_get_r, &__color_rgba_set_r).
					property("g", &__color_rgba_get_g, &__color_rgba_set_g).
					property("b", &__color_rgba_get_b, &__color_rgba_set_b).
					property("a", &__color_rgba_get_a, &__color_rgba_set_a)

			];

		}

		float __color_rgb_get_r(const ColorRGB &color)
		{
			return color.r();
		}

		float __color_rgb_get_g(const ColorRGB &color)
		{
			return color.g();
		}

		float __color_rgb_get_b(const ColorRGB &color)
		{
			return color.b();
		}

		float __color_rgba_get_r(const ColorRGBA &color)
		{
			return color.r();
		}

		float __color_rgba_get_g(const ColorRGBA &color)
		{
			return color.g();
		}

		float __color_rgba_get_b(const ColorRGBA &color)
		{
			return color.b();
		}

		float __color_rgba_get_a(const ColorRGBA &color)
		{
			return color.a();
		}

		void __color_rgb_set_r(ColorRGB &color, float v)
		{
			color.r() = v;
		}

		void __color_rgb_set_g(ColorRGB &color, float v)
		{
			color.g() = v;
		}

		void __color_rgb_set_b(ColorRGB &color, float v)
		{
			color.b() = v;
		}

		void __color_rgba_set_r(ColorRGBA &color, float v)
		{
			color.r() = v;
		}

		void __color_rgba_set_g(ColorRGBA &color, float v)
		{
			color.g() = v;
		}

		void __color_rgba_set_b(ColorRGBA &color, float v)
		{
			color.b() = v;
		}

		void __color_rgba_set_a(ColorRGBA &color, float v)
		{
			color.a() = v;
		}

	}

}