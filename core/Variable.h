#pragma once

#include <Eigen/Eigen>

namespace mye
{

	namespace core
	{

		class Variable
		{

		public:

			enum Type
			{
				MYE_NULL = 0,
				MYE_FLOAT,
				MYE_VEC3,
				MYE_POINTER
			};

			Variable(void);
			~Variable(void);

			Type GetType(void);

			void SetNull(void);

			void SetFloat(float x);
			float GetFloat(void) const;

			void SetVec3(const Eigen::Vector3f &v);
			Eigen::Vector3f GetVec3(void) const;		

			void SetPointer(void *p);
			void* GetPointer(void) const;

		private:

			void Clear(void);

			Type _type;			
			void *_data;

		};

	}

}
