#pragma once

#include "AudioParameter.h"
#include "../Utilities/AudioPreference.h"
#include <assert.h>
#include <cmath>
#include <iostream>
#include <limits.h>
#include "../AudioParameter/RangedAudioParameter.h"

template <typename ValueType>
class RangedAudioParameter : public AudioParameter
{
public:
	RangedAudioParameter(const char* parameterID, const char* parameterName, ValueType rangeStart, ValueType rangeEnd, float skewValue = 0, bool isSymmetric = false, bool isOctaveCurve = false)
    : AudioParameter(parameterID, 
      parameterName), 
      start(rangeStart), 
      end(rangeEnd), 
      skew(skewValue), 
      octaveCurve(isOctaveCurve), 
      symmetricSkew(isSymmetric), 
      slope(LOG10E/std::pow(10, skew))
	{
		checkInvariants();
	}

    ~RangedAudioParameter()
    {
    }

	/** Start of the range */
	const ValueType start;

	/** End of the range */
	const ValueType end;

    /** Define a sweet point of the range, which corresponded the middle value of MIDI  */
	const ValueType sweetPoint = (static_cast<float>(end) - static_cast<float>(start)) * 0.5;
	
    /** An optional skew factor that alters the way values are distribute across the range.
	  	A factor of 1.0 has no skewing effect at all. If the factor is < 1.0, the lower end
        of the range will fill more of the slider's length; if the factor is > 1.0, the upper
        end of the range will be expanded.
	*/
	const float skew;

	/** Symmetrical skew factor. */
	const bool symmetricSkew;

	/** Defined the curve follow MIDI standard. */
	bool octaveCurve;

    unsigned int setSweetPoint(ValueType v) 
    {
        sweetPoint = static_cast<float>(v);
        return 0;
    }
    /** Uses the properties of this mapping to convert a non-normalised value to
        its 0->127 representation.
    */
	unsigned int convertToMIDICC(ValueType v)
    {
        float proportion = 0;
        if (octaveCurve) 
        {
            float middleA = (std::log10(v / 440.0f) / std::log10(2.0f)) * 12.0f;
            return (unsigned int)middleA + 69;
        }
        else if (symmetricSkew == true) 
        {
            
            if( static_cast<float>(v) < static_cast<float>(sweetPoint) ) 
            {
                proportion = static_cast<float>(v) * 0.5 / static_cast<float>(sweetPoint) ;
            } 
            else 
            {
                proportion  = 0.5 + (static_cast<float>(v) - static_cast<float>(sweetPoint) )*(0.5 / (static_cast<float>(end) - static_cast<float>(sweetPoint))); 
            }

            proportion = 2 * proportion -1;

            if( proportion < 0) 
            {
                proportion = 1 + std::pow(std::abs(proportion), LOG10E/slope) *(-1);
            }
            else 
            {
                proportion = 1 + std::pow(std::abs(proportion), LOG10E/slope);
            }
            proportion = proportion*0.5*127;
            return  (int)proportion;
            
        }
        else 
        {
            proportion = clampTo0To1((static_cast<float>(v) - static_cast<float>(start)) / (static_cast<float>(end) - static_cast<float>(start)));
            float roots = std::log10(static_cast<float>(v - start) / static_cast<float>(end - start) + MIN_VAL) * slope / LOG10E;
            proportion = std::pow(10.0f, roots);
            return (unsigned int)(proportion * 127);
        }
    }

    /** Uses the properties of this mapping to convert a normalised 0->127 value to
        its full-range representation.
    */
    ValueType convertFromMIDICC(int v)
    {
        float proportion = 0;

        if (octaveCurve)
        {
            float octave = std::pow(2.0f, static_cast<float>(v - 69) / 12.0f);
            return ValueType (octave * 440.0f);
        }
        else if(symmetricSkew == true && skew !=0)
        {
//        	printf("slope = %f ", slope);
            float x = 0;
            x =  clampTo0To1(v * 0.007874); // 1/127 = 0.007874 
            x = 2 * x -1;
            proportion = std::exp(std::log(std::abs(x + MIN_VAL)) / (LOG10E/slope));
            if( x < 0)
            {
                proportion = proportion*(-1) +1;  
                proportion = start + (sweetPoint - start) * proportion;               
            }else {
                proportion = sweetPoint + (end - sweetPoint) * proportion;   

            }
            return ValueType (proportion);
        }
        else
        {
            proportion = clampTo0To1(v * 0.007874);
            float temp = std::log10(proportion + MIN_VAL) / slope;
            proportion = std::exp(temp);
            return ValueType(start + (end - start) * proportion);
        }
    }

    /** To make sure the value is clamp between 0.0 - 1.0, dedicated to proportion format.
    */
    static float clampTo0To1(float value)
    {
    	float lowerLimit = 0;
    	float upperLimit = 1;
    	return value < lowerLimit ? lowerLimit : (upperLimit < value ? upperLimit : value);
    }

    /** To make sure the value is clamp between 0 - 127, dedicated to midi format.
    */
    static unsigned int clampTo0To127(int value)
    {
    	int lowerLimit = 0;
    	int upperLimit = 127;
    	return value < lowerLimit ? lowerLimit : (upperLimit < value ? upperLimit : value);
    }


private:
	/** Another factor to express skew factor */
    const float slope;

    void checkInvariants() const
    {
    	assert(end > start);
    	assert(skew > -2);
    	assert(2 > skew);
    }
};
