from polynomial_module import *

# check if polynomial poly is in the ideal defined by the collection of polynomials in ideal
def is_in_ideal(poly, ideal):
    gcd = ideal[0]
    i = 1
    while (i < len(ideal)):
        gcd = polynomial.polynomial_gcd(gcd, ideal[i])
        i = i + 1
    (quotient, remainder) = polynomial.polynomial_divide(poly, gcd)
    return remainder.isZero()
    
def problem_2_1_1():
    print is_in_ideal(polynomial.from_string("x^2 - 3x + 2"), [polynomial.from_string("x - 2")])
    print is_in_ideal(polynomial.from_string("x^5 - 4x + 1"), [polynomial.from_string("x^3 - x^2 + x")])
    print is_in_ideal(polynomial.from_string("x^2 - 4x + 4"), [polynomial.from_string("x^4 - 4x^2 + 12x - 8"), polynomial.from_string("2x^3 - 10x^2 + 16x - 8")])
    print is_in_ideal(polynomial.from_string("x^3 - 1"), [polynomial.from_string("x^9 - 1"), polynomial.from_string("x^5 + x^3 - x^2 - 1")])
