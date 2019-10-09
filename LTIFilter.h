/**
 * @file LTIFilter.h
 * @brief Class for implementing linear time-invariant discrete-time filters.
 * @author Dan Oates (WPI Class of 2020)
 * 
 * This class implements the following difference equation:
 * 
 * a[0] * y[n] + a[1] * y[n-1] + ... + a[A-1] * y[n-(A-1)] =
 * b[0] * x[n] + b[1] * x[n-1] + ... + b[B-1] * x[n-(B-1)]
 * 
 * Where y[n] and x[n] are zero for n < 0. After initialization
 * or reset, the filters output 0 until all past inputs x[n] to
 * x[n-(B-1)] are populated.
 * 
 * This library contains functions to making four basic first-
 * order filters:
 * 
 * - Low-pass filter
 * - High-pass filter
 * - Differentiator
 * - Integrator
 * 
 * This library avoids dynamic memory for variable-size filters
 * by pre-allocating fixed-size arrays for every instance. The
 * maximum sizes are defined by the following two macros:
 * 
 * LTIFILTER_MAX_A : Maximum value of A (>= 2)
 * LTIFILTER_MAX_B : Maximum value of B (>= 2)
 * 
 * If either of these macros is not defined at compile time or
 * defined as less than 2, a corresponding error message is
 * thrown by this H-file.
 * 
 * Dependencies:
 * - CppUtil: https://github.com/doates625/CppUtil.git
 * 
 * References:
 * - Digital Filters: https://en.wikipedia.org/wiki/Digital_filter
 */
#pragma once
#include <stdint.h>

/**
 * Macro Assignment Checks
 */

// Check MAX_A macro definition
#if !defined(LTIFILTER_MAX_A) || LTIFILTER_MAX_A < 2
#error Must define LTIFILTER_MAX_A as integer >= 2
#endif

// Check MAX_B macro definition
#if !defined(LTIFILTER_MAX_B) || LTIFILTER_MAX_B < 2
#error Must define LTIFILTER_MAX_B as integer >= 2
#endif

/**
 * Class Declaration
 */
class LTIFilter
{
public:

	// Class Routines
	LTIFilter(
		uint8_t A, float* a,
		uint8_t B, float* b);
	LTIFilter();
	float update(float xn);
	void reset();

	// Convolution Operation
	friend LTIFilter operator*(LTIFilter f1, LTIFilter f2);

	// Factory Routines
	static LTIFilter make_lpf(float f_cutoff, float f_sample);
	static LTIFilter make_hpf(float f_cutoff, float f_sample);
	static LTIFilter make_int(float f_sample);
	static LTIFilter make_dif(float f_sample);

protected:
	uint8_t A, B;
	uint8_t frame;
	float a[LTIFILTER_MAX_A];
	float b[LTIFILTER_MAX_B];
	float y[LTIFILTER_MAX_A];
	float x[LTIFILTER_MAX_B];
};

/**
 * Array Convolution Operation
 */
void conv(float* x1, uint8_t N1, float* x2, uint8_t N2, float* y);