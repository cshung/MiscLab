#include <iostream>
#include <unordered_map>
#include <cassert>
using namespace std;

// This code represents GF(2^4), a Galois field of 16 elements
// Each element in the field is represented by a polynomial with cofficients in GF(2) [i.e. a single bit]
// We use an integer to represent the polynomial. Therefore, 00001011 represents the polynomial x^3 + x + 1
// Operations are done modulo a irreducible_polynomial polynomial, in this case, 00010011
// Beyond the constants, the code can be adapted to other irreducible polynomials and thus fields of different size
// 
// This page contains a list of irreducible polynomials to use:
// https://www.csee.umbc.edu/~lomonaco/f97/442/Peterson_Table.html
// 
// const int degree = 3;
// const int irreducible_polynomial = 11;
// 
// const int degree = 10;
// const int irreducible_polynomial = 1877;
//
// const int degree = 4;
// const int irreducible_polynomial = 19;

const int degree = 10;
const int irreducible_polynomial = 1877;
const int N = 1 << degree;

int add(int i, int j)
{
    // Adding a pair of polynomial is simply bitwise xor
    return i ^ j;
}

int additive_inverse(int i)
{
    return i;
}

int mul(int i, int j)
{
    // The idea of this algorithm is that when we write i to be f(x) and j to be g(x)
    // If we write g(x) as a sum of monomials, then we can write it as
    // g(x) = c2 x^2 + c1 x^1 + c0 x^0
    // 
    // Then the product can be written as
    // f(x)g(x) = c2 f(x) x^2 + c_1 f(x) x^1 + c_0 f(x) x^0
    //          = ((c2 f(x) x + c1 f(x))x) + c0 f(x)
    //          = ((((0)x + c2 f(x)) x + c1 f(x))x) + c0 f(x)
    // 
    // The formula does look complicated, but the code isn't.
    //
    // The evaluation starts from the innermost bracket, every time we want to evaluate the 
    // outer bracket, we multiply by x and then add f(x) if the coefficient is not zero.
    //
    // That's it
    //
    int result = 0;
    int mask = 1 << (degree - 1);
    for (int bit = 0; bit < degree; bit++)
    {
        // This operation multiply the current polynomial by x
        result = result << 1;
        // Assuming result was a polynomial with degree at most 2
        // After the multiply, it is at most 3, so either it is or it is not
        if ((result & (1 << degree)) != 0)
        {
            // In case it is, we compute mod irreducible_polynomial simply by subtracting it.
            result = add(result, additive_inverse(irreducible_polynomial));
        }
        // If the coefficient is not 0
        if ((mask & j) != 0)
        {
            // Add f(x) to the result
            result = add(result, i);
        }
        // And consider the next less significant term
        mask = mask >> 1;
    }
    return result;
}

// A simple repeated squaring algorithm
int power(int a, int n)
{
    if (n == 0)
    {
        return 1;
    }
    else if (n == 1)
    {
        return a;
    }
    else
    {
        int half = power(a, n / 2);
        int answer = mul(half, half);
        if (n % 2 == 1)
        {
            answer = mul(answer, a);
        }
        return answer;
    }
}

// We know that the is multiplicative group is cyclic with order N - 1, 
// therefore a^(N-1) = 1, so a^(N-2) is the multiplicative inverse
int multiplicative_inverse(int a)
{
    return power(a, N - 2);
}

bool is_primitive_root(int i)
{
    int t = 1;
    bool succeed = true;
    for (int j = 0; succeed && j < (N - 2); j++)
    {
        t = mul(t, i);
        if (t == 1)
        {
            succeed = false;
        }
    }
    return succeed;
}

void test_finite_field()
{
    // closure not tested
    for (int a = 0; a < N; a++)
    {
        // identity rule
        assert(add(a, 0) == a);
        assert(add(0, a) == a);
        assert(mul(a, 1) == a);
        assert(mul(1, a) == a);
        // additive inverse
        assert(add(a, additive_inverse(a)) == 0);
        if (a != 0)
        {
            assert(mul(a, multiplicative_inverse(a)) == 1);
        }
        for (int b = 0; b < N; b++)
        {
            // commutative
            assert(add(a, b) == add(b, a));
            assert(mul(a, b) == mul(b, a));
            for (int c = 0; c < N; c++)
            {
                // associative
                assert(add(add(a, b), c) == add(a, add(b, c)));
                assert(mul(mul(a, b), c) == mul(a, mul(b, c)));

                // distributive
                assert(mul(add(a, b), c) == add(mul(a, c), mul(b, c)));
            }
        }
    }
}

void find_primitive_roots()
{
    // The multiplicative group has 15 elements, therefore there should be 
    // phi(15) = phi(3) x phi(5) = 2 x 4 = 8 primitive roots.
    for (int i = 2; i < N; i++)
    {
        if (is_primitive_root(i))
        {
            cout << "A primitive root is " << i << endl;
        }
    }
}

// The procedure test that the field we generated does satisfy the field axioms.
int main()
{
    // test_finite_field();
    // find_primitive_roots();

    int q = power(2, 327);
    int m = 35;

    unordered_map<int, int> table;
    int p = 1;
    table[0] = p;
    for (int i = 1; i < m; i++)
    {
        p = mul(p, 2);
        table[p] = i;
    }

    for (int j = 0; j < m; j++)
    {
        int match = mul(q, multiplicative_inverse(power(2, j * m)));
        auto probe = table.find(match);
        if (probe != table.end())
        {
            cout << j * m + probe->second << endl;
            break;
        }
    }

    return 0;
}