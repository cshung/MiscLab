#include <iostream>
#include <vector>
#include <map>
#include <cassert>
#include <algorithm>
using namespace std;

// To be producted, reed solomon code

void print_vector(vector<int> v)
{
    for (int i = 0; i < v.size(); i++)
    {
        cout << v[i] << " ";
    }
    cout << endl;
}

int field_add(int x, int y)
{
    return (x + y) % 7;
}

int field_multiply(int x, int y)
{
    return (x * y + 7) % 7;
}

int additive_inverse(int x)
{
    return (7 - x) % 7;
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
        int answer = field_multiply(half, half);
        if (n % 2 == 1)
        {
            answer = field_multiply(answer, a);
        }
        return answer;
    }
}

int multiplicative_inverse(int a)
{
    return power(a, 5);
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
        answer.push_back(field_add(x_coefficient, y_coefficient));
        i++;
    }
    return polynomial_simplify(answer);
}

vector<int> polynomial_multiply(vector<int> x, vector<int> y)
{
    vector<int> result;
    for (int i = 0; i < x.size(); i++)
    {
        vector<int> temp(i, 0);
        for (int j = 0; j < y.size(); j++)
        {
            temp.push_back(field_multiply(x[i], y[j]));
        }
        result = polynomial_add(result, temp);
    }
    return result;
}

