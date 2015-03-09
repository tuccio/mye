#pragma once

#include "GameObjectsManager.h"
#include "String.h"

#include <mye/math/Geometry.h>

#include <typeindex>
#include <type_traits>

namespace mye
{

	namespace core
	{

		enum class VariableTypes
		{
			STRING,
			BOOL,
			INT,
			REAL,
			VEC2,
			VEC2I,
			VEC3,
			VEC3I,
			VEC4,
			VEC4I,
			MAT2,
			MAT3,
			MAT4,
			QUATERNION,
			GAMEOBJECTHANDLE
		};

		template <typename T>
		struct VariableTraits { };


#define __MYE_DEFINE_VARIABLE_TRAITS(Type, Alignment, EnumType) template <> struct VariableTraits<Type> \
		{ typedef std::integral_constant<std::size_t, Alignment> alignment; typedef std::integral_constant<VariableTypes, (EnumType)> type; };

#define __MYE_DEFAULT_VARIABLE_TRAITS(Type, EnumType) __MYE_DEFINE_VARIABLE_TRAITS(Type, std::alignment_of<Type>::value, EnumType)

		__MYE_DEFAULT_VARIABLE_TRAITS(String,           VariableTypes::STRING)
		__MYE_DEFAULT_VARIABLE_TRAITS(bool,             VariableTypes::BOOL)
		__MYE_DEFAULT_VARIABLE_TRAITS(int,              VariableTypes::INT)
		__MYE_DEFAULT_VARIABLE_TRAITS(mye::math::Real,  VariableTypes::REAL)
		__MYE_DEFAULT_VARIABLE_TRAITS(GameObjectHandle, VariableTypes::GAMEOBJECTHANDLE)

		__MYE_DEFAULT_VARIABLE_TRAITS(mye::math::Vector2,  VariableTypes::VEC2)
		__MYE_DEFAULT_VARIABLE_TRAITS(mye::math::Vector2i, VariableTypes::VEC2I)

		__MYE_DEFINE_VARIABLE_TRAITS(mye::math::Vector3,  16, VariableTypes::VEC3)
		__MYE_DEFINE_VARIABLE_TRAITS(mye::math::Vector3i, 16, VariableTypes::VEC3I)

		__MYE_DEFINE_VARIABLE_TRAITS(mye::math::Vector4,  16, VariableTypes::VEC4)
		__MYE_DEFINE_VARIABLE_TRAITS(mye::math::Vector4i, 16, VariableTypes::VEC4I)

		__MYE_DEFINE_VARIABLE_TRAITS(mye::math::Quaternion, 16, VariableTypes::QUATERNION)

	}

}