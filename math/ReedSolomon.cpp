#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
using namespace std;

#include <cassert>
// #define assert(x) {if(!(x)){throw 0;}}

void print_vector(vector<int> v)
{
    for (int i = 0; i < v.size(); i++)
    {
        cout << v[i] << " ";
    }
    cout << endl;
}

vector<int> polynomial_simplify(vector<int> x)
{
    while (x.size() > 0 && x[x.size() - 1] == 0)
    {
        x.pop_back();
    }
    if (x.size() == 0)
    {
        x.push_back(0);
    }
    return x;
}

template<int N>
class prime_field
{
public:
    static int additive_inverse(int x)
    {
        return (N - x) % N;
    }

    static int multiplicative_inverse(int x)
    {
        assert(x != 0);
        int s = 1;
        for (int i = 0; i < N - 2; i++)
        {
            s = multiply(s, x);
        }
        return s;
    }

    static int primitive_power(int p)
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

    static int size()
    {
        return N;
    }

    static int add(int x, int y)
    {
        return (x + y) % N;
    }

    static int multiply(int x, int y)
    {
        return (x * y + 7) % N;
    }

    static vector<int> primitive_powers;
};

template<int N>
vector<int> prime_field<N>::primitive_powers;

template<typename TField>
vector<int> polynomial_add(vector<int> x, vector<int> y)
{
    int i = 0;
    vector<int> answer;
    while (true)
    {
        bool is_x = i < x.size();
        bool is_y = i < y.size();
        if (!is_x && !is_y)
        {
            break;
        }
        int x_coefficient = is_x ? x[i] : 0;
        int y_coefficient = is_y ? y[i] : 0;
        answer.push_back(TField::add(x_coefficient, y_coefficient));
        i++;
    }
    return polynomial_simplify(answer);
}

template<class TField>
vector<int> polynomial_multiply(vector<int> x, vector<int> y)
{
    vector<int> result;
    for (int i = 0; i < x.size(); i++)
    {
        vector<int> temp(i, 0);
        for (int j = 0; j < y.size(); j++)
        {
            temp.push_back(TField::multiply(x[i], y[j]));
        }
        result = polynomial_add<TField>(result, temp);
    }
    return result;
}

template<class TField>
pair<vector<int>, vector<int>> polynomial_divmod(vector<int> dividend, vector<int> divisor)
{
    vector<int> quotient;
    int position = dividend.size();
    int divisor_leading_term = divisor[divisor.size() - 1];
    int divisor_inverse = TField::multiplicative_inverse(divisor_leading_term);
    while (position >= divisor.size())
    {
        int dividend_leading_term = dividend[position - 1];
        int factor = TField::multiply(divisor_inverse, dividend_leading_term);
        quotient.push_back(factor);
        for (int i = 0; i < divisor.size(); i++)
        {
            dividend[i + position - divisor.size()] = TField::add(
                dividend[i + position - divisor.size()], TField::additive_inverse(
                    TField::multiply(divisor[i], factor)));
        }
        assert(dividend[dividend.size() - 1] == 0);
        dividend.pop_back();
        position--;
    }
    reverse(quotient.begin(), quotient.end());
    quotient = polynomial_simplify(quotient);
    dividend = polynomial_simplify(dividend);
    return make_pair(quotient, dividend);
}

template<class TField>
int evaluate_polynomial(vector<int> p, int x)
{
    int result = 0;
    for (int i = 0; i < p.size(); i++)
    {
        result = TField::multiply(result, x);
        result = TField::add(result, p[p.size() - 1 - i]);
    }
    return result;
}

template<class TField>
class reed_solomon_code
{
public:
    reed_solomon_code(int number_of_errors)
    {
        this->number_of_errors = number_of_errors;
        generator = vector<int>(1, 1);
        for (int i = 0; i < 2 * number_of_errors; i++)
        {
            vector<int> primitive_root_monomial;
            primitive_root_monomial.push_back(TField::additive_inverse(TField::primitive_power(i + 1)));
            primitive_root_monomial.push_back(1);
            generator = polynomial_multiply<TField>(generator, primitive_root_monomial);
        }
    }