pair<vector<int>, vector<int>> polynomial_divmod(vector<int> dividend, vector<int> divisor)
{
    vector<int> quotient;
    int position = dividend.size();
    int divisor_leading_term = divisor[divisor.size() - 1];
    int divisor_inverse = multiplicative_inverse(divisor_leading_term);
    while (position >= divisor.size())
    {
        int dividend_leading_term = dividend[position - 1];
        int factor = field_multiply(divisor_inverse, dividend_leading_term);
        quotient.push_back(factor);
        for (int i = 0; i < divisor.size(); i++)
        {
            dividend[i + position - divisor.size()] = field_add(
                dividend[i + position - divisor.size()], additive_inverse(
                    field_multiply(divisor[i], factor)));
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

int evaluate_polynomial(vector<int> p, int x)
{
    int result = 0;
    for (int i = 0; i < p.size(); i++)
    {
        result = field_multiply(result, x);
        result = field_add(result, p[p.size() - 1 - i]);
    }
    return result;
}

vector<int> euclidean(vector<int> p1, vector<int> p2)
{
    vector<int> minus_one;
    minus_one.push_back(additive_inverse(1));

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
        auto divmod = polynomial_divmod(ra, rb);
        vector<int> qc = divmod.first;
        vector<int> rc = divmod.second;
        assert(polynomial_add(polynomial_multiply(qc, rb), rc) == ra);

        // ra = qc * rb + rc
        // 
        // sa p1 + ta p2 = ra
        // sb p1 + tb p2 = rb
        // 
        // (sa p1 + ta p2) = qc * (sb p1 + tb p2) + rc
        // (sa - qc sb) p1 + (ta - qc tb) p2 = rc    
        // sc p1 + tc p2 = rc

        vector<int> minus_qc = polynomial_multiply(qc, minus_one);
        vector<int> sc = polynomial_add(sa, polynomial_multiply(minus_qc, sb));
        vector<int> tc = polynomial_add(ta, polynomial_multiply(minus_qc, tb));

        assert(polynomial_add(polynomial_multiply(sa, p1), polynomial_multiply(ta, p2)) == ra);
        assert(polynomial_add(polynomial_multiply(sb, p1), polynomial_multiply(tb, p2)) == rb);
        assert(polynomial_add(polynomial_multiply(sc, p1), polynomial_multiply(tc, p2)) == rc);

        sa = sb;
        ta = tb;
        sb = sc;
        tb = tc;
        ra = rb;
        rb = rc;

        if (rc.size() == 1)
        {
            return tc;
        }
    }
}

int main(int argc, char** argv)
{
    int t = 2;
    int q = 7;
    int primitive_root = 3;

    int powers = primitive_root;

    // The encoder side of thing
    vector<int> p1; p1.push_back(additive_inverse(powers)); p1.push_back(1);
    powers = field_multiply(powers, 3);
    vector<int> p2; p2.push_back(additive_inverse(powers)); p2.push_back(1);
    powers = field_multiply(powers, 3);
    vector<int> p3; p3.push_back(additive_inverse(powers)); p3.push_back(1);
    powers = field_multiply(powers, 3);
    vector<int> p4; p4.push_back(additive_inverse(powers)); p4.push_back(1);

    // That gives us the polynomial of example 4.130.
    vector<int> polynomial = polynomial_multiply(polynomial_multiply(p1, p2), polynomial_multiply(p3, p4));

    cout << "g" << endl;
    for (int i = 0; i < polynomial.size(); i++)
    {
        cout << polynomial[i] << " ";
    }
    cout << endl;

    // The decoder side of thing
    int received_array[] = { 4, 0, 5, 1, 0, 1 };
    vector<int> received(received_array, received_array + _countof(received_array));
    vector<int> syndrome;
    for (int i = 0; i < 2 * t; i++)
    {
        int s = 0;
        for (int j = 0; j < q - 1; j++)
        {
            s = field_add(s, field_multiply(power(primitive_root, (i + 1) * j), received_array[j]));
        }
        syndrome.push_back(s);
    }

    cout << "syndrome" << endl;
    for (int i = 0; i < 2 * t; i++)
    {
        cout << syndrome[i] << " ";
    }
    cout << endl;

    vector<int> z(2 * t, 0);
    z[2 * t - 1] = 1;
    vector<int> error_locator_polynomial = euclidean(z, syndrome);

    cout << "error_locator_polynomial" << endl;
    print_vector(error_locator_polynomial);

    for (int i = 0; i < q; i++)
    {
        cout << "error_locator_polynomial[" << i << "] = " << evaluate_polynomial(error_locator_polynomial, i) << endl;
    }
    for (int i = 0; i < q; i++)
    {
        cout << "primitive_root^" << i << "=" << power(primitive_root, i) << endl;
    }

    reverse(error_locator_polynomial.begin(), error_locator_polynomial.end());
    vector<int> error_in_frequency(q - 1, 0);

    // Solving the constant term
    int s = 0;
    for (int i = 1; i <= t; i++)
    {
        s = field_add(s, field_multiply(error_locator_polynomial[i], syndrome[i - 1]));
    }

    // 4x + s = 0, therefore x = (-s)/4
    error_in_frequency[0] = field_multiply(additive_inverse(1), field_multiply(s, multiplicative_inverse(syndrome[0])));

    // Copy the syndrome (TODO - we can use the same storage to avoid copy)
    for (int i = 0; i < 2 * t; i++)
    {
        error_in_frequency[i + 1] = syndrome[i];
    }

    // Solve for the other error_in_frequency values
    s = 0;
    for (int target_to_solve = 2 * t + 1; target_to_solve < q - 1; target_to_solve++)
    {
        int s = 0;
        for (int i = 0; i < t; i++)
        {
            // The error_locator_polynomial[t] is aligned with error_in_frequency[target_to_solve]
            s = field_add(s, field_multiply(error_locator_polynomial[i], error_in_frequency[i + target_to_solve - t]));
        }
        error_in_frequency[target_to_solve] = additive_inverse(s);
    }
    cout << "error_in_frequency" << endl;
    print_vector(error_in_frequency);

    // The circular convolution should be 0 always
    for (int shift = 0; shift < q - 1; shift++)
    {
        s = 0;
        for (int i = 0; i < q - 1; i++)
        {
            int up = i <= t ? error_locator_polynomial[i] : 0;
            int down = error_in_frequency[(i + shift) % (q - 1)];
            s = field_add(s, field_multiply(up, down));
        }
        assert(s == 0);
    }

    // Compute the inverse fourier transform
    vector<int> error_in_time(q - 1, 0);
    for (int i = 0; i < q - 1; i++)
    {
        for (int j = 0; j < q - 1; j++)
        {
            int v = multiplicative_inverse(power(primitive_root, i * j));
            error_in_time[i] = field_add(error_in_time[i], field_multiply(error_in_frequency[j], v));
        }
        error_in_time[i] = field_multiply(error_in_time[i], multiplicative_inverse(q - 1));
    }
    cout << "error_in_time" << endl;
    print_vector(error_in_time);

    for (int i = 0; i < q - 1; i++)
    {
        received[i] = field_add(received[i], additive_inverse(error_in_time[i]));
    }
    cout << "corrected" << endl;
    print_vector(received);

    auto decoding = polynomial_divmod(received, polynomial);
    cout << "decoded" << endl;
    print_vector(decoding.first);

    auto encoded = polynomial_multiply(decoding.first, polynomial);
    cout << "encoded" << endl;
    print_vector(encoded);

    return 0;
}
