#include <iostream>
#include <vector>
#include <map>
#include <algorithm>

#include <cassert>
// #define assert(x) {if(!(x)){throw 0;}}

void print_vector(std::vector<int> v)
{
    for (int i = 0; i < v.size(); i++)
    {
        std::cout << v[i] << " ";
    }
    std::cout << std::endl;
}

std::vector<int> polynomial_simplify(std::vector<int> x)
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

template<class TField>
std::vector<int> polynomial_add(std::vector<int> x, std::vector<int> y)
{
    int i = 0;
    std::vector<int> answer;
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
std::vector<int> polynomial_multiply(std::vector<int> x, std::vector<int> y)
{
    std::vector<int> result;
    for (int i = 0; i < x.size(); i++)
    {
        std::vector<int> temp(i, 0);
        for (int j = 0; j < y.size(); j++)
        {
            temp.push_back(TField::multiply(x[i], y[j]));
        }
        result = polynomial_add<TField>(result, temp);
    }
    return result;
}

template<class TField>
std::pair<std::vector<int>, std::vector<int>> polynomial_divmod(std::vector<int> dividend, std::vector<int> divisor)
{
    std::vector<int> quotient;
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
int evaluate_polynomial(std::vector<int> p, int x)
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
reed_solomon_code<TField>::reed_solomon_code(int number_of_errors)
{
    this->number_of_errors = number_of_errors;
    generator = std::vector<int>(1, 1);
    for (int i = 0; i < 2 * number_of_errors; i++)
    {
        std::vector<int> primitive_root_monomial;
        primitive_root_monomial.push_back(TField::additive_inverse(TField::primitive_power(i + 1)));
        primitive_root_monomial.push_back(1);
        generator = polynomial_multiply<TField>(generator, primitive_root_monomial);
    }
}

template<class TField>
std::vector<int> reed_solomon_code<TField>::encode(std::vector<int> input)
{
    assert(input.size() == TField::size() - 1 - 2 * number_of_errors);
    std::vector<int> encoded = polynomial_multiply<TField>(input, generator);
    while (encoded.size() < TField::size() - 1)
    {
        encoded.push_back(0);
    }
    return encoded;
}

template<class TField>
std::vector<int> reed_solomon_code<TField>::decode(std::vector<int> input)
{
    assert(input.size() == TField::size() - 1);
    std::vector<int> syndrome = compute_syndrome(input);
    std::vector<int> error_in_time;
    if (syndrome == std::vector<int>(number_of_errors * 2, 0))
    {
        error_in_time = std::vector<int>(TField::size() - 1, 0);
    }
    else
    {
        std::vector<int> error_locator_polynomial = find_error_locator_polynomial(syndrome);
        /*
        for (int i = 0; i < TField::size() - 1; i++)
        {
            int answer = evaluate_polynomial<TField>(error_locator_polynomial, TField::primitive_power(-i));
            if (answer == 0)
            {
                std::cout << "error_locator_polynomial(alpha^{-" << i << "}) = " << answer << std::endl;
            }
        }
        */
        std::vector<int> error_in_frequency = solve_error_in_frequency(error_locator_polynomial, syndrome);
        verify_circular_convolution(error_locator_polynomial, error_in_frequency);
        error_in_time = solve_error_in_time(error_in_frequency);
    }
    return decode_with_error(input, error_in_time);
}

template<class TField>
std::vector<int> reed_solomon_code<TField>::compute_syndrome(std::vector<int> input)
{
    std::vector<int> syndrome;
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

template<class TField>
std::vector<int> reed_solomon_code<TField>::find_error_locator_polynomial(std::vector<int> syndrome)
{
    std::vector<int> p1(2 * number_of_errors, 0);
    p1.push_back(1);
    std::vector<int> p2 = polynomial_simplify(syndrome);

    std::vector<int> minus_one;
    minus_one.push_back(TField::additive_inverse(1));

    std::vector<int> ra = p1;
    std::vector<int> rb = p2;
    std::vector<int> sa;
    std::vector<int> ta;
    std::vector<int> sb;
    std::vector<int> tb;
    sa.push_back(1);
    ta.push_back(0);
    sb.push_back(0);
    tb.push_back(1);

    while (true)
    {
        auto divmod = polynomial_divmod<TField>(ra, rb);
        std::vector<int> qc = divmod.first;
        std::vector<int> rc = divmod.second;
        assert(polynomial_add<TField>(polynomial_multiply<TField>(qc, rb), rc) == ra);

        std::vector<int> minus_qc = polynomial_multiply<TField>(qc, minus_one);
        std::vector<int> sc = polynomial_add<TField>(sa, polynomial_multiply<TField>(minus_qc, sb));
        std::vector<int> tc = polynomial_add<TField>(ta, polynomial_multiply<TField>(minus_qc, tb));

        assert(polynomial_add<TField>(polynomial_multiply<TField>(sa, p1), polynomial_multiply<TField>(ta, p2)) == ra);
        assert(polynomial_add<TField>(polynomial_multiply<TField>(sb, p1), polynomial_multiply<TField>(tb, p2)) == rb);
        assert(polynomial_add<TField>(polynomial_multiply<TField>(sc, p1), polynomial_multiply<TField>(tc, p2)) == rc);

        sa = sb;
        ta = tb;
        sb = sc;
        tb = tc;
        ra = rb;
        rb = rc;

        std::vector<int> product = polynomial_multiply<TField>(tc, p2);
        if (rc.size() <= number_of_errors)
        {
            return tc;
        }
    }
}

template<class TField>
std::vector<int> reed_solomon_code<TField>::solve_error_in_frequency(std::vector<int> error_locator_polynomial, std::vector<int> syndrome)
{
    int error_locator_polynomial_degree = error_locator_polynomial.size() - 1;
    reverse(error_locator_polynomial.begin(), error_locator_polynomial.end());
    std::vector<int> error_in_frequency(TField::size() - 1, 0);

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

template<class TField>
std::vector<int> reed_solomon_code<TField>::solve_error_in_time(std::vector<int> error_in_frequency)
{
    std::vector<int> error_in_time(TField::size() - 1, 0);

    int n = 0;
    for (int i = 0; i < TField::size() - 1; i++)
    {
        n = TField::add(n, 1);
    }
    int normalization = TField::multiplicative_inverse(n);

    for (int i = 0; i < TField::size() - 1; i++)
    {
        for (int j = 0; j < TField::size() - 1; j++)
        {
            int v = TField::primitive_power(-i * j);
            error_in_time[i] = TField::add(error_in_time[i], TField::multiply(error_in_frequency[j], v));
        }
        error_in_time[i] = TField::multiply(error_in_time[i], normalization);
    }
    return error_in_time;
}

template<class TField>
std::vector<int> reed_solomon_code<TField>::decode_with_error(std::vector<int> input, std::vector<int> error_in_time)
{
    std::vector<int> corrected(TField::size() - 1);
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

template<class TField>
void reed_solomon_code<TField>::verify_circular_convolution(std::vector<int> error_locator_polynomial, std::vector<int> error_in_frequency)
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
