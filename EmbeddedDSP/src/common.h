/*************************************************************************
	> File Name: common.h
	> Author: 
	> Mail: 
	> Created Time: Fri 19 Mar 2021 12:14:30 PM CST
 ************************************************************************/

#ifndef _COMMON_H
#define _COMMON_H
#include <stdbool.h>

/*define global constants */
#define  kSmallestPositiveFloatValue (1.175494351e-38)         /* min positive value */
#define  kSmallestNegativeFloatValue (-1.175494351e-38)         /* min negative value */
#define  kSqrtTwo (1.4142135623730950488016887242097)
#define  kMinFilterFrequency (20.0)
#define  kMaxFilterFrequency (20480.0) // 10 octaves above 20 Hz
#define  kPi (3.14159265358979323846264338327950288419716939937510582097494459230781640628620899)
#define  kTwoPi (2.0*3.14159265358979323846264338327950288419716939937510582097494459230781640628620899)



// common interface
float raw2dB(float raw);
float dB2Raw(float dB);
float peakGainFor_Q(float Q);
float dBPeakGainFor_Q(float Q);

float sgn(float xn);
float softClipWaveShaper(float xn, float saturation);

void boundValue(float* value, float minValue, float maxValue);
float doUnipolarModulationFromMin(float unipolarModulatorValue, float minValue, float maxValue);
float doUnipolarModulationFromMax(float unipolarModulatorValue, float minValue, float maxValue);

float unipolarToBipolar(float value);
float bipolarToUnipolar(float value);

float doLinearInterpolation(float y1, float y2, float fractional_X);


/* --- valve common*/
float calcMappedVariableOnRange(float inLow, float inHigh, 
								float outLow, float outHigh, 
								float control, bool convertFromDB);
#endif
