/**
 * @file LTIFilter.h
 * @brief Class for linear time-invariant discrete-time filters
 * @author Dan Oates (WPI Class of 2020)
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
	LTIFilter(uint8_t A, float* a, uint8_t B, float* b);
	LTIFilter();
	float update(float xn);
	void reset();

	// Convolution Operation
	friend LTIFilter operator*(LTIFilter f1, LTIFilter f2);

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
