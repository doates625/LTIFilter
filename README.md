# LTIFilter
Class for linear time-invariant discrete-time filters  
Written by Dan Oates (WPI Class of 2020)

### Description
This class implements the following difference equation:  
  
```
a[0] * y[n] + a[1] * y[n-1] + ... + a[A-1] * y[n-(A-1)] =
b[0] * x[n] + b[1] * x[n-1] + ... + b[B-1] * x[n-(B-1)]
```
  
Where y[n] and x[n] are zero for n < 0. After initialization or reset, the filters output 0 until all past inputs x[n] to x[n-(B-1)] are populated.
  
This library contains functions to making four basic first-order filters:

- Low-pass filter
- High-pass filter
- Differentiator
- Integrator

This library avoids dynamic memory for variable-size filters by pre-allocating fixed-size arrays for every instance. The maximum sizes are defined by the following two macros:
  
  LTIFILTER_MAX_A : Maximum value of A (>= 2)  
  LTIFILTER_MAX_B : Maximum value of B (>= 2)
  
If either of these macros is not defined at compile time or defined as less than 2, a corresponding error message is thrown by this H-file.

### Dependencies
- [CppUtil](https://github.com/doates625/CppUtil.git)