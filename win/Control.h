#pragma once

#include <mye/math/Math.h>
#include "IDGenerator.h"

namespace mye
{

	namespace win
	{

		enum ControlType
		{
			CT_BUTTON,
			CT_LISTVIEW,
			CT_EDIT,
			CT_CHECKBOX
		};

		class Control
		{

		public:

			Control(ControlType type);
			~Control(void);

			virtual void Destroy(void);

			virtual void Show(void);
			virtual void Hide(void);

			virtual void SetPosition(const mye::math::Vector2i &position);
			virtual void SetSize(const mye::math::Vector2i &size);

			virtual bool Notify(unsigned int code, LPARAM lParam);

			IDGenerator::ID GetID(void) const;
			ControlType GetControlType(void) const;

		protected:
			
			IDGenerator::ID m_id;
			ControlType m_controlType;

		};

	}

}

