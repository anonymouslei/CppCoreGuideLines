# add_library(MathFunctions mysqrt.cxx)  # remove in step 6

# first we add the executable that generates the table
add_executable(MakeTable MakeTable.cxx)  # step 6

# step 6
# add the command to generate the source code
# we add a custom command that specifies how to produce 
# table.h by running MakeTable
add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/Table.h
  COMMAND MakeTable ${CMAKE_CURRENT_BINARY_DIR}/Table.h
  DEPENDS MakeTable
)

# step 6
# we habe to let CMake know that 'mysqrt.cxx' depends on 
# generated file table.h. this is done by adding the 
# generated table.h to the list of sources for the library
# MathFunctions.
add_library(MathFunctions
            mysqrt.cxx 
            ${CMAKE_CURRENT_BINARY_DIR}/Table.h
            )

target_include_directories(MathFunctions
	INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
  PRIVATE ${CMAKE_CURRENT_BINARY_DIR}  # step 6
  # we also have to add the current binary directory to the 
  # list of include directories so that table.h can be found 
  # and included by mysqrt.cxx.
	)
# step 6 remove
# step 5
# does this system provide the log and exp functions?
# include(CheckSymbolExists)
# check_symbol_exists(log "math.h" HAVE_LOG)
# check_symbol_exists(exp "math.h" HAVE_EXP)
# if(NOT (HAVE_LOG AND HAVE_EXP))
  # unset(HAVE_LOG CACHE)
  # unset(HAVE_EXP CACHE)
  # set(CMAKE_REQUIRED_LIBRARIES "m")
  # check_symbol_exists(log "math.h" HAVE_LOG)
  # check_symbol_exists(exp "math.h" HAVE_EXP)
  # if(HAVE_LOG AND HAVE_EXP)
    # target_link_libraries(MathFunctions PRIVATE m)
  # endif()
# endif()

# step 6 remove
# step 5
# add compile definitions
# if(HAVE_LOG AND HAVE_EXP)
  # target_compile_definitions(MathFunctions
                            #  PRIVATE "HAVE_LOG" "HAVE_EXP")
#
# endif()

# install rules
install(TARGETS MathFunctions DESTINATION lib)  # step 4
install(FILES MathFunctions.h DESTINATION include)  # step 4


message(${CMAKE_CURRENT_BINARY_DIR}) 
message(${CMAKE_CURRENT_SOURCE_DIR})