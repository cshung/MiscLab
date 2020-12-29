#include <cassert>

template<int N>
int prime_field<N>::additive_inverse(int x)
{
    return (N - x) % N;
}

template<int N>
int prime_field<N>::multiplicative_inverse(int x)
{
    assert(x != 0);
    int s = 1;
    for (int i = 0; i < N - 2; i++)
    {
        s = multiply(s, x);
    }
    return s;
}

template<int N>
int prime_field<N>::primitive_power(int p)
{
    if (primitive_powers.size() == 0)
    {
        primitive_powers.reserve(N);
        for (int trial_primitive_root = 2; trial_primitive_root < N; trial_primitive_root++)
        {
            bool is_primitive_root = true;
            primitive_powers.clear();
            primitive_powers.push_back(1);

            for (int power = 1; is_primitive_root && power < N - 1; power++)
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
    p = p % (N - 1);
    if (p < 0)
    {
        p = p + (N - 1);
    }
    return primitive_powers[p];
}

template<int N>
int prime_field<N>::size()
{
    return N;
}

template<int N>
int prime_field<N>::add(int x, int y)
{
    return (x + y) % N;
}

template<int N>
int prime_field<N>::multiply(int x, int y)
{
    return (x * y + N) % N;
}

template<int N>
std::vector<int> prime_field<N>::primitive_powers;
