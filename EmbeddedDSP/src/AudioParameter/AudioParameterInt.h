#pragma once

#include "RangedAudioParameter.h"
#include <iostream>

class AudioParameterInt : public RangedAudioParameter<int>
{
public:
	AudioParameterInt(const char* parameterID, const char* parameterName, int minValue, int maxValue, int defaultValue, float skewValue = 1, bool isSymmetric = false) : RangedAudioParameter<int>(parameterID, parameterName, minValue, maxValue, skewValue, isSymmetric, false), def(defaultValue), value(defaultValue)
	{
		checkInvariants();
		cc_value = convertToMIDICC(value);
	}

    ~AudioParameterInt()
    {
    }

	/** Value to be manipulated */
    int value;

	/** Default value for restore behavior */
    const int def;

    /** Returns the parameter's current value. */
    operator int() const
    {
    	return value;
    }

    /** Direct access to set value. */
    void setValue(int newValue)
    {
    	value = newValue;
    }

    /** Changes the parameter's current value via midi. */
    AudioParameterInt& operator= (int newValue)
    {
        if(cc_value != newValue)
        {
        	cc_value = clampTo0To127(newValue);
        	value = convertFromMIDICC(cc_value);
        }
        return *this;
    }

    /** Reset parameter to default value. */
    void resetToDefaultValue()
    {
    	value = def;
    	cc_value = convertToMIDICC(value);
    }

private:
    void checkInvariants() const
    {
    	assert(end >= value);
    	assert(value >= start);
    	assert(skew > 0);
    }
};
