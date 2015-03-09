cmake_minimum_required ( VERSION 3.0 )

project ( mye )

set ( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/" )

set ( EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin )
set ( LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/lib )

if (MSVC)
	set ( CMAKE_CXX_FLAGS_RELEASE        "${CMAKE_CXX_FLAGS_RELEASE} /Ob2 /O2 /Ot /MD" )
	set ( CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} /Ob2 /O2 /Ot /MD" )
	set ( CMAKE_CXX_FLAGS_DEBUG          "${CMAKE_CXX_FLAGS_DEBUG} /Ob0 /Od /MDd" )
	#set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} /NODEFAULTLIB:MSVCRT")
endif ()

find_package ( Bullet       REQUIRED )
find_package ( Boost        REQUIRED )
find_package ( Lua          REQUIRED )
find_Package ( lua++11      REQUIRED )
find_package ( DevIL        REQUIRED )
find_package ( Assimp       REQUIRED )
find_package ( RapidXML     REQUIRED )
find_package ( Windows81SDK REQUIRED )

include_directories ( ${Boost_INCLUDE_DIRS} )
include_directories ( ${LUA_INCLUDE_DIR} )
include_directories ( ${LUAPP11_INCLUDE_DIR} )
include_directories ( ${BULLET_INCLUDE_DIR} )
include_directories ( ${IL_INCLUDE_DIR} )
include_directories ( ${ASSIMP_INCLUDE_DIR} )
#include_directories ( ${DirectX11_INCLUDE_DIR} )
include_directories ( ${RAPIDXML_DIR} )

include_directories ( ${CMAKE_SOURCE_DIR}/.. )
include_directories ( ${CMAKE_SOURCE_DIR} )

#add_subdirectory ( luabind )
add_subdirectory ( core )
add_subdirectory ( win )
add_subdirectory ( lua )
add_subdirectory ( d3d11 )
add_subdirectory ( mye )

add_subdirectory ( messingwithentities )