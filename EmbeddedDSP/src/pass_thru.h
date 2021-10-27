/*
 * pass_thru.h
 *
 *  Created on: Apr 12, 2021
 *      Author: Lava.kweiwen
 */

#ifndef PASS_THRU_H_
#define PASS_THRU_H_

#include <filter.h>

#define FFT_SIZE 1024
#define FFT_SIZE_DIV_2 512

#pragma align FFT_SIZE
float dm buffer_Real[FFT_SIZE];
float pm buffer_Imag[FFT_SIZE];

float dm temp_Real[FFT_SIZE];
float pm temp_Imag[FFT_SIZE];

float dm twiddle_Real[FFT_SIZE/2];
float pm twiddle_Imag[FFT_SIZE/2];

float raw_data[FFT_SIZE] = {
		#include "inputcomplx512.dat"
};

#endif /* PASS_THRU_H_ */
