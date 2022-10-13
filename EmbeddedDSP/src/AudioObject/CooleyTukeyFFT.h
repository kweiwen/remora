#pragma once

#include <complex>
#include <iostream>
#include <valarray>

# define M_PI          3.141592653589793238462643383279502884 /* pi */

class divid_N_conquer
{
public:
    void fft(std::valarray<std::complex<float>>& x)
    {
        int N = x.size();
        if (N <= 1)
        {
            return;
        }
        else
        {
            // divide
            std::valarray<std::complex<float>> even = x[std::slice(0, N / 2, 2)];
            std::valarray<std::complex<float>> odd = x[std::slice(1, N / 2, 2)];

            // conquer
            fft(even);
            fft(odd);

            // combine
            for (int k = 0; k < N / 2; ++k)
            {
                std::complex<float> t = std::complex<float>(std::polar(1.0, -2 * M_PI * k / N)) * odd[k];
                x[k] = even[k] + t;
                x[k + N / 2] = even[k] - t;
            }
        }
    }

    void ifft(std::valarray<std::complex<float>>& x)
    {
        x = x.apply(std::conj);
        fft(x);
        x = x.apply(std::conj);

        x /= x.size();
    }
};

class twiddle
{

public:
    // address of cell if base adderss not nullptr, nullptr otherwise
    #define fft_private_safe_addrof(ptr,i) ((ptr!=nullptr)?(&(ptr[i])):(nullptr))

    // For a 8-sample input, the FFT's last three bins contain "negative" frequencies. (So, the last (size/2)-1 bins.) They are only meaningful for complex inputs.
    void fft_core(double* input_real, double* input_imag, uint64_t size, uint64_t gap, double* output_real, double* output_imag, bool forwards)
    {
        if (size == 1)
        {
            output_real[0] = input_real[0];
            if (input_imag != nullptr)
                output_imag[0] = input_imag[0];
            else
                output_imag[0] = 0;
        }
        else
        {
            // This algorithm works by extending the concept of how two-bin DFTs (discrete fourier transform) work, in order to correlate decimated DFTs, recursively.
            // No, I'm not your guy if you want a proof of why it works, but it does.
            fft_core(input_real, input_imag, size / 2, gap * 2, output_real, output_imag, forwards);
            fft_core(&(input_real[gap]), fft_private_safe_addrof(input_imag, gap), size / 2, gap * 2, &(output_real[size / 2]), &(output_imag[size / 2]), forwards);
            // non-combed decimated output to non-combed correlated output
            for (uint64_t i = 0; i < size / 2; i++)
            {
                double a_real = output_real[i];
                double a_imag = output_imag[i];
                double b_real = output_real[i + size / 2];
                double b_imag = output_imag[i + size / 2];

                double twiddle_real = cos(2 * M_PI * i / size);
                double twiddle_imag = sin(2 * M_PI * i / size) * (forwards ? -1 : 1);
                // complex multiplication (vector angle summing and length multiplication)
                double bias_real = b_real * twiddle_real - b_imag * twiddle_imag;
                double bias_imag = b_imag * twiddle_real + b_real * twiddle_imag;
                // real output (sum of real parts)
                output_real[i] = a_real + bias_real;
                output_real[i + size / 2] = a_real - bias_real;
                // imag output (sum of imaginary parts)
                output_imag[i] = a_imag + bias_imag;
                output_imag[i + size / 2] = a_imag - bias_imag;
            }
        }
    }

    void normalize_fft(double* input_real, double* input_imag, uint64_t size)
    {
        for (uint64_t i = 0; i < size; i++)
        {
            input_real[i] /= size;
            input_imag[i] /= size;
        }
    }
    void half_normalize_fft(double* input_real, double* input_imag, uint64_t size)
    {
        for (uint64_t i = 0; i < size; i++)
        {
            input_real[i] /= sqrt(size);
            input_imag[i] /= sqrt(size);
        }
    }
    void fft(double* input_real, double* input_imag, uint64_t size, double* output_real, double* output_imag)
    {
        fft_core(input_real, input_imag, size, 1, output_real, output_imag, 1);
    }
    void fft_normalize(double* input_real, double* input_imag, uint64_t size, double* output_real, double* output_imag)
    {
        fft_core(input_real, input_imag, size, 1, output_real, output_imag, 1);
        half_normalize_fft(output_real, output_imag, size); // allows calling fft() four times to result in the original signal with no amplitude change
    }
    void ifft(double* input_real, double* input_imag, uint64_t size, double* output_real, double* output_imag)
    {
        fft_core(input_real, input_imag, size, 1, output_real, output_imag, 0);
    }
    void ifft_normalize(double* input_real, double* input_imag, uint64_t size, double* output_real, double* output_imag)
    {
        fft_core(input_real, input_imag, size, 1, output_real, output_imag, 0);
        half_normalize_fft(output_real, output_imag, size); // see above, also causes ifft(fft(x)) to result in the original signal with no amplitude change
    }

    // boost bins that are split into positive (A-handed spin) and negative (B-handed spin) parts
    // only useful if former input signal was not complex, for only needing to look at one bin to get the magnitude
    // FIXME or HELPME: How come the nyquist frequency is quiet in saw waves, but loud in pure signal?
    void sanitize_fft(double* input_real, double* input_imag, uint64_t size)
    {
        for (uint64_t i = 1; i < size / 2; i++)
        {
            input_real[i] *= 2;
            input_imag[i] *= 2;
            input_real[size - i] *= 2;
            input_imag[size - i] *= 2;
        }
    }
    // opposite of above
    void unsanitize_fft(double* input_real, double* input_imag, uint64_t size)
    {
        for (uint64_t i = 1; i < size / 2; i++)
        {
            input_real[i] /= 2;
            input_imag[i] /= 2;
            input_real[size - i] /= 2;
            input_imag[size - i] /= 2;
        }
    }
};