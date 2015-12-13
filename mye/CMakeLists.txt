cmake_minimum_required ( VERSION 3.0 )

project ( mye )

add_executable ( mye WIN32 main.cpp MainWindowListener.cpp MainWindowListener.h )

target_link_libraries( mye myecore myed3d11 myelua myewin )
target_link_libraries( mye debug ${BULLET_COLLISION_LIBRARY_DEBUG} debug ${BULLET_DYNAMICS_LIBRARY_DEBUG} debug ${BULLET_MATH_LIBRARY_DEBUG} )
target_link_libraries( mye optimized ${BULLET_COLLISION_LIBRARY} optimized ${BULLET_DYNAMICS_LIBRARY} optimized ${BULLET_MATH_LIBRARY} )

target_link_libraries( mye ${IL_LIBRARIES} ${ILU_LIBRARIES} ${ILUT_LIBRARIES} )
target_link_libraries( mye ${LUA_LIBRARY} )

target_link_libraries( mye debug ${LUAPP11_LIBRARIES_DEBUG} )
target_link_libraries( mye optimized ${LUAPP11_LIBRARIES} )

target_link_libraries( mye ${DirectX11_LIBRARY} )
target_link_libraries( mye ${ASSIMP_LIBRARY} )