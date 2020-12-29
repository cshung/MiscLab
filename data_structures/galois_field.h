#pragma once

#include <vector>

// TODO: do not hard code n
template<int degree, int irreducible_polynomial>
class galois_field
{
public:
    static int additive_inverse(int x);

    static int multiplicative_inverse(int x);

    static int primitive_power(int p);

    static int size();

    static int add(int x, int y);

    static int multiply(int x, int y);

    static std::vector<int> primitive_powers;
};

#include "galois_field.inl"
