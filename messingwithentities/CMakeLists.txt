cmake_minimum_required ( VERSION 3.0 )

project ( messingwithentities )

add_executable ( messingwithentities main.cpp )

target_link_libraries( messingwithentities myecore )
target_link_libraries( messingwithentities debug ${BULLET_COLLISION_LIBRARY_DEBUG} debug ${BULLET_DYNAMICS_LIBRARY_DEBUG} debug ${BULLET_MATH_LIBRARY_DEBUG} )
target_link_libraries( messingwithentities optimized ${BULLET_COLLISION_LIBRARY} optimized ${BULLET_DYNAMICS_LIBRARY} optimized ${BULLET_MATH_LIBRARY} )

target_link_libraries( messingwithentities ${IL_LIBRARIES} ${ILU_LIBRARIES} ${ILUT_LIBRARIES} )
target_link_libraries( messingwithentities ${LUA_LIBRARY} )
target_link_libraries( messingwithentities ${LUAPP11_LIBRARIES} )

target_link_libraries( messingwithentities ${Windows81SDK_D3D11_LIBRARY} ${Windows81SDK_D3DCOMPILER_LIBRARY} )

target_link_libraries( messingwithentities ${ASSIMP_LIBRARY} )