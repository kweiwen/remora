#pragma once

#include "RangedAudioParameter.h"
#include <iostream>

class AudioParameterFloat : public RangedAudioParameter<float>
{
public:
    AudioParameterFloat(const char* parameterID, const char* parameterName, float minValue, float maxValue, float defaultValue, float skewValue = 1, bool isSymmetric = false) : RangedAudioParameter<float>(parameterID, parameterName, minValue, maxValue, skewValue, isSymmetric, false), def(defaultValue), value(defaultValue)
    {
        checkInvariants();
        cc_value = convertToMIDICC(value);
    }

    /** MIDI note numbers as defined in the MIDI standard and matched to the A4 */
    AudioParameterFloat(const char* parameterID, const char* parameterName, unsigned int defaulNote) : RangedAudioParameter<float>(parameterID, parameterName, 8.18f, 12543.85f, 1, false, true), def(convertFromMIDICC(defaulNote)), value(convertFromMIDICC(defaulNote))
    {
        checkInvariants();
        cc_value = defaulNote;
    }

	~AudioParameterFloat()
	{
	}

	/** Value to be manipulated */
    float value;

	/** Default value for restore behavior */
    const float def;

    /** Returns the parameter's current value. */
    operator float() const
    {
    	return value;
    }

    /** Direct access to set value. */
    void setValue(float newValue)
    {
    	value = newValue;
    }

    /** Changes the parameter's current value via midi. */
    AudioParameterFloat& operator= (int newValue)
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
