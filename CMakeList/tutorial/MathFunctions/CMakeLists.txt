add_library(MathFunctions mysqrt.cxx)

target_include_directories(MathFunctions
	INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
	)

# install rules
install(TARGETS MathFunctions DESTINATION lib)  # step 4
install(FILES MathFunctions.h DESTINATION include)  # step 4