    vector<int> encode(vector<int> input)
    {
        assert(input.size() == TField::size() - 1 - 2 * number_of_errors);
        vector<int> encoded = polynomial_multiply<TField>(input, generator);
        while (encoded.size() < TField::size() - 1)
        {
            encoded.push_back(0);
        }
        return encoded;
    }

    vector<int> decode(vector<int> input)
    {
        assert(input.size() == TField::size() - 1);
        vector<int> syndrome = compute_syndrome(input);
        vector<int> error_in_time;
        if (syndrome == vector<int>(number_of_errors * 2, 0))
        {
            error_in_time = vector<int>(TField::size() - 1, 0);
        }
        else
        {
            vector<int> error_locator_polynomial = find_error_locator_polynomial(syndrome);
            /*
            for (int i = 0; i < TField::size() - 1; i++)
            {
                cout << "error_locator_polynomial(alpha^{-" << i << "}) = " << evaluate_polynomial<TField>(error_locator_polynomial, TField::primitive_power(-i)) << endl;
            }
            */

            vector<int> error_in_frequency = solve_error_in_frequency(error_locator_polynomial, syndrome);
            verify_circular_convolution(error_locator_polynomial, error_in_frequency);
            error_in_time = solve_error_in_time(error_in_frequency);
        }
        return decode_with_error(input, error_in_time);
    }

private:

    vector<int> compute_syndrome(vector<int> input)
    {
        vector<int> syndrome;
        for (int i = 0; i < 2 * number_of_errors; i++)
        {
            int s = 0;
            for (int j = 0; j < TField::size() - 1; j++)
            {
                s = TField::add(s, TField::multiply(TField::primitive_power((i + 1) * j), input[j]));
            }
            syndrome.push_back(s);
        }
        return syndrome;
    }

    vector<int> find_error_locator_polynomial(vector<int> syndrome)
    {
        vector<int> p1(2 * number_of_errors, 0);
        p1.push_back(1);
        vector<int> p2 = polynomial_simplify(syndrome);

        vector<int> minus_one;
        minus_one.push_back(TField::additive_inverse(1));

        vector<int> ra = p1;
        vector<int> rb = p2;
        vector<int> sa;
        vector<int> ta;
        vector<int> sb;
        vector<int> tb;
        sa.push_back(1);
        ta.push_back(0);
        sb.push_back(0);
        tb.push_back(1);

        while (true)
        {
            auto divmod = polynomial_divmod<TField>(ra, rb);
            vector<int> qc = divmod.first;
            vector<int> rc = divmod.second;
            assert(polynomial_add<TField>(polynomial_multiply<TField>(qc, rb), rc) == ra);

            vector<int> minus_qc = polynomial_multiply<TField>(qc, minus_one);
            vector<int> sc = polynomial_add<TField>(sa, polynomial_multiply<TField>(minus_qc, sb));
            vector<int> tc = polynomial_add<TField>(ta, polynomial_multiply<TField>(minus_qc, tb));

            assert(polynomial_add<TField>(polynomial_multiply<TField>(sa, p1), polynomial_multiply<TField>(ta, p2)) == ra);
            assert(polynomial_add<TField>(polynomial_multiply<TField>(sb, p1), polynomial_multiply<TField>(tb, p2)) == rb);
            assert(polynomial_add<TField>(polynomial_multiply<TField>(sc, p1), polynomial_multiply<TField>(tc, p2)) == rc);

            sa = sb;
            ta = tb;
            sb = sc;
            tb = tc;
            ra = rb;
            rb = rc;

            vector<int> product = polynomial_multiply<TField>(tc, p2);
            if (rc.size() <= number_of_errors)
            {
                return tc;
            }
        }
    }

