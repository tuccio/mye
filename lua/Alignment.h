#pragma once

#include <mye/core/Components.h>
#include <luabind/detail/typetraits.hpp>

//namespace luabind
//{
//	namespace detail
//	{
//
//		template <> struct is_nonconst_pointer<      mye::core::RigidBodyComponent  > : is_nonconst_pointer<      int >{};
//		template <> struct is_nonconst_pointer<      mye::core::RigidBodyComponent *> : is_nonconst_pointer<      int*>{};
//		template <> struct is_nonconst_pointer<      mye::core::RigidBodyComponent &> : is_nonconst_pointer<      int&>{};
//		template <> struct is_nonconst_pointer<const mye::core::RigidBodyComponent  > : is_nonconst_pointer<const int >{};
//		template <> struct is_nonconst_pointer<const mye::core::RigidBodyComponent *> : is_nonconst_pointer<const int*>{};
//		template <> struct is_nonconst_pointer<const mye::core::RigidBodyComponent &> : is_nonconst_pointer<const int&>{};
//
//		template <> struct is_const_pointer<      mye::core::RigidBodyComponent  > : is_const_pointer<      int >{};
//		template <> struct is_const_pointer<      mye::core::RigidBodyComponent *> : is_const_pointer<      int*>{};
//		template <> struct is_const_pointer<      mye::core::RigidBodyComponent &> : is_const_pointer<      int&>{};
//		template <> struct is_const_pointer<const mye::core::RigidBodyComponent  > : is_const_pointer<const int >{};
//		template <> struct is_const_pointer<const mye::core::RigidBodyComponent *> : is_const_pointer<const int*>{};
//		template <> struct is_const_pointer<const mye::core::RigidBodyComponent &> : is_const_pointer<const int&>{};
//
//	}
//}

#define __MYE_LUABIND_ALIGNMENT_WORKAROUND(__TYPE)\
namespace luabind\
{\
	namespace detail\
	{\
		template <> struct is_nonconst_pointer<      __TYPE  > : is_nonconst_pointer<      int >{};\
		template <> struct is_nonconst_pointer<      __TYPE *> : is_nonconst_pointer<      int*>{};\
		template <> struct is_nonconst_pointer<      __TYPE &> : is_nonconst_pointer<      int&>{};\
		template <> struct is_nonconst_pointer<const __TYPE  > : is_nonconst_pointer<const int >{};\
		template <> struct is_nonconst_pointer<const __TYPE *> : is_nonconst_pointer<const int*>{};\
		template <> struct is_nonconst_pointer<const __TYPE &> : is_nonconst_pointer<const int&>{};\
		template <> struct is_const_pointer<      __TYPE  > : is_const_pointer<      int >{};\
		template <> struct is_const_pointer<      __TYPE *> : is_const_pointer<      int*>{};\
		template <> struct is_const_pointer<      __TYPE &> : is_const_pointer<      int&>{};\
		template <> struct is_const_pointer<const __TYPE  > : is_const_pointer<const int >{};\
		template <> struct is_const_pointer<const __TYPE *> : is_const_pointer<const int*>{};\
		template <> struct is_const_pointer<const __TYPE &> : is_const_pointer<const int&>{};\
	}\
}\

__MYE_LUABIND_ALIGNMENT_WORKAROUND(mye::core::RigidBodyComponent)