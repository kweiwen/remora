#pragma once

unsigned int firmware_upgrade (char* ldr_file, int size_of_ldr);

unsigned int coefficients_write (char* coefficients, int size_of_coefficients);
void coefficients_read (char* coefficients, int size_of_coefficients);
void float2char(float input_data, char* output_data);
float char2float(char* input_data);
