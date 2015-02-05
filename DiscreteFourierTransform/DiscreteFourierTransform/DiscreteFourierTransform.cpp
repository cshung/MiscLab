// DiscreteFourierTransform.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <cmath>
#include <iostream>

using namespace std;

// naive implementation of discrete fourier transform
// can be used efficiently for small inputs 
void discrete_fourier_transform(unsigned int length, double* input_real, double* input_imag, double* output_real, double* output_imag);

int _tmain(int argc, _TCHAR* argv[])
{
    double input_real[4];
    double input_imag[4];
    double output_real[4];
    double output_imag[4];
    input_real[0] = 1;
    input_real[1] = 2;
    input_real[2] = 3;
    input_real[3] = 4;
    input_imag[0] = 0;
    input_imag[1] = 0;
    input_imag[2] = 0;
    input_imag[3] = 0;
    discrete_fourier_transform(4, input_real, input_imag, output_real, output_imag);
    for (unsigned int i = 0; i < 4; i++)
    {
        cout << output_real[i] << " + " << output_imag[i] << "j" << endl;
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
            double angle = -2 * 3.1415926 / length * n * k;
            double complex_real = cos(angle);
            double complex_imag = sin(angle);
            output_real[k] += input_real[n] * complex_real - input_imag[n] * complex_imag;
            output_imag[k] += input_real[n] * complex_imag + input_imag[n] * complex_real;
        }
    }
}
