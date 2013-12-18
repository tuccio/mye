#include <luabind/luabind.hpp>
#include <luabind/operator.hpp>

#include <luabind/return_reference_to_policy.hpp>

#include <mye/math/Vector.h>

namespace mye
{
	
	namespace lua
	{

		int RegisterVector(lua_State *L)
		{

			using namespace mye::math;
			using namespace luabind;
			
			open(L);

			module(L)
			[
				class_<Vector3f>("vec3")
					.def(constructor<>())
					.def(constructor<float, float, float>())
					.def(constructor<float>())
					.def(constructor<const Vector3f>())
					.def(self + other<Vector3f>())
					.property("x", &Vector3f::GetX, &Vector3f::SetX)
					.property("y", &Vector3f::GetY, &Vector3f::SetY)
					.property("z", &Vector3f::GetZ, &Vector3f::SetZ)
			];

			return 0;

		}

	}

}