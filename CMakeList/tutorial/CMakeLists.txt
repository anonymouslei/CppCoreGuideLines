cmake_minimum_required(VERSION 3.10)

# set the project name
project(Tutorial VERSION 1.0)  # step 1

# specify the C++ standard
# Make sure to add the CMAKE_CXX_STANDARD declarations above the call to add_executable
set(CMAKE_CXX_STANDARD 11)  # step 1
set(CMAKE_CXX_STANDARD_REQUIRED True)  # step 1

option(USE_MYMATH "Use tutorial provided math implementation" ON)  # step 2

# configure a header file to pass some of the CMake settings to the source code
configure_file(TutorialConfig.h.in TutorialConfig.h)

# add the MathFunctions library
# add_subdirectory(MathFunctions)  # step 2

# add the MathFunctions library
if(USE_MYMATH) # step 2
  add_subdirectory(MathFunctions)
  list(APPEND EXTRA_LIBS MathFunctions)
  # list(APPEND EXTRA_INCLUDES "${PROJECT_SOURCE_DIR}/MathFunctions")  # remove in step 3
endif()

# add the executable
add_executable(Tutorial tutorial.cxx)
target_link_libraries(Tutorial PUBLIC ${EXTRA_LIBS})

# make sure to add the executable declarations above the call to target_include_directories
target_include_directories(Tutorial PUBLIC
                           "${PROJECT_BINARY_DIR}"  # step 1
                          #  "${PROJECT_SOURCE_DIR}/MathFunctions"  # step 2
                          # ${EXTRA_INCLUDES}  # step 2  # remove in step 3
                           )

# add the install targets
install(TARGETS Tutorial DESTINATION bin)  # step 4
install(FILES "${PROJECT_BINARY_DIR}/TutorialConfig.h"  # step 4
       DESTINATION include  # step 4
       )

# step 4 testing support
enable_testing()
# does the application run
add_test(NAME Runs COMMAND Tutorial 25)

# does the usage message work?
add_test(NAME Usage COMMAND Tutorial)
set_tests_properties(Usage 
        PROPERTIES PASS_REGULAR_EXPRESSION "Usage:.*number"
        )

# define a function to simplify adding tests
function(do_test target arg result)
        add_test(NAME Comp${arg} COMMAND ${target} ${arg})
        set_tests_properties(Comp${arg}
            PROPERTIES PASS_REGULAR_EXPRESSION ${result}
            )
endfunction(do_test)

# do a bunch of result based tests
do_test(Tutorial 4 "4 is 2")
do_test(Tutorial 9 "9 is 3")
do_test(Tutorial 5 "5 is 2.236")
do_test(Tutorial 7 "7 is 2.645")
do_test(Tutorial 25 "25 is 5")
do_test(Tutorial -25 "-25 is [-nan|nan|0]")
do_test(Tutorial 0.0001 "0.0001 is 0.01")