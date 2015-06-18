cmake_minimum_required ( VERSION 3.0 )

project ( mathtest )

add_executable ( mathtest mathcycles.cpp mathtest.cpp mathtest2.cpp mathtestlib.cpp )
add_executable ( mathtest2 mathcycles.cpp mathtest.cpp mathtest2.cpp mathtestlib.cpp )
add_library ( mathtestlib mathcycles.cpp mathtest.cpp mathtest2.cpp mathtestlib.cpp )
add_executable ( mathcycles mathcycles.cpp mathtest.cpp mathtest2.cpp mathtestlib.cpp )
