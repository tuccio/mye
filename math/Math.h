#pragma once

#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/arithmetic/add.hpp>
#include <boost/preprocessor/arithmetic/div.hpp>
#include <boost/preprocessor/arithmetic/mod.hpp>
#include <boost/preprocessor/arithmetic/mul.hpp>
#include <boost/preprocessor/arithmetic/sub.hpp>
#include <boost/preprocessor/comparison/equal.hpp>
#include <boost/preprocessor/comparison/not_equal.hpp>
#include <boost/preprocessor/control/if.hpp>
#include <boost/preprocessor/control/expr_if.hpp>
#include <boost/preprocessor/facilities/empty.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>
#include <boost/preprocessor/tuple/rem.hpp>

#include "Declarations.h"

#include "Matrix.h"

#include "Matrix2.h"
#include "Matrix2fSSE.h"

#include "Matrix3.h"
#include "Matrix3fSSE.h"

#include "Matrix4.h"
#include "Matrix4fSSE.h"

#include "Vector2.h"
#include "Vector2fSSE.h"

#include "Vector3.h"
#include "Vector3fSSE.h"

#include "Vector4.h"
#include "Vector4fSSE.h"

#include "Functions.h"
#include "Trigonometry.h"

#include "Rational.h"

#include "Swizzle.h"

#include "MatrixOperations.h"

#include "Matrix2fSSE.inl"
#include "Matrix3fSSE.inl"
#include "Matrix4fSSE.inl"

#include "Vector2fSSE.inl"
#include "Vector3fSSE.inl"
#include "Vector4fSSE.inl"