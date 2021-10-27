/*************************************************************************
	> File Name: common.c
	> Author: 
	> Mail: 
	> Created Time: Fri 19 Mar 2021 12:32:41 PM CST
 ************************************************************************/
#include <math.h>
#include <stdbool.h>

/**
@rawTo_dB
\ingroup FX-Functions

@brief calculates dB for given input

\param raw - value to convert to dB
\return the dB value
*/
float raw2dB(float raw)
{
	return 20.0*log10(raw);
}

/**
@dBTo_Raw
\ingroup FX-Functions

@brief converts dB to raw value

\param dB - value to convert to raw
\return the raw value
*/
float dB2Raw(float dB)
{
	return pow(10.0, (dB / 20.0));
}

/**
@peakGainFor_Q
\ingroup FX-Functions

@brief calculates the peak magnitude for a given Q

\param Q - the Q value
\return the peak gain (not in dB)
*/
float peakGainFor_Q(float Q)
{
	// --- no resonance at or below unity
	if (Q <= 0.707) return 1.0;
	return (Q*Q) / (pow((Q*Q - 0.25), 0.5));
}

/**
@dBPeakGainFor_Q
\ingroup FX-Functions

@brief calculates the peak magnitude in dB for a given Q

\param Q - the Q value
\return the peak gain in dB
*/
float dBPeakGainFor_Q(float Q)
{
	return raw2dB(peakGainFor_Q(Q));
}


/**
@sgn
\ingroup FX-Functions

@brief calculates sgn( ) of input
\param xn - the input value
\return -1 if xn is negative or +1 if xn is 0 or greater
*/
float sgn(float xn)
{
	return (xn > 0) - (xn < 0);
}

/**
@softClipWaveShaper
\ingroup FX-Functions

@brief calculates hyptan waveshaper
\param xn - the input value
\param saturation  - the saturation control
\return the waveshaped output value
*/
float softClipWaveShaper(float xn, float saturation)
{
	// --- un-normalized soft clipper from Reiss book
	return sgn(xn)*(1.0 - exp(-fabs(saturation*xn)));
}




/**
@boundValue
\ingroup FX-Functions

@brief  Bound a value to min and max limits

\param value - value to bound
\param minValue - lower bound limit
\param maxValue - upper bound limit
*/
void boundValue(float* value, float minValue, float maxValue)
{
	*value = fmin(*value, maxValue);
	*value = fmax(*value, minValue);
}

/**
@doUnipolarModulationFromMin
\ingroup FX-Functions

@brief Perform unipolar modulation from a min value up to a max value using a unipolar modulator value

\param unipolarModulatorValue - modulation value on range [0.0, +1.0]
\param minValue - lower modulation limit
\param maxValue - upper modulation limit
\return the modulated value
*/
float doUnipolarModulationFromMin(float unipolarModulatorValue, float minValue, float maxValue)
{
	// --- UNIPOLAR bound
	boundValue(&unipolarModulatorValue, 0.0, 1.0);

	// --- modulate from minimum value upwards
	return unipolarModulatorValue*(maxValue - minValue) + minValue;
}

/**
@doUnipolarModulationFromMax
\ingroup FX-Functions

@brief Perform unipolar modulation from a max value down to a min value using a unipolar modulator value

\param unipolarModulatorValue - modulation value on range [0.0, +1.0]
\param minValue - lower modulation limit
\param maxValue - upper modulation limit
\return the modulated value
*/
float doUnipolarModulationFromMax(float unipolarModulatorValue, float minValue, float maxValue)
{
	// --- UNIPOLAR bound
	boundValue(&unipolarModulatorValue, 0.0, 1.0);

	// --- modulate from maximum value downwards
	return maxValue - (1.0 - unipolarModulatorValue)*(maxValue - minValue);
}


/**
@unipolarToBipolar
\ingroup FX-Functions

@brief calculates the bipolar [-1.0, +1.0] value FROM a unipolar [0.0, +1.0] value

\param value - value to convert
\return the bipolar value
*/
float unipolarToBipolar(float value)
{
	return 2.0*value - 1.0;
}

/**
@bipolarToUnipolar
\ingroup FX-Functions

@brief calculates the unipolar [0.0, +1.0] value FROM a bipolar [-1.0, +1.0] value

\param value - value to convert
\return the unipolar value
*/
float bipolarToUnipolar(float value)
{
	return 0.5*value + 0.5;
}



/**
@doLinearInterpolation
\ingroup FX-Functions

@brief performs linear interpolation of fractional x distance between two adjacent (x,y) points;
returns interpolated value

\param y1 - the y coordinate of the first point
\param y2 - the 2 coordinate of the second point
\param x - the interpolation location as a fractional distance between x1 and x2 (which are not needed)
\return the interpolated value or y2 if the interpolation is outside the x interval
*/
float doLinearInterpolation(float y1, float y2, float fractional_X)
{
	// --- check invalid condition
	if (fractional_X >= 1.0) return y2;

	// --- use weighted sum method of interpolating
	return fractional_X*y2 + (1.0 - fractional_X)*y1;
}



/* ----------- valve */
// --- helper
float calcMappedVariableOnRange(float inLow, float inHigh, 
								float outLow, float outHigh, 
								float control, bool convertFromDB)
{
	// --- mapper 
	float slope = (outHigh - outLow) / (inHigh - inLow);
	float yn = outLow + slope * (control - inLow);
	if (convertFromDB)
		return pow(10.0, yn / 20.0);
	else
		return yn;
}







