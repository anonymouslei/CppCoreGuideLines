cmake_minimum_required(VERSION 3.10)

# set the project name
project(Tutorial VERSION 1.0)

# specify the C++ standard
# Make sure to add the CMAKE_CXX_STANDARD declarations above the call to add_executable
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# add the executable
add_executable(Tutorial tutorial.cxx)

configure_file(TutorialConfig.h.in TutorialConfig.h)

# make sure to add the executable declarations above the call to target_include_directories
target_include_directories(Tutorial PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           )
