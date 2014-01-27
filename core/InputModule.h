#pragma once

#include "Module.h"

#include "Mouse.h"
#include "Keyboard.h"

namespace mye
{

	namespace core
	{

		class InputModule : public Module
		{

		public:

			virtual bool HasKeyboard(void) const;
			virtual Keyboard* GetKeyboard(void);
			virtual const Keyboard* GetKeyboard(void) const;

			virtual bool HasMouse(void) const;
			virtual Mouse* GetMouse(void);
			virtual const Mouse* GetMouse(void) const;

			virtual void ResetDeltas(void);

		};

	}

}