#include <math.h>

int factorial(int n) {
  return n*(n-1?factorial(n-1):1);
}
