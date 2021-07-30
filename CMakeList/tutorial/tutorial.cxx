#ifdef USE_MYMATH  // step 2
#  include "MathFunctions.h"
#endif

// A simple program that computes the square root of a number
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <string>
#include "TutorialConfig.h"  // STEP 1

int main(int argc, char* argv[])
{
  if (argc < 2) {
    // report version
    std::cout << argv[0] << " Version " << TUTORIAL_VERSION_MAJOR << "."
              << TUTORIAL_VERSION_MINOR << std::endl;
    std::cout << "Usage: " << argv[0] << " number" << std::endl;
    return 1;
  }

  // convert input to double
  const double inputValue = atof(argv[1]);

  // calculate square root
  #ifdef USE_MYMATH  // step 2
    const double outputValue = mysqrt(inputValue);
  #else
    const double outputValue = sqrt(inputValue);
  #endif

  std::cout << "The square root of " << inputValue << " is " << outputValue
            << std::endl;
  return 0;
}

