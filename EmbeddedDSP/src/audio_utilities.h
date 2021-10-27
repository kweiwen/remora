#ifndef _AUDIO_UTILITIES_H
#define _AUDIO_UTILITIES_H

#include <stdint.h>

// Wrapper allows C code to be called from C++ files
#if __cplusplus
extern "C" {
#endif

// Basic audio buffer utilities
void    clear_buffer_int(int * buffer, uint32_t audio_block_size);
void    clear_buffer(float * buffer, uint32_t audio_block_size);

void    copy_buffer_int(int * input, int * output, uint32_t audio_block_size);
void    copy_buffer(float * input, float * output, uint32_t audio_block_size);

void    gain_buffer_int(int * buffer, int gain, uint32_t audio_block_size);
void    gain_buffer(float * buffer, float gain, uint32_t audio_block_size);

void 	mix_buffer_int(int * output, int * input1, int * input2, uint32_t audio_block_size);
void 	mix_buffer(float * output, float * input1, float * input2, uint32_t audio_block_size);


// Wrapper allows C code to be called from C++ files
#if __cplusplus
}
#endif

#endif  // _AUDIO_UTILITIES_H
