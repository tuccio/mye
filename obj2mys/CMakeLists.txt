cmake_minimum_required ( VERSION 3.0 )

project ( obj2mys )

add_executable ( obj2mys obj2mys.cpp )

#target_link_libraries( obj2mys myecore )

target_link_libraries( obj2mys ${IL_LIBRARIES} ${ILU_LIBRARIES} ${ILUT_LIBRARIES} )
target_link_libraries( obj2mys ${ASSIMP_LIBRARY} )