/*
 * Copyright (c) 2018 Analog Devices, Inc.  All rights reserved.
 * 
 * This file includes various misc audio processing functions used in audio 
 * processing.
 */
#include <math.h>
#include "audio_elements_common.h"
#include "audio_utilities.h"

/**
 * @brief Clears a fixed-point audio buffer
 *
 * @param buffer Pointer to floating point buffer to clear (mono)
 * @param audio_block_size The number of floating-point words to process in the buffer
 */
#pragma optimize_for_speed
void    clear_buffer_int(int * buffer, uint32_t audio_block_size) {
    for (int i=0;i<audio_block_size;i++) {
        buffer[i] = 0;
    }
}

/**
 * @brief Clears a floating-point audio buffer
 * 
 * @param buffer Pointer to floating point buffer to clear (mono)
 * @param audio_block_size The number of floating-point words to process in the buffer
 */
#pragma optimize_for_speed
void    clear_buffer(float * buffer, uint32_t audio_block_size) {
    for (int i=0;i<audio_block_size;i++) {
        buffer[i] = 0.0;
    }
}

/**
 * @brief Copy a fixed-point audio buffer
 * 
 * @param input Pointer to source floating point buffer
 * @param output Pointer to destination floating point buffer
 * @param audio_block_size The number of floating-point words to process in the buffer
 */
#pragma optimize_for_speed
void copy_buffer_int(int * input, int * output, uint32_t audio_block_size) {
    for (int i=0;i<audio_block_size;i++) {
        output[i] = input[i];
    }
}

/**
 * @brief Copy a floating-point audio buffer
 *
 * @param input Pointer to source floating point buffer
 * @param output Pointer to destination floating point buffer
 * @param audio_block_size The number of floating-point words to process in the buffer
 */
#pragma optimize_for_speed
void copy_buffer(float * input, float * output, uint32_t audio_block_size) {
    for (int i=0;i<audio_block_size;i++) {
        output[i] = input[i];
    }
}

/**
 * @brief Scale a floating-point buffer by a gain value
 * 
 * @param buffer Pointer to floating point buffer to scale (mono)
 * @param gain Gain value
 * @param audio_block_size The number of floating-point words to process in the buffer
 */
#pragma optimize_for_speed
void    gain_buffer_int(int * buffer, int gain, uint32_t audio_block_size) {
    for (int i=0;i<audio_block_size;i++) {
        buffer[i] *= gain;
    }
}

/**
 * @brief Scale a floating-point buffer by a gain value
 *
 * @param buffer Pointer to floating point buffer to scale (mono)
 * @param gain Gain value
 * @param audio_block_size The number of floating-point words to process in the buffer
 */
#pragma optimize_for_speed
void    gain_buffer(float * buffer, float gain, uint32_t audio_block_size) {
    for (int i=0;i<audio_block_size;i++) {
        buffer[i] *= gain;
    }
}

/**
 * @brief mix two fixed-point buffer into one buffer
 *
 * @param output Pointer to destination floating point buffer
 * @param input1 Pointer to source floating point buffer
 * @param input2 Pointer to source floating point buffer
 * @param audio_block_size The number of floating-point words to process in the buffer
 */
#pragma optimize_for_speed
void    mix_buffer_int(int * output, int * input1, int * input2, uint32_t audio_block_size) {
    for (int i=0;i<audio_block_size;i++) {
        output[i] = input1[i] + input2[i];
    }
}

/**
 * @brief mix two floating-point buffer into one buffer
 *
 * @param output Pointer to destination floating point buffer
 * @param input1 Pointer to source floating point buffer
 * @param input2 Pointer to source floating point buffer
 * @param audio_block_size The number of floating-point words to process in the buffer
 */
#pragma optimize_for_speed
void    mix_buffer(float * output, float * input1, float * input2, uint32_t audio_block_size) {
    for (int i=0;i<audio_block_size;i++) {
        output[i] = input1[i] + input2[i];
    }
}

