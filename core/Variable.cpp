#include "Variable.h"

namespace mye
{

	namespace core
	{

		Variable::Variable(void) :
			_datasize(0),
			_data(NULL),
			_type(typeid(void))
		{

		}

		Variable::~Variable(void)
		{

		}

		std::type_index Variable::GetType(void) const
		{
			return _type;
		}

	}

}