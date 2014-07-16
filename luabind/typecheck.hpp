#ifndef LUABIND_MYE_TYPECHECK_HPP
#define LUABIND_MYE_TYPECHECK_HPP

#include <luabind/object.hpp>

namespace luabind
{

	template<class T, class ValueWrapper>
	bool object_typecheck(ValueWrapper const& value_wrapper)
	{
		return detail::object_typecheck_aux(
			value_wrapper
			, (T*)0
			, (detail::null_type*)0
			);
	}

	template<class T, class ValueWrapper, class Policies>
	bool object_typecheck(ValueWrapper const& value_wrapper, Policies const&)
	{
		return detail::object_typecheck_aux(
			value_wrapper
			, (T*)0
			, (Policies*)0
			);
	}

	namespace detail
	{

		template<
			class T
				, class ValueWrapper
				, class Policies
		>
		bool object_typecheck_aux(
		ValueWrapper const& value_wrapper
		, T*
		, Policies*
		)
			{
			lua_State* interpreter =
				value_wrapper_traits<ValueWrapper>::interpreter(
				value_wrapper
				);

			if (!interpreter)
				return false;


			value_wrapper_traits<ValueWrapper>::unwrap(interpreter, value_wrapper);

			detail::stack_pop pop(interpreter, 1);

			typedef typename detail::find_conversion_policy<
				0
				, Policies
			>::type converter_generator;

			typename mpl::apply_wrap2<converter_generator, T,
				lua_to_cpp>::type cv;

			if (cv.match(interpreter, LUABIND_DECORATE_TYPE(T),
				-1) < 0)
			{
				return false;
			}

			return true;
		}
	}
}

#endif