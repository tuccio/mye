find_path (
	LUAPP11_INCLUDE_DIR lua++11/lua++11.h
	PATH_SUFFIXES include
	PATHS
		${LUAPP11_ROOT}
)

find_library (
	LUAPP11_LIBRARIES
	NAMES lua++11
	HINTS
		${LUAPP11_ROOT}/lib
	PATH_SUFFIXES
		Release
)

find_library (
	LUAPP11_LIBRARIES_DEBUG
	NAMES lua++11
	HINTS
		${LUAPP11_ROOT}/lib
	PATH_SUFFIXES
		Debug
)

mark_as_advanced ( LUAPP11_INCLUDE_DIR LUAPP11_LIBRARIES LUAPP11_LIBRARIES_DEBUG )