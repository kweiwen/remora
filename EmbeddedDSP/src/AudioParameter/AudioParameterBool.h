#pragma once

#include "AudioParameter.h"
#include <iostream>

class AudioParameterBool : public AudioParameter
{
public:
	AudioParameterBool(const char* parameterID, const char* parameterName, bool defaultValue) : AudioParameter(parameterID, parameterName), def(defaultValue), value(defaultValue)
	{
		cc_value = (value == true) ? 127 : 0;
	}

	~AudioParameterBool()
	{
	}


	/** Value to be manipulated */
    bool value;

	/** Default value for restore behavior */
    const bool def;

    /** Returns the parameter's current value. */
    operator bool() const
    {
    	return value;
    }

    /** Direct access to set value. */
    void setValue(bool newValue)
    {
    	value = newValue;
    }

    /** Changes the parameter's current value via midi. */
    AudioParameterBool& operator= (int newValue)
    {
        if(cc_value != newValue)
        {
        	cc_value = newValue;
        	value = (cc_value < 64) ? false : true;
        }
        return *this;
    }

    /** Reset parameter to default value. */
    void resetToDefaultValue()
    {
    	value = def;
    	cc_value = (value == true) ? 127 : 0;
    }
};
