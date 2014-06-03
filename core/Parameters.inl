namespace mye
{

	namespace core
	{

		template <typename T>
		mye::math::Matrix<T, 2, 1> Parameters::GetVector2(const String &key) const
		{

			String s = GetString(key);

			mye::math::Matrix<T, 2, 1> value = 0;

			if (!s.IsEmpty())
			{
				value = ParseVector2<T>(s);
			}

			return value;

		}

		template <typename T>
		mye::math::Matrix<T, 3, 1> Parameters::GetVector3(const String &key) const
		{

			String s = GetString(key);

			mye::math::Matrix<T, 3, 1> value = 0;

			if (!s.IsEmpty())
			{
				value = ParseVector3<T>(s);
			}

			return value;

		}

		template <typename T>
		mye::math::Matrix<T, 4, 1> Parameters::GetVector4(const String &key) const
		{

			String s = GetString(key);

			mye::math::Matrix<T, 4, 1> value = 0;

			if (!s.IsEmpty())
			{
				value = ParseVector4<T>(s);
			}

			return value;

		}

		template <typename T>
		mye::math::QuaternionTempl<T> Parameters::GetQuaternion(const String &key) const
		{

			String s = GetString(key);

			mye::math::Quaternion<T> value = 0;

			if (!s.IsEmpty())
			{
				value = ParseQuaternion<T>(s);
			}

			return value;

		}

	}

}