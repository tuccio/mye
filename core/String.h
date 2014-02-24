#pragma once

#include <cassert>
#include <cstring>
#include <string>
#include <sstream>

#include <mye/math/Geometry.h>

#define __MYE_STRING_INTMAXLEN 11
#define __MYE_STRING_UINTMAXLEN 10
#define __MYE_STRING_FLOATMAXLEN 12

namespace mye
{

	namespace core
	{

		class String
		{

		public:

			typedef size_t Position;

			static const Position Null;

			String(void);
			String(size_t length);
			String(const char *string);
			String(const String &string);

			~String(void);

			inline const char* CString(void) const;

			inline char& operator[] (Position i);
			inline const char& operator[] (Position i) const;

			inline void UpdateLength(void);
			inline size_t Length(void) const;

			inline bool IsEmpty(void) const;

			void Resize(size_t size);
			void Clear(void);

			String Flip(void) const;
			void FlipInPlace(void);

			String Repeat(unsigned int n) const;

			inline String operator+ (const String &s) const;
			inline String operator+ (const char *s) const;

			inline String operator+ (char c) const;

			inline String& operator= (const String &s);

			inline String& operator+= (const String &s);
			inline String& operator+= (char c);

			inline bool operator== (const String &s) const;
			inline bool operator!= (const String &s) const;

			inline bool operator== (const char *s) const;
			inline bool operator!= (const char *s) const;

			inline bool operator< (const String &s) const;

			inline Position FindFirst(const String &s) const;
			inline Position FindLast(const String &s) const;

		private:

			char *m_string;

			size_t m_size;
			size_t m_length;

			friend std::hash<mye::core::String>;

			friend String operator+ (const char *s1, const String &s2);

		};

		inline String operator+ (const char *s1, const String &s2);

		inline String ToString(int x);
		inline String ToString(unsigned int x);
		inline String ToString(float x);
		inline String ToString(double x);

		template <typename T>
		inline String ToString(const mye::math::Matrix<T, 3, 1> &v);

		template <typename T>
		inline String ToString(const mye::math::Quaternion<T> &q);

		template <typename T>
		inline mye::math::Matrix<T, 3, 1> ParseVector3(const String &s);

		template <typename T>
		inline mye::math::Quaternion<T> ParseQuaternion(const String &s);

		template <typename T>
		inline T ParseType(const String &s);

	}

}

#include "String.inl"