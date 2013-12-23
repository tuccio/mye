#pragma once

#include <typeindex>

namespace mye
{

	namespace core
	{

		class Variable
		{

		public:

			Variable(void);
			~Variable(void);

			std::type_index GetType(void) const;

			void SetNull(void);

			template <typename T>
			void Set(T x);

			template <typename T>
			T Get(void) const;

		private:

			typedef size_t SizeType;

			void Clear(void);

			std::type_index _type;	
			void *_data;
			SizeType _datasize;

		};

		template <typename T>
		void Variable::Set(T x)
		{

			SizeType newsize = sizeof(T);

			if (newsize > _datasize)
			{

				if (_datasize > 0)
				{
					delete _data;
				}

				_data = new T;
				_datasize = newsize;

			}

			_type = typeid(T);
			memcpy(_data, &x, newsize);

		}

		template <typename T>
		T Variable::Get(void) const
		{
			return * (T*) _data;
		}

	}

}