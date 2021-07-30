# CMake Tutorial
- [CMake Tutorial](#cmake-tutorial)
  - [Step 1: A basic starting point](#step-1-a-basic-starting-point)
    - [adding a version number and configured header file](#adding-a-version-number-and-configured-header-file)
    - [specify the C++ Standard](#specify-the-c-standard)
    - [build and test](#build-and-test)
  - [step 2: adding a library](#step-2-adding-a-library)
  - [step 3: adding usage requirements for a library](#step-3-adding-usage-requirements-for-a-library)
  - [Step 4: Installing and testing](#step-4-installing-and-testing)
  - [step 5: adding system introspection](#step-5-adding-system-introspection)
  - [step 6: adding a custom command and generated file](#step-6-adding-a-custom-command-and-generated-file)
  - [step 7: packaging an installer](#step-7-packaging-an-installer)
  - [step 8: adding support for a testing dashboard](#step-8-adding-support-for-a-testing-dashboard)
  - [step 9: selecting static or shared libraries](#step-9-selecting-static-or-shared-libraries)
  - [step 10: adding generator expressions](#step-10-adding-generator-expressions)
  - [step 11: adding export configuration](#step-11-adding-export-configuration)
  - [step 12: packaging debug and release](#step-12-packaging-debug-and-release)
## Step 1: A basic starting point
```
cmake_minimum_required(VERSION 3.10)

# set the project name
project(Tutorial)

# add the executable
add_executable(Tutorial tutorial.cxx)
```
Upper, lower, and mixed case commands are supported by CMake. 

### adding a version number and configured header file
The first feature we will add is to provide our executable and project with a version number.
While we could do this exclusively in the source code, using `CMakeLists.txt` provides more flexibility.
1. modify the `CMakeLists.txt` to use the `project()` command to set the project name and version number.
   ```
   cmake_minimum_required(VERSION 3.10)

   # set the project name and version
   project(Tutorial VERSION 1.0)
   ```
2. configure a header file to pass the version number to the source code
   ```
   configure_file(TutorialConfig.h.in TutorialConfig.h)
   ```
3. since the configured file will be written into the binary tree, we must add that directory to the list of paths to search for include files. Add the following lines to the end of the `CMakeLists.txt` file
   ```
   target_include_directories(Tutorial PUBLIC
   "${PROJECT_BINARY_DIR}"
   )
   ```
4. using your favorite editor, create `TutorialConfig.h.in` in the source directory with the following contents
   ```
   # TutorialConfig.h.in
   // the configured options and settings for Tutorial
   #define Tutorial_VERSION_MAJOR @Tutorial_VERSION_MAJOR@
   #define Tutorial_VERSION_MINOR @Tutorial_VERSION_MINOR@
   ```
   When CMake configures this header file the values for `@Tutorial_VERSION_MAJOR@` AND `@Tutorial_VERSION_MINOR@` will be replaced.
5. modify `tutorial.cxx` to include the configured header file, `TutorialConfig.h`.
6. let's print out the executable name and version number by updating `tutorial.cxx` as follows: ...

### specify the C++ Standard
Next let's add some C++11 features to our project by replacing `atof` with `std::stod` in `tutorial.cxx`. At the same time, remove `#include<cstdlib>`.

```
#tutorial.cxx
const double inputValue = std::stod(argv[1]);
```
we will need to explicitly state in the CMake code that it should use the correct flags.
The easiest way to enable support for a specific C++ standard in CMake is by using the `CMAKE_CXX_STANDARD` variable.
Make sure to add the `CMAKE_CXX_STANDARD` declarations above the call to `add_executable`.
```
cmake_minimum_required(VERSION 3.10)

# set the project name and version
project(Tutorial VERSION 1.0)

#specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
```

### build and test
1. create a build directory
   ```
   mkdir Step1_build
   ```
2. navigate to the build directory and run CMake to configure the project and generate a native build system
   ```
   cmake ../Step1
   ```
3. then call that build system to actually compile/link the project
   ```
   cmake --build
   ```

## step 2: adding a library
Now we will add a library to our project.
This library will contain our own implementation for computing the square root of a number.
The executable can then use this library instead of the standard square root function provided by the compiler.
For this tutorial we will put the library into a subdirectory called `MathFunctions`. 
this directory already contains a header file,
`MathFunction.h`, and a source file `mysqrt.cxx`.

1. add the following one line `CMakeLists.txt` file to the `MathFunctions` directory.
   ```
   # MathFunctions/CMakeLists.txt
   add_library(MathFunctions mysqrt.cxx)
   ```
2. To make use of the new library we will add an `add_directory()` call in the top-level `CMakeLists.txt` file so that the library will get built.
we add the new library to the executable, and add `MathFunctions` as an include directory so that the `mysqrt.h` header file can be found.
   ```
   #CMakeLists.txt
   # the last few lines of the top-level CMakeLists.txt file should now look like:
   # add the MathFunctions library
   add_subdirectory(MathFunctions)

   # add the executable
   add_executable(Tutorial tutorial.cxx)

   target_link_libraries(Tutorial PUBLIC MathFunctions)

   # add the binary tree to the search path for include files
   # so that we will find TutorialConfig.h
   target_include_directories(Tutorial PUBLIC 
   "${PROJECT_BINARY_DIR}"
   "${PROJECT_SOURCE_DIR}/MathFunctions")
   ```
3. let us make the `MathFunctions` library optional. For larger projects this is a common occurrence.
   1. add an option to the top-level `CMakeLists.txt` file
      ```
      option(USE_MYMATH "Use tutorial provided math implementation" ON)

      ``` 
      this option will be displayed in the  `cmake-gui` and `ccmake` with a default value of `ON` that can be changed by the user.
      This setting will be stored in the cache so that the user does not need to set the value each time they run CMake on a build directory.
   2. make building and linking the `MathFunctions` library conditional. To do this we change the end of the top-level `CMakeLists.txt` file to look like the following.
      ```
      if(USE_MYMATH)
         add_subdirectoty(MathFunctions)
         list(APPEND EXTRA_LIBS MathFunctions)
         lsit(APPEND EXTRA_INCLUDES "{PROJECT_SOURCE_DIR}/MathFunctions")
      endif()

      # add the executable
      add_executable(Tutorial tutorial.cxx)

      target_link_libraries(Tutorial PUBLIC ${EXTRA_LIBS})

      # add the binary tree to the search path for include files 
      # so that we will find TutorialConfig.h
      target_include_directories(Tutorial PUBLIC 
      "${PROJECT_BINARY_DIR}"
      ${EXTRA_INCLUDES})

      ```
      Note the use of the variable `EXTRA_LIBS` to collect up any optional libraries to later be linked into the executable.
      the variable `EXTRA_LIBS` is used similary for optional header files.
      This is a classic approach when dealing with many optional components.

   3. changes to the source code.
      1. in `tutorial.cxx`, include the `MathFunctions.h` header if we need it:
         ```
         # tutorial.cxx
         #ifdef USE_MYMATH
         #  include "MathFunctions.h"
         #endif
         ```
      2. in the same file, make `USE_MYMATH` control which square root function is used:
         ```
         #ifdef USE_MYMATH
            const double outputValue = mysqrt(inputValue);
         #else
            const double outputValue = sqrt(inputValue);
         #endif
         ```
   4. since the source code now requires `USE_MYMATH` we can add it to `TutorialConfig.h.in` with the following line:
      ```
      # TUtorialConfig.h.in
      #cmakedefine USE_MYMATH
      ```
4. if you want to change the option from the command-line, try:
   ```
   cmake ../Step2 -DUSE_MYMATH=off
   ```
   
## step 3: adding usage requirements for a library
Usage requirements allow for far better control over a library or executable's link and include line while also giving more control over the transitive property of targets inside CMake.
The primary commands that leverage usage requirements are:
- target_compile_definitions()
- target_compile_options()
- target_include_directories()
- target_link_libraries()

we first state that anybody linking to `MathFunctions` needs to include the current source directory, while `MathFunctions` itself doesn't.
So this can become an `INTERFACE` usage requirement.
`INTERFACE` means things that consumers require but the producer doesn't.
Add the following lines to the end of `MathFunctions/CMakeLists.txt`.

## Step 4: Installing and testing
## step 5: adding system introspection
## step 6: adding a custom command and generated file
## step 7: packaging an installer
## step 8: adding support for a testing dashboard
## step 9: selecting static or shared libraries
## step 10: adding generator expressions
## step 11: adding export configuration
## step 12: packaging debug and release