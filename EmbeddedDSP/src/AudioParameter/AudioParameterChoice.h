#pragma once

#include "RangedAudioParameter.h"
#include <iostream>

class AudioParameterChoice : public RangedAudioParameter<int>
{
public:
	AudioParameterChoice(const char* parameterID, const char* parameterName, unsigned int size, int* choicesList, unsigned int deafultIndex) 
    : RangedAudioParameter<int>(parameterID, parameterName, 0, size-1), 
      def(deafultIndex),
      value(deafultIndex), 
      list(choicesList), 
      size(size)
	{
        checkInvariants();
        cc_value = convertToMIDICC(value);
	}

	~AudioParameterChoice()
	{
	}

	/** Value to be manipulated */
    int value;

	/** Default value for restore behavior */
    const int def;

    /** List to store index and value */
    const int* list;

    /** Size of list */
    unsigned int size;

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
    AudioParameterChoice& operator= (int newValue)
    {
        if (cc_value != newValue)
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

    int convertFromMIDICC(int v)
    {
        float proportion = clampTo0To1(v / 127.0f);
        int index = int(start + (end - start) * proportion);
        return list[index];
    }

private:
    void checkInvariants() const
    {
        assert(size > 0);
        assert(end >= value);
        assert(value >= start);
        //assert(slope > 0);
    }
};




