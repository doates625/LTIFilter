/**
 * @file LTIFilter.cpp
 * @author Dan Oates (WPI Class of 2020)
 */
#include "LTIFilter.h"
#include <CppUtil.h>
#include <math.h>



/**
 * @brief Constructs LTI filter with given coefficients
 * @param A Number of y-coefficients (minimum 1)
 * @param a Array of y-coefficients (ordered a[k], k = 0 ... A-1)
 * @param B Number of x-coefficients (minimum 1)
 * @param b Array of x-coefficients (ordered b[k], k = 0 ... B-1)
 */
LTIFilter::LTIFilter(uint8_t A, float* a, uint8_t B, float* b)
{
	// Copy array sizes
	this->A = A;
	this->B = B;

	// Copy coefficients
	for (uint8_t k = 0; k < A; k++) this->a[k] = a[k];
	for (uint8_t k = 0; k < B; k++) this->b[k] = b[k];

	// Run filter reset routine
	reset();
}

/**
 * @brief Initializes filter
 * 
 * Call once before any calls to update().
 */
void LTIFilter::init()
{
	const float scale = 1.0f / a[0];
	for (uint8_t k = 0; k < A; k++) a[k] = a[k] * scale;
	for (uint8_t k = 0; k < B; k++) b[k] = b[k] * scale;
}

/**
 * @brief Constructs pass-through filter y[n] = x[n]
 */
LTIFilter::LTIFilter()
{
	this->A = 1;
	this->B = 1;
	this->a[0] = 1.0f;
	this->b[0] = 1.0f;
	reset();
}

/**
 * @brief Inputs x[n] and outputs y[n]
 * @param xn Input x[n]
 * @return y[n]
 * 
 * Returns 0 until all x buffers are full.
 */
float LTIFilter::update(float xn)
{
	// Time-shift buffers of x[n] and y[n]
	for(uint8_t n = A - 1; n > 0; n--) y[n] = y[n - 1];
	for(uint8_t n = B - 1; n > 0; n--) x[n] = x[n - 1];

	// Put new x[n] in buffer
	x[0] = xn;

	// Calculate and buffer y[n]
	float yn = 0.0f;
	for(uint8_t k = 0; k < B; k++) yn += b[k] * x[k];
	for(uint8_t k = 1; k < A; k++) yn -= a[k] * y[k];
	y[0] = yn;

	// Return zeros until all x[n] are filled
	return (++frame < B) ? 0.0f : yn;
}

/**
 * @brief Resets filter (all past inputs and outputs to 0)
 */
void LTIFilter::reset()
{
	for(uint8_t n = 0; n < A; n++) y[n] = 0.0f;
	for(uint8_t n = 0; n < B; n++) x[n] = 0.0f;
	frame = 1;
}

/**
 * @brief Convolves two discrete filters in the time domain
 */
LTIFilter operator*(LTIFilter f1, LTIFilter f2)
{
	// Allocate data
	const uint8_t A = f1.A + f2.A - 1;
	const uint8_t B = f1.B + f2.B - 1;
	float a[LTIFILTER_MAX_A];
	float b[LTIFILTER_MAX_B];

	// Convolve filters
	conv(f1.a, f1.A, f2.a, f2.A, a);
	conv(f1.b, f1.B, f2.b, f2.B, b);

	// Construct filter
	return LTIFilter(A, a, B, b);
}

/**
 * @brief Performs linear convolution y = x1 * x2
 * @param x1 Array to convolve (length >= 1)
 * @param x2 Array to convolve (length >= 1)
 * @param N1 length of x1 (>= 1)
 * @param N2 length of x2 (>= 1)
 * @param y Pointer to output array (length >= N1 + N2 - 1)
 */
void conv(float* x1, uint8_t N1, float* x2, uint8_t N2, float* y)
{
	uint8_t N = N1 + N2 - 1;
	for (uint8_t n = 0; n < N; n++)
	{
		y[n] = 0.0f;
		uint8_t k_min = CppUtil::max(0, (int)(n - (N2 - 1)));
		uint8_t k_max = CppUtil::min(N1 - 1, (int)n);
		for (uint8_t k = k_min; k <= k_max; k++)
		{
			y[n] += x1[k] * x2[n - k];
		}
	}
}
