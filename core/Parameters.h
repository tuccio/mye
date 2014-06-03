#pragma once

#include <mye/core/String.h>
#include <mye/math/Math.h>

#include <unordered_map>

namespace mye
{

	namespace core
	{

		class Parameters
		{

		public:

			typedef std::unordered_map<String, String>::const_iterator Iterator;

			Parameters(void);
			Parameters(const std::unordered_map<String, String> map);

			~Parameters(void);

			String GetString(const String &key) const;
			int GetInteger(const String &key) const;
			float GetFloat(const String &key) const;
			bool GetBool(const String &key) const;
			void* GetPointer(const String &key) const;

			template <typename T>
			mye::math::Matrix<T, 2, 1> GetVector2(const String &key) const;

			template <typename T>
			mye::math::Matrix<T, 3, 1> GetVector3(const String &key) const;

			template <typename T>
			mye::math::Matrix<T, 4, 1> GetVector4(const String &key) const;

			template <typename T>
			mye::math::QuaternionTempl<T> GetQuaternion(const String &key) const;

			void Add(const String &key, const String &value);
			bool Contains(const String &key) const;

			void Clear(void);
			size_t Size(void) const;

			Iterator begin(void) const;
			Iterator end(void) const;

			String& operator[] (const String &key);

		private:

			std::unordered_map<String, String> m_map;

		};

	}

}

#include "Parameters.inl"