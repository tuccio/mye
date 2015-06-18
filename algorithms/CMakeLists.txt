cmake_minimum_required ( VERSION 3.0 )

project ( myealgorithms )

add_library ( myealgorithms Boundings.h Common.h LooseOctree.h LooseOctree.inl Morton.h )
set_target_properties ( myealgorithms PROPERTIES LINKER_LANGUAGE CXX )