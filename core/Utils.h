#pragma once

#include "String.h"
#include <vector>

#include <boost/preprocessor/repeat.hpp>
#include <boost/preprocessor/variadic/to_list.hpp>
#include <boost/preprocessor/comma_if.hpp>

#define __MYE_INITIALIZE_VALUES_IMPL_2(ELEM, VALUE) ELEM ( VALUE ) 
#define __MYE_INITIALIZE_VALUES_IMPL(R, VALUE, I, ELEM) __MYE_INITIALIZE_VALUES_IMPL_2(ELEM, VALUE) BOOST_PP_COMMA_IF(BOOST_PP_NOT_EQUAL(I, R))
#define __MYE_INITIALIZE_VALUES(VALUE, ...) BOOST_PP_LIST_FOR_EACH_I(__MYE_INITIALIZE_VALUES_IMPL, VALUE, BOOST_PP_VARIADIC_TO_LIST( __VA_ARGS__ ))

namespace mye
{

	namespace core
	{

		String PointerToString(const void *p);
		void* StringToPointer(const String &str);

		void RuntimeError(const String &error);

		std::vector<String> SplitCommandLine(const String &cmd);

	}
}