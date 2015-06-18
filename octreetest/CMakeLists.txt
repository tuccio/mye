project ( octreetest )

find_package ( OpenGL REQUIRED )
find_package ( GLUT   REQUIRED )

include_directories ( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} )

add_executable ( octreetest octreetest.cpp )

target_link_libraries ( octreetest ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} )