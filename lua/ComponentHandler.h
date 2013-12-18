#pragma once

#include <mye/core/GameObject.h>
#include <mye/core/Components.h>

#include <string>

namespace mye
{

	namespace lua
	{

		class ComponentHandler
		{

		public:

			ComponentHandler(
				const mye::core::GameObjectHandle &hObj,
				mye::core::Components component
				);

			mye::core::GameObjectHandle GetGameObject(void);
			mye::core::Components GetComponent(void);

		private:

			mye::core::GameObjectHandle _hObj;
			mye::core::Components _component;

		};

	}

}