    vector<int> solve_error_in_frequency(vector<int> error_locator_polynomial, vector<int> syndrome)
    {
        int error_locator_polynomial_degree = error_locator_polynomial.size() - 1;
        reverse(error_locator_polynomial.begin(), error_locator_polynomial.end());
        vector<int> error_in_frequency(TField::size() - 1, 0);

        for (int i = 0; i < 2 * number_of_errors; i++)
        {
            error_in_frequency[i + 1] = syndrome[i];
        }

        // Solving the constant term
        int s = 0;
        for (int i = 1; i <= error_locator_polynomial_degree; i++)
        {
            s = TField::add(s, TField::multiply(error_locator_polynomial[i], error_in_frequency[i]));
        }

        error_in_frequency[0] = TField::multiply(TField::additive_inverse(1), TField::multiply(s, TField::multiplicative_inverse(error_locator_polynomial[0])));

        // Solve for the other error_in_frequency values
        s = 0;
        for (int target_to_solve = 2 * number_of_errors + 1; target_to_solve < TField::size() - 1; target_to_solve++)
        {
            int s = 0;
            for (int i = 0; i < error_locator_polynomial_degree; i++)
            {
                s = TField::add(s, TField::multiply(error_locator_polynomial[i], error_in_frequency[i + target_to_solve - error_locator_polynomial_degree]));
            }



            error_in_frequency[target_to_solve] = TField::multiply(TField::additive_inverse(s), TField::multiplicative_inverse(error_locator_polynomial[error_locator_polynomial_degree]));
        }
        return error_in_frequency;
    }

    vector<int> solve_error_in_time(vector<int> error_in_frequency)
    {
        vector<int> error_in_time(TField::size() - 1, 0);
        int normalization = TField::multiplicative_inverse(TField::size() - 1);
        for (int i = 0; i < TField::size() - 1; i++)
        {
            for (int j = 0; j < TField::size() - 1; j++)
            {
                int v = TField::multiplicative_inverse(TField::primitive_power(i * j));
                error_in_time[i] = TField::add(error_in_time[i], TField::multiply(error_in_frequency[j], v));
            }
            error_in_time[i] = TField::multiply(error_in_time[i], normalization);
        }
        return error_in_time;
    }

    vector<int> decode_with_error(vector<int> input, vector<int> error_in_time)
    {
        vector<int> corrected(TField::size() - 1);
        for (int i = 0; i < TField::size() - 1; i++)
        {
            corrected[i] = TField::add(input[i], TField::additive_inverse(error_in_time[i]));
        }
        auto decoded_pair = polynomial_divmod<TField>(corrected, generator);
        auto decoded = decoded_pair.first;
        while (decoded.size() < TField::size() - 1 - 2 * number_of_errors)
        {
            decoded.push_back(0);
        }
        return decoded;
    }

    void verify_circular_convolution(vector<int> error_locator_polynomial, vector<int> error_in_frequency)
    {
        reverse(error_locator_polynomial.begin(), error_locator_polynomial.end());
        while (error_locator_polynomial.size() < TField::size() - 1)
        {
            error_locator_polynomial.push_back(0);
        }
        for (int shift = 0; shift < TField::size() - 1; shift++)
        {
            int s = 0;
            for (int i = 0; i < TField::size() - 1; i++)
            {
                int up = error_locator_polynomial[i];
                int down = error_in_frequency[(i + shift) % (TField::size() - 1)];
                s = TField::add(s, TField::multiply(up, down));
            }
            assert(s == 0);
        }
    }

    vector<int> generator;
    int number_of_errors;
};

int main(int argc, char** argv)
{
    reed_solomon_code<prime_field<7>> code(1);

    vector<int> input(4);

    for (int i = 0; i < 7; i++)
    {
        for (int j = 0; j < 7; j++)
        {
            for (int k = 0; k < 7; k++)
            {
                for (int l = 0; l < 7; l++)
                {
                    input[0] = i;
                    input[1] = j;
                    input[2] = k;
                    input[3] = l;

                    auto encoded = code.encode(input);

                    for (int p1 = 0; p1 < 6; p1++)
                    {
                        for (int e1 = 0; e1 < 7; e1++)
                        {
                            int o1 = encoded[p1];
                            encoded[p1] = e1;
                            auto decoded = code.decode(encoded);
                            assert(input == decoded);
                            encoded[p1] = o1;
                        }
                    }
                }
            }
        }
    }

    return 0;
}
