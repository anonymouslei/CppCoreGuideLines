# CMake Tutorial
- [CMake Tutorial](#cmake-tutorial)
  - [Step 1: A basic starting point](#step-1-a-basic-starting-point)
    - [adding a version number and configured header file](#adding-a-version-number-and-configured-header-file)
    - [specify the C++ Standard](#specify-the-c-standard)
    - [build and test](#build-and-test)
  - [step 2: adding a library](#step-2-adding-a-library)
  - [step 3: adding usage requirements for a library](#step-3-adding-usage-requirements-for-a-library)
  - [Step 4: Installing and testing](#step-4-installing-and-testing)
    - [install rules](#install-rules)
    - [Testing support](#testing-support)
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
   cmake --build .
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
<!-- TODO: not understand -->
我的理解是在mathfunctions的cmakelists加入几句话，就可以在主cmakelist中删除掉关于extra_include的语句

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

```
# MathFunctions/CMakeLists.txt
target_include_directories(MathFunctions 
INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
)
```
Now that we've specified usage requirements for `MathFunctions` we can safely remove our uses of the `EXTRA_INCLUDES` variable from the top-level `CMakeLists.txt` here:
```
#CMakeLists.txt
if(USE_MYMATH)
  add_subdirectory(MathFunctions)
  list(APPEND EXTRA_LIBS MathFunctions)
endif()
```
and here:
```
target_include_directories(Tutorial PUBLIC
   "${PROJECT_BINARY_DIR}"
   )
```
once this is done, run the `cmake` executable or the `cmake-gui` to configure the project and then build it with your build tool or by using `cmake --build .` from the build directory.

## Step 4: Installing and testing
### install rules
The install rules ar efairly simple: for `MathFunctions` we want to install the library and header file and for the application we want to install the executable and configured header.
1. So to the end of `MathFunctions/CMakeLists.txt` we add:
   ```
   # MathFunctions/CMakeLists.txt
   install(TARGETS MathFunctions DESTINATION lib)
   install(FILES MathFunctions.h DESTINATION include)
   ```
2. to the end of the top-level `CMakeLists.txt` we add:
   ```
   #CMakeLists.txt
   install(TARGETS Tutorial DESTINATION bin)
   install(FILES "${PROJECT_BINARY_DIR}/TutorialConfig.h"
      DESTINATION include
      )
   ```
   That is all that is needed to create a basic local install of the tutorial.
3. run the install step by using the `install` option of the cmake command from the command line. For multi-configuration tools, don't forget to use the `--config` argument to specify the configuration.
   If using an IDE, simply build the `INSTALL` target.
   This step will install the approprate header files, libraries, and executables.
   ```
   cmake --install .
   ```
   The CMake variable `CMAKE_INSTALL_PREFIX` is used to determine the root of where the files will be installed.
   If using the `cmake --install` command, the installation prefix can be overridden via the `--prefix` argument.
   ```
   cmake --install . --prefix "/home/myuser/installdir"
   ```
4. Navigate to the install directory and verify that the installed Tutorial runs. 

### Testing support
At the end of the top-level `CMakeLists.txt` file we can enable testing and then add a number of basic tests to verify that the application is working correctly.
```
#CMakeLists.txt
enable_testing()
# does the application run
add_test(NAME Runs COMMAND Tutorial 25)
# this test simply verifies that the application runs, does not segfault or otherwise crash, and has a zero return value. this is the basic form of a CTest test.

# does the usage message work?
add_test(NAME Usage COMMAND Tutorial)
set_tests_properties(Usage
    PROPERTIES PASS_REGULAR_EXPRESSION "Usage:.*number"
    )
# this test makes use of the PASS_REGULAR_EXPRESSION test property to verify that the output of the test contains certain strings. In this case, verifying that the usage message is printed when an incorrect number of arguments are provided.

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

# we have a function called do_test that runs the application and verifies that the computed square root is correct for given input.
For each invocation of do_test, another test is added to the project with a name, input, and expected results based on the passed arguments.
```

1. Rebuild the application and then cd to the binary directory
2. run the ctest executable: `ctest -N` and `ctest -VV`.
For multi-config generators, the configuration type must be specified. To run tests in Debug mode, for example, use `ctest -C Debug -VV` from the build directory.
Alternatively, build the `RUN_TESTS` target from the IDE.

## step 5: adding system introspection
Let us consider adding some code to our project that depends on features the target platform may not have. 
For this example, we will add some code that depends on whether or not the target platform has the `log` and `exp` functions.
Of course almost every platform has these functions but for this tutorial assume that they are not common.

If the platform has `log` and `exp` then we will use them to compute the square root in the `mysqrt` function. 
1. we first test for the availability of these functions using the `checkSymbolExists` module in `MathFunctions/CMakeLists.txt`.
On some platforms, we will need to link to the `m` library.
If `log` and `exp` are not initially found, require the `,` library and try again.
```
# MathFunctions/CMakeLists.txt
include(CheckSymbolExists)
check_symbol_exists(log "math.h" HAVE_LOG)
check_symbol_exists(exp "math.h" HAVE_EXP)
if(NOT (HAVE_LOG AND HAVE_EXP))
  unset(HAVE_LOG CACHE)
  unset(HAVE_EXP CACHE)
  set(CMAKE_REQUIRED_LIBRARIES "m")
  check_symbol_exists(log "math.h" HAVE_LOG)
  check_symbol_exists(exp "math.h" HAVE_EXP)
  if(HAVE_LOG AND HAVE_EXP)
    target_link_libraries(MathFunctions PRIVATE m)
  endif()
endif()
```
If available, use `target_compile_definitions()` to specify `HAVE_LOG` and `HAVE_EXP` as `PRIVATE` compile definitions.

```
# MathFunctions/CMakeLists.txt
if (HAVE_LOG AND HAVE_EXP)
  target_compile_definitions(MathFunctions
                             PRIVATE "HAVE_LOG" "HAVE_EXP")
endif()
```
if `log` and `exp` are available on the system, then we will use them to compute the square root in the `mysqrt` function.
```cpp
# MathFunctions/mysqrt.cxx
#if defined(HAVE_LOG) && defined(HAVE_EXP)
  double result = exp(log(x) * 0.5);
  std::cout << "Computing sqrt of " << x << " to be " << result << " using log and exp " << std::endl;
#else
  double result = x;
```
we will also need to modify `mysqrt.cxx` to include `cmath`.
```
#MathFucntions/mysqrt.cxx
#include <cmath>
```

## step 6: adding a custom command and generated file
suppose, for the purpose of this tutorial, we decide that we never want to use the platform `log` and `exp` functions and instead would like to generate a table of precomputed values to use in the `mysqrt` function.
In this section, we will create the table as part of the build process, and then compile that table into our aplication.
1. let's remove the check for the `log` and `exp` functions in `MathFunctions/CMakeLists.txt`. 
2. Then remove the check for `HAVE_LOG` and `HAVE_EXP` from `mysqrt.cxx`. 
At the same time, we can remove `#include <cmath>`.
3. In the `MathFunctions` subdirectory, a new source file named `MakeTable.cxx` has been provided to generate the table.

after reviewing the file, we can see that the table is produced as valid C++ code and that the output filename is passed in as an argument.
4. to add the appropriate commands to the `MathFunctions/CMakeLists.txt` file to build the MakeTable executable and then run it as part of the build process.
A few commands are needed to accomplish this.
   1. at the top of `MathFunctions/CMakeLists.txt`, the executable for `MakeTable` is added as any other executable would be added.
   ```
   # MathFunctions/CMakeLists.txt
   add_executable(MakeTable MakeTable.cxx)
   ```
   2. we add a custom command that specifies how to produce `Table.h` by running MakeTable
   ```
   # MathFunctions/CMakeLists.txt
   add_custom_command(
      OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/Table.h
      COMMAND MakeTable ${CMAKE_CURRENT_BINARY_DIR}/Table.h
      DEPENDS MakeTable
   )
   ```
   3. we have to let CMake know that `mysqrt.cxx` depends on generated file `Table.h`. This is done by adding the genreated `Table.h` to the list of sources for the library MathFunctions.

   ```
   #MathFunctions/CMakeLists.txt
   add_library(MathFunctions
               mysqrt.cxx
               ${CMAKE_CURRENT_BINARY_DIR}/Table.h)
   ```
   4. we also have to add the current binary directory to the list of include directories so that `Table.h` can be found and included by `mysqrt.cxx`.
   ```
   #MathFunctions/CMakeLists.txt
   target_include_directories(MathFunctions
               INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
               PRIVATE ${CMAKE_CURRENT_BINARY_DIR}
               )
   ```
   5. let's use the generated table. First, modify `mysqrt.cxx` to include `Table.h`.
   6. we can rewrite the `mysqrt` function to use the table.
   ```cpp
   double mysqrt(double x) {
      if (x < = 0) {
         return 0;
      }

      // use the table to help find an initial value
      double result = x;
      if (x >= 1 && x < 10) {
         std::cout << "use the table to help find an initial value " << std::endl;
         result = sqrtTable[static_cast<int>(x)];
      }

      // do ten iterations
      for (int i = 0; i < 10; ++i) {
         if (result <= 0) {
            result = 0.1;
         }
         double delta = x - (result * result);
         result = result + 0.5 * delta / result;
         std::cout << "Computing sqrt of " << x << " to be " << result << std::endl;
      }
      return result;
   }
   ```

when this project is built it will first build the `MakeTable` executable. It will then run `MakeTable` to produce `Table.h`.
Finally, it will compile `mysqrt.cxx` which includes `Table.h` to produce the `MathFunctions` library.
## step 7: packaging an installer

## step 8: adding support for a testing dashboard
## step 9: selecting static or shared libraries
## step 10: adding generator expressions
## step 11: adding export configuration
## step 12: packaging debug and release