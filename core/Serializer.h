#pragma once

#include "Singleton.h"

namespace mye
{

	namespace core
	{

		template <typename T>
		class Serializer :
			public Singleton<T>
		{

		public:

			Serializer(void);
			~Serializer(void);

			virtual void SerializeOut(T *object, const std::string &file) = 0;
			virtual void SerializeIn(T *object, const std::string &file) = 0;

		};

		template <typename T>
		class Serializable
		{

		public:

			void SerializeOut(const std::string &file)
			{
				Serializer<T>::GetSingleton().SerializeOut(this, file);
			}

			void SerializeIn(const std::string &file)
			{
				Serializer<T>::GetSingleton().SerializeIn(this, file);
			}

		};

	}

}
