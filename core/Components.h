#pragma once

#include "NullComponent.h"
#include "VariableComponent.h"
#include "TransformComponent.h"

namespace mye
{

	namespace core
	{

		enum Components
		{
			NULL_COMPONENT = 0,
			NUMBER_COMPONENT,
			TRANSFORM_COMPONENT,
			COMPONENTS_N
		};

		const char* COMPONENT_NAMES[COMPONENTS_N] = {
			NullComponent::Name(),
			VariableComponent::Name(),
			TransformComponent::Name()
		};

	}

}