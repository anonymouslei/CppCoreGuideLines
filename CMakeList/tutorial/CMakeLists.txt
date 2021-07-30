cmake_minimum_required(VERSION 3.10)

# set the project name
project(Tutorial VERSION 1.0)  # step 1

# specify the C++ standard
# Make sure to add the CMAKE_CXX_STANDARD declarations above the call to add_executable
set(CMAKE_CXX_STANDARD 11)  # step 1
set(CMAKE_CXX_STANDARD_REQUIRED True)  # step 1

# add the MathFunctions library
add_subdirectory(MathFunctions)  # step 2

if(USE_MYMATH) # step 2
  add_subdirectory(MathFunctions)
  list(APPEND EXTRA_LIBS MathFunctions)
  list(APPEND EXTRA_INCLUDES "${PROJECT_SOURCE_DIR}/MathFunctions")
endif()

# add the executable
add_executable(Tutorial tutorial.cxx)

# make sure to add the executable declarations above the call to target_include_directories
target_include_directories(Tutorial PUBLIC
                           "${PROJECT_BINARY_DIR}"  # step 1
                          #  "${PROJECT_SOURCE_DIR}/MathFunctions"  # step 2
                          ${EXTRA_INCLUDES}  # step 2
                           )

option(USE_MYMATH "Use tutorial provided math implementation" ON)  # step 2

configure_file(TutorialConfig.h.in TutorialConfig.h)