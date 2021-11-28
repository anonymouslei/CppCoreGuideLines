#include "MathFunctions.h"
#include <iostream>
#include "Table.h"  // step 6

// a hack square root calculation using simple operations
double mysqrt(double x) {
  if (x <= 0) {
    return 0;
  }

/* 
remove in step 6
#if defined(HAVE_LOG) && defined(HAVE_EXP)  // step 5
  double result = exp(log(x) * 0.5);
  std::cout << "Computing sqrt of " << x << " to be " << result
            << " using log and exp" << std::endl;
#else  // step 5
*/
  double result = x;

  // step 6
  if (x >= 1 && x < 10) {
    std::cout << "use the table to help find an initial value"
    << std::endl;
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
// #endif  // step 5  // remove in step 6

  return result;
}