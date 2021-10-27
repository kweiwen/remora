#pragma once

#ifndef PreferenceAudio
#define PreferenceAudio

const float EULER		= 2.71828182845904523536f;
const float LOG10E		= 0.43429448190325182765f;

const float HALF_PI		= 1.570796326794896f;
const float PI			= 3.141592653589793f;
const float TWO_PI		= 6.283185307179586f;

const float INV_HALF_PI = 0.636619772367582f;
const float INV_PI		= 0.318309886183791f;
const float INT_TWO_PI	= 0.159154943091896f;

const float SR_8K		= 8000.0f;
const float SR_11K025	= 11025.0f;
const float SR_16K		= 16000.0f;
const float SR_22K05	= 22100.0f;
const float SR_32K		= 32000.0f;
const float SR_44K1		= 44100.0f;
const float SR_48K		= 48000.0f;
const float SR_88K2		= 88200.0f;
const float SR_96K		= 96000.0f;
const float SR_17K64	= 176400.0f;
const float SR_19K2		= 192000.0f;
const float SR_35K28	= 352800.0f;
const float SR_38K4		= 384000.0f;

const float INV_SR_8K		= 0.00012500000000000000f;
const float INV_SR_11K025	= 0.00009070294784580499f;
const float INV_SR_16K		= 0.00006250000000000000f;
const float INV_SR_22K05	= 0.00004524886877828054f;
const float INV_SR_32K		= 0.00003125000000000000f;
const float INV_SR_44K1		= 0.00002267573696145125f;
const float INV_SR_48K		= 0.00002083333333333333f;
const float INV_SR_88K2		= 0.00001133786848072562f;
const float INV_SR_96K		= 0.00001041666666666667f;
const float INV_SR_17K64	= 0.00000566893424036281f;
const float INV_SR_19K2		= 0.00000520833333333333f;
const float INV_SR_35K28	= 0.00000283446712018141f;
const float INV_SR_38K4		= 0.00000260416666666667f;

const int BLOCK_SIZE_8		= 8;
const int BLOCK_SIZE_16		= 16;
const int BLOCK_SIZE_32		= 32;
const int BLOCK_SIZE_64		= 64;
const int BLOCK_SIZE_128	= 128;
const int BLOCK_SIZE_256	= 256;
const int BLOCK_SIZE_512	= 512;
#endif // !PreferenceAudio

#ifndef BLOCK_SIZE
#define BLOCK_SIZE (BLOCK_SIZE_512)
#endif // !BLOCK_SIZE

#ifndef SR
#define SR (SR_44K1)
#endif // !SR

#ifndef INV_SR
#define INV_SR (INV_SR_44K1)
#endif // !INV_SR
