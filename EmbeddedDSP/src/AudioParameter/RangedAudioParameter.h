#pragma once

#include "AudioParameter.h"
#include "../AudioPreference.h"
#include <assert.h>
#include <cmath>
#include <iostream>
#include <limits.h>


template <typename ValueType>
class RangedAudioParameter : public AudioParameter
{
public:
	RangedAudioParameter(const char* parameterID, const char* parameterName, ValueType rangeStart, ValueType rangeEnd, float skewValue = 1, bool isSymmetric = false, bool isOctaveCurve = false) : AudioParameter(parameterID, parameterName), start(rangeStart), end(rangeEnd), skew(skewValue), octaveCurve(isOctaveCurve), symmetricSkew(isSymmetric), slope(LOG10E / skew)
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

    /** Uses the properties of this mapping to convert a non-normalised value to
        its 0->127 representation.
    */
	unsigned int convertToMIDICC(ValueType v)
    {
        if (octaveCurve)
        {
            float middleA = (std::log10(v / 440.0f) / std::log10(2.0f)) * 12.0f;
            return (unsigned int)middleA + 69;
        }
        else
        {
            float proportion = 0;
            proportion = clampTo0To1((static_cast<float>(v) - static_cast<float>(start)) / (static_cast<float>(end) - static_cast<float>(start)));
            if (skew == 1)
            {
                proportion = clampTo0To1((static_cast<float>(v) - static_cast<float>(start)) / (static_cast<float>(end) - static_cast<float>(start)));
            }
            else
            {
                float roots = std::log10(static_cast<float>(v - start) / static_cast<float>(end - start)) * slope / LOG10E;
                proportion = std::pow(10.0f, roots);
            }
            return (unsigned int)(proportion * 127);
        }
    }

    /** Uses the properties of this mapping to convert a normalised 0->127 value to
        its full-range representation.
    */
    ValueType convertFromMIDICC(int v)
    {
        if (octaveCurve)
        {
            float octave = std::pow(2.0f, static_cast<float>(v - 69) / 12.0f);
            return ValueType (octave * 440.0f);
        }
        else
        {
            float proportion = clampTo0To1(v / 127.0f);
            if (skew != 1.0f)
            {
                float temp = std::log10(proportion) / slope;
                proportion = std::exp(temp);
            }
            return ValueType (start + (end - start) * proportion);
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
    	assert(skew > ValueType());
    }
};
