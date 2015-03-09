#pragma once

#include "Allocator.h"
#include "Component.h"
#include "VariableTypes.h"

#include <mye/math/Geometry.h>

#include <typeindex>
#include <type_traits>

namespace mye
{

	namespace core
	{

		template <typename T>
		class VariableConstraint;


		class VariableRTTI :
			public Component
		{

		public:

			inline VariableTypes GetType(void) const
			{
				return m_type;
			}

			Component * Clone(void) const = 0;

		protected:

			VariableRTTI(const String & name, VariableTypes type) :
				Component(ComponentTypes::VARIABLE, name)
			{
				m_type = type;
			}

			VariableTypes m_type;

		};

		template <typename T, typename Allocator = DefaultAllocator>
		class VariableComponent :
			public VariableRTTI
		{

		public:

			VariableComponent(const String &name);
			~VariableComponent(void);

			VariableComponent * Clone(void) const;

			inline T * GetPointer(void);

			inline const T & Get(void) const;
			inline void Set(const T &v);

			__MYE_USE_ALLOCATOR(Allocator)

		private:

			T m_variable;

		};

		typedef    VariableComponent<mye::math::Vector2> Vector2VariableComponent;
		typedef    VariableComponent<mye::math::Vector3> Vector3VariableComponent;
		typedef    VariableComponent<mye::math::Vector4> Vector4VariableComponent;
				  
		typedef   VariableComponent<mye::math::Vector2i> Vector2iVariableComponent;
		typedef   VariableComponent<mye::math::Vector3i> Vector3iVariableComponent;
		typedef   VariableComponent<mye::math::Vector4i> Vector4iVariableComponent;
				  
		typedef                  VariableComponent<bool> BoolVariableComponent;
		typedef                   VariableComponent<int> IntVariableComponent;
		typedef       VariableComponent<mye::math::Real> RealVariableComponent;
				     
		typedef                VariableComponent<String> StringVariableComponent;
		typedef      VariableComponent<GameObjectHandle> GameObjectVariableComponent;

		typedef VariableComponent<mye::math::Quaternion> QuaternionVariableComponent;

		//typedef    VariableComponent<mye::math::Matrix2> Matrix2VariableComponent;
		typedef    VariableComponent<mye::math::Matrix3> Matrix3VariableComponent;
		typedef    VariableComponent<mye::math::Matrix4> Matrix4VariableComponent;

		//class VariableComponent_
		//{

		//public:

		//	VariableComponent_(void) { }

		//	template <typename T>
		//	T * Get(const String &name)
		//	{
		//		
		//		T * result = nullptr;

		//		auto it = m_map.find(name);

		//		if (it != m_map.end())
		//		{
		//			assert(it->second.type == VariableTraits<T>::type::value);
		//			return static_cast<T*>(&m_data[it->second.offset]);
		//		}

		//		return result;

		//	}

		//	bool RetrieveType(const String &name, VariableTypes &type)
		//	{

		//		bool success = false;
		//		auto it = m_map.find(name);

		//		if (it != m_map.end())
		//		{
		//			type = it->second.type;
		//			success = true;
		//		}

		//		return success;

		//	}

		//	template <typename T>
		//	bool Add(const String &name)
		//	{


		//		bool success = false;
		//		auto it = m_map.find(name);

		//		if (it != m_map.end())
		//		{
		//			size_t aligment = VariableTraits<T>::alignment::value;
		//			success = true;
		//		}

		//		return success;

		//	}

		//private:

		//	static struct VariableInfo
		//	{
		//		size_t        offset;
		//		VariableTypes type;
		//	};

		//	typedef std::unordered_map<String, VariableInfo> VariablesMap;

		//	std::vector<unsigned char> m_data;
		//	              VariablesMap m_map;

		//};

	}

}

#include "VariableComponent.inl"