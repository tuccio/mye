cmake_minimum_required ( VERSION 3.0 )

project ( mathtest )

add_executable ( mathtest mathtest.cpp )
add_library ( mathtestlib mathtestlib.cpp )
add_executable ( mathcycles mathcycles.cpp )