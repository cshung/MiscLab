#include <bitset>
#include <iostream>
#include <cassert>
#include <vector>
#include <stack>
#include <map>
#include <queue>
#include <algorithm>
#include <unordered_map>
#include <algorithm>
using namespace std;

/*

GF(2n), to be productized

void print(int i)
{
    // bitset<8> x(i);
    // cout << x << " ";
    cout << i << " ";
}

int shift(int i)
{
    i = i << 1;
    if ((i & (1 << 3)) != 0)
    {
        // 76543210
        // 00001011
        i = i ^ 0xb;
    }
    return i;
}

int add(int i, int j)
{
    return i ^ j;
}

int multiply(int i, int j)
{
    int result = 0;
    int mask = 1;
    for (int bit = 0; bit < 3; bit++)
    {
        if ((mask & j) != 0)
        {
            int temp = i;
            for (int times = 0; times < bit; times++)
            {
                temp = shift(temp);
            }
            result = add(result, temp);
        }
        mask = mask << 1;
    }
    return result;
}

int main()
{
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            cout << multiply(i, j) << " ";
        }
        cout << endl;
    }
    return 0;
}
*/

const int n = 64;
const int a = 125;
const int prime = 193;
const int inverse = 190;
int powers[64];

int mod(int x, int p)
{
    return ((x % p) + p) % p;
}

void number_theoretic_transform_helper(int n, int* input, int* output, int sign)
{
    for (int f = 0; f < n; f++)
    {
        output[f] = 0;
        for (int t = 0; t < n; t++)
        {
            output[f] = mod(output[f] + mod(powers[mod(sign * t * f, n)] * input[t], prime), prime);
        }
    }
}

void number_theoretic_transform(int n, int* input, int* output)
{
    number_theoretic_transform_helper(n, input, output, -1);
}

void inverse_number_theoretic_transform(int n, int* input, int* output)
{
    number_theoretic_transform_helper(n, input, output, 1);
    for (int f = 0; f < n; f++)
    {
        output[f] = mod(output[f] * inverse, prime);
    }
}

void fast_number_theoretic_transform_helper(int length, int input_offset, int stride, int* input, int output_offset, int output_stride, int* output, int sign)
{
    if (length == 1)
    {
        output[output_offset] = input[input_offset];
    }
    else
    {
        int half = length / 2;
        fast_number_theoretic_transform_helper(half, input_offset, stride * 2, input, output_offset, output_stride, output, sign);
        fast_number_theoretic_transform_helper(half, input_offset + stride, stride * 2, input, output_offset + half * output_stride, output_stride, output, sign);
        for (int i = 0; i < half; i++)
        {
            int a = output[output_offset + i * output_stride];
            int b = output[output_offset + (i + half) * output_stride];
            int c = mod(a + mod(powers[mod(-i * n / length * sign, n)] * b, prime), prime);
            int d = mod(a + mod(powers[mod((half - i) * n / length * sign, n)] * b, prime), prime);
            output[output_offset + i * output_stride] = c;
            output[output_offset + (i + half) * output_stride] = d;
        }
    }
}

void fast_number_theoretic_transform(int length, int input_offset, int stride, int* input, int output_offset, int output_stride, int* output)
{
    fast_number_theoretic_transform_helper(length, input_offset, stride, input, output_offset, output_stride, output, 1);
}

void inverse_fast_number_theoretic_transform(int length, int input_offset, int stride, int* input, int output_offset, int output_stride, int* output)
{
    fast_number_theoretic_transform_helper(length, input_offset, stride, input, output_offset, output_stride, output, -1);
    for (int f = 0; f < n; f++)
    {
        output[output_offset + f * output_stride] = mod(output[output_offset + f * output_stride] * inverse, prime);
    }
}

int main(int argc, char** argv)
{
    int p = 1;
    for (int i = 0; i < n; i++)
    {
        powers[i] = p;
        p = mod(p * a, prime);
    }
    int input[128];
    int output[128];
    for (int i = 0; i < 32; i++)
    {
        input[i * 2] = 1;
        input[i * 2 + 64] = 0;
    }
    fast_number_theoretic_transform(64, 0, 2, input, 0, 2, output);
    for (int i = 0; i < 64; i++)
    {
        // output[i] = mod(output[i] * output[i], prime);
        cout << output[i * 2] << " ";
    }
    cout << endl;
    inverse_fast_number_theoretic_transform(64, 0, 2, output, 0, 2, input);
    for (int i = 0; i < 64; i++)
    {
        cout << input[i * 2] << " ";
    }
    return 0;
}