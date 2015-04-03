#include "stdafx.h"

#include <cmath>
#include <iostream>

using namespace std;

#define M_PI 3.1415926

// naive implementation of discrete fourier transform
// can be used efficiently for small inputs 
void discrete_fourier_transform(unsigned int length, double* input_real, double* input_imag, double* output_real, double* output_imag);

// my first implementation of fast fourier transform
// note that length has to be a power of 2
void fast_fourier_transform(unsigned int length, double* input_real, double* input_imag, double* output_real, double* output_imag, double* temp_real, double* temp_imag);

int _tmain(int argc, _TCHAR* argv[])
{
    const unsigned int length = 64;
    double input_real[length];
    double input_imag[length];
    double output_real[length];
    double output_imag[length];
    double temp_real[length];
    double temp_imag[length];
    for (unsigned int i = 0; i < length; i++)
    {
        input_real[i] = 2 * i;
        input_imag[i] = 2 * i + 1;
    }

    // discrete_fourier_transform(length, input_real, input_imag, output_real, output_imag);
    fast_fourier_transform(length, input_real, input_imag, output_real, output_imag, temp_real, temp_imag);
    for (unsigned int i = 0; i < length; i++)
    {
        cout << output_real[i] << " " << output_imag[i] << "j" << endl;
        // cout << input_real[i] << " " << input_imag[i] << "j" << endl;
    }
    return 0;
}

void discrete_fourier_transform(unsigned int length, double* input_real, double* input_imag, double* output_real, double* output_imag)
{
    for (unsigned int k = 0; k < length; k++)
    {
        output_real[k] = 0;
        output_imag[k] = 0;
        for (unsigned int n = 0; n < length; n++)
        {
            double angle = -2 * M_PI / length * n * k;
            double complex_real = cos(angle);
            double complex_imag = sin(angle);
            output_real[k] += input_real[n] * complex_real - input_imag[n] * complex_imag;
            output_imag[k] += input_real[n] * complex_imag + input_imag[n] * complex_real;
        }
    }
}

void fast_fourier_transform(unsigned int length, double* input_real, double* input_imag, double* output_real, double* output_imag, double* temp_real, double* temp_imag)
{
    // Step 1: Base case handling
    if (length == 1)
    {
        output_real[0] = input_real[0];
        output_imag[0] = input_imag[0];
        return;
    }

    // Step 1: Decimation in time
    unsigned int half = length / 2;
    for (unsigned int i = 0; i < half; i++)
    {
        temp_real[i] = input_real[2 * i];
        temp_imag[i] = input_imag[2 * i];
        temp_real[i + half] = input_real[2 * i + 1];
        temp_imag[i + half] = input_imag[2 * i + 1];
    }
    for (unsigned int i = 0; i < length; i++)
    {
        input_real[i] = temp_real[i];
        input_imag[i] = temp_imag[i];
    }

    // Step 2: Recursive calls
    fast_fourier_transform(half, input_real       , input_imag       , temp_real       , temp_imag       , output_real       , output_imag       );
    fast_fourier_transform(half, input_real + half, input_imag + half, temp_real + half, temp_imag + half, output_real + half, output_imag + half);

    // Step 3: Merge results
    for (unsigned int i = 0; i < half; i++)
    {
        double angle = -2 * M_PI / length * i;
        double cosine = cos(angle);
        double sine = sin(angle);
        output_real[i]        = temp_real[i] + temp_real[i + half] * cosine - temp_imag[i + half] * sine  ;
        output_imag[i]        = temp_imag[i] + temp_real[i + half] * sine   + temp_imag[i + half] * cosine;
        output_real[i + half] = temp_real[i] - temp_real[i + half] * cosine + temp_imag[i + half] * sine  ;
        output_imag[i + half] = temp_imag[i] - temp_real[i + half] * sine   - temp_imag[i + half] * cosine;
    }

    // Step 4: (Optional) restore input
    for (unsigned int i = 0; i < length; i++)
    {
        temp_real[i] = input_real[i];
        temp_imag[i] = input_imag[i];
    }
    for (unsigned int i = 0; i < half; i++)
    {
        input_real[2 * i] = temp_real[i];
        input_imag[2 * i] = temp_imag[i];
        input_real[2 * i + 1] = temp_real[i + half];
        input_imag[2 * i + 1] = temp_imag[i + half];
    }
}