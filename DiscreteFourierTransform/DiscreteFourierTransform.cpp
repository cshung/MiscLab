#include <cmath>
#include <iostream>
#include <iomanip>

using namespace std;

#define M_PI 3.1415926

// naive implementation of discrete fourier transform
// can be used efficiently for small inputs
void discrete_fourier_transform(unsigned int length, double* input_real, double* input_imag, double* output_real, double* output_imag);

// fast fourier transform, note that length has to be a power of 2
void fast_fourier_transform(unsigned int length, int input_offset, int input_stride, double* input_real, double* input_imag, int output_offset, int output_stride, double* output_real, double* output_imag);

int main(int argc, char** argv)
{
    const unsigned int length = 64;
    double input_real[length];
    double input_imag[length];
    double output_real[length];
    double output_imag[length];
    for (unsigned int i = 0; i < length; i++)
    {
        input_real[i] = 2 * i;
        input_imag[i] = 2 * i + 1;
    }

    cout << fixed << setprecision(2);

    if (argc == 1)
    {
        discrete_fourier_transform(length, input_real, input_imag, output_real, output_imag);
    }
    else
    {
        fast_fourier_transform(length, 0, 1, input_real, input_imag, 0, 1, output_real, output_imag);
    }
    for (unsigned int i = 0; i < length; i++)
    {
        cout << output_real[i] << " " << output_imag[i] << "j" << endl;
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

void fast_fourier_transform(unsigned int length, int input_offset, int input_stride, double* input_real, double* input_imag, int output_offset, int output_stride, double* output_real, double* output_imag)
{
    // Step 1: Base case handling
    if (length == 1)
    {
        output_real[output_offset] = input_real[input_offset];
        output_imag[output_offset] = input_imag[input_offset];
        return;
    }

    // Step 2: Recursive calls
    unsigned int half = length / 2;
    fast_fourier_transform(half, input_offset               , input_stride * 2, input_real, input_imag, output_offset       , output_stride, output_real, output_imag);
    fast_fourier_transform(half, input_offset + input_stride, input_stride * 2, input_real, input_imag, output_offset + half, output_stride, output_real, output_imag);

    // Step 3: Merge results
    for (unsigned int i = 0; i < half; i++)
    {
        double angle = -2 * M_PI / length * i;
        double cosine = cos(angle);
        double sine = sin(angle);

        double a_real = output_real[output_offset + i * output_stride];
        double a_imag = output_imag[output_offset + i * output_stride];
        double b_real = output_real[output_offset + (i + half) * output_stride];
        double b_imag = output_imag[output_offset + (i + half) * output_stride];

        double c_real = a_real + b_real * cosine - b_imag * sine  ;
        double c_imag = a_imag + b_real * sine   + b_imag * cosine;
        double d_real = a_real - b_real * cosine + b_imag * sine  ;
        double d_imag = a_imag - b_real * sine   - b_imag * cosine;

        output_real[output_offset + i * output_stride] = c_real;
        output_imag[output_offset + i * output_stride] = c_imag;
        output_real[output_offset + (i + half) * output_stride] = d_real;
        output_imag[output_offset + (i + half) * output_stride] = d_imag;
    }
}