#pragma once

#include <stdlib.h>

class AudioParameter
{
public:
	AudioParameter() 
	{
		cc_value = NULL;
		paramID = NULL;
		name = NULL;
	}

	AudioParameter(const char* pid, const char* nm) : paramID(pid), name(nm)
	{
		cc_value = NULL;
	}

	virtual ~AudioParameter()
	{
	}

	virtual AudioParameter& operator= (int newValue) = 0;

	/** Value to be display in midi format */
	unsigned int cc_value;

    /** Provides access to the parameter's ID string. */
	const char* paramID;

    /** Provides access to the parameter's name. */
	const char* name;
};
