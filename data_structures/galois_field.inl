#include <cassert>

template<int degree, int irreducible_polynomial>
int galois_field<degree, irreducible_polynomial>::additive_inverse(int x)
{
    return x;
}

template<int degree, int irreducible_polynomial>
int galois_field<degree, irreducible_polynomial>::multiplicative_inverse(int x)
{
    assert(x != 0);
    int s = 1;
    for (int i = 0; i < size() - 2; i++)
    {
        s = multiply(s, x);
    }
    return s;
}

template<int degree, int irreducible_polynomial>
int galois_field<degree, irreducible_polynomial>::primitive_power(int p)
{
    if (primitive_powers.size() == 0)
    {
        primitive_powers.reserve(size());
        for (int trial_primitive_root = 2; trial_primitive_root < size(); trial_primitive_root++)
        {
            bool is_primitive_root = true;
            primitive_powers.clear();
            primitive_powers.push_back(1);

            for (int power = 1; is_primitive_root && power < size() - 1; power++)
            {
                int next = multiply(primitive_powers[power - 1], trial_primitive_root);
                if (next == 1)
                {
                    is_primitive_root = false;
                }
                else
                {
                    primitive_powers.push_back(next);
                }
            }
            if (is_primitive_root)
            {
                break;
            }
        }
    }
    p = p % (size() - 1);
    if (p < 0)
    {
        p = p + (size() - 1);
    }
    return primitive_powers[p];
}

template<int degree, int irreducible_polynomial>
int galois_field<degree, irreducible_polynomial>::size()
{
    return 1 << degree;
}

template<int degree, int irreducible_polynomial>
int galois_field<degree, irreducible_polynomial>::add(int x, int y)
{
    return x ^ y;
}

template<int degree, int irreducible_polynomial>
int galois_field<degree, irreducible_polynomial>::multiply(int x, int y)
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
        if ((mask & y) != 0)
        {
            // Add f(x) to the result
            result = add(result, x);
        }
        // And consider the next less significant term
        mask = mask >> 1;
    }
    return result;
}

template<int degree, int irreducible_polynomial>
std::vector<int> galois_field<degree, irreducible_polynomial>::primitive_powers;
