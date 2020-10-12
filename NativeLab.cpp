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
// TODO, prime should account for largest possible overlap, good for now
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


void fast_number_theoretic_transform_helper(int length, int input_offset, int stride, vector<int>& input, int output_offset, int output_stride, vector<int>& output, int sign)
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

void fast_number_theoretic_transform(int length, int input_offset, int stride, vector<int>& input, int output_offset, int output_stride, vector<int>& output)
{
    fast_number_theoretic_transform_helper(length, input_offset, stride, input, output_offset, output_stride, output, 1);
}

void inverse_fast_number_theoretic_transform(int length, int input_offset, int stride, vector<int>& input, int output_offset, int output_stride, vector<int>& output)
{
    fast_number_theoretic_transform_helper(length, input_offset, stride, input, output_offset, output_stride, output, -1);
    for (int f = 0; f < length; f++)
    {
        // TODO - What I really needed here is the multiplicative inverse of length, not the multiplicative inverse of 64
        output[output_offset + f * output_stride] = mod(output[output_offset + f * output_stride] * inverse, prime);
    }
}

void init_powers()
{
    int p = 1;
    for (int i = 0; i < n; i++)
    {
        powers[i] = p;
        p = mod(p * a, prime);
    }
}

/*
clc;clear;
it1 = [1 1 0;0 1 0;0 1 0];
it2 = [1 0 0;1 1 0;0 0 0];
IT1 = [it1 zeros(3, 3);zeros(3, 6)];
IT2 = [it2 zeros(3, 3);zeros(3, 6)];
IF1 = fft2(IT1);
IF2 = fft2(IT2);
CF = IF1 .* IF2;
CT = ifft2(CF)
conv2(it1,it2)
 */
int main(int argc, char** argv)
{
    init_powers();
    vector<vector<int>> img1(3, vector<int>(3, 0));
    vector<vector<int>> img2(3, vector<int>(3, 0));
    img1[0][0] = 1; img1[0][1] = 1; img1[0][2] = 0;
    img1[1][0] = 0; img1[1][1] = 1; img1[1][2] = 0;
    img1[2][0] = 0; img1[2][1] = 1; img1[2][2] = 0;

    img2[0][0] = 0; img2[0][1] = 0; img2[0][2] = 0;
    img2[1][0] = 0; img2[1][1] = 1; img2[1][2] = 1;
    img2[2][0] = 0; img2[2][1] = 0; img2[2][2] = 1;

    vector<int> padded_image1(64, 0);
    vector<int> padded_image2(64, 0);

    vector<int> image1_rows(64, 0);
    vector<int> image2_rows(64, 0);
    vector<int> image1_rows_cols(64, 0);
    vector<int> image2_rows_cols(64, 0);
    vector<int> product (64, 0);
    vector<int> product_rows(64, 0);
    vector<int> product_rows_cols(64, 0);

    for (int row = 0; row < img1.size(); row++)
    {
        for (int col = 0; col < img1[0].size(); col++)
        {
            padded_image1[row * 8 + col] = img1[row][col];
        }
    }

    for (int row = 0; row < img2.size(); row++)
    {
        for (int col = 0; col < img2[0].size(); col++)
        {
            padded_image2[row * 8 + col] = img2[img2.size() - row - 1][img2[0].size() - col - 1];
        }
    }

    for (int row = 0; row < 8; row++)
    {
        fast_number_theoretic_transform(8, row * 8, 1, padded_image1, row * 8, 1, image1_rows);
        fast_number_theoretic_transform(8, row * 8, 1, padded_image2, row * 8, 1, image2_rows);
    }
    for (int col = 0; col < 8; col++)
    {
        fast_number_theoretic_transform(8, col, 8, image1_rows, col, 8, image1_rows_cols);
        fast_number_theoretic_transform(8, col, 8, image2_rows, col, 8, image2_rows_cols);
    }
    for (int i = 0; i < 64; i++)
    {
        product[i] = mod(image1_rows_cols[i] * image2_rows_cols[i], prime);
    }
    for (int row = 0; row < 8; row++)
    {
        inverse_fast_number_theoretic_transform(8, row * 8, 1, product, row * 8, 1, product_rows);
    }
    for (int col = 0; col < 8; col++)
    {
        inverse_fast_number_theoretic_transform(8, col, 8, product_rows, col, 8, product_rows_cols);
    }
    for (int row = 0; row < 8; row++)
    {
        for (int col = 0; col < 8; col++)
        {
            // TODO, eliminate this compensation
            cout << mod(product_rows_cols[row * 8 + col] * 64, prime) << " ";
        }
        cout << endl;
    }

    return 0;
}