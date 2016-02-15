from polynomial_module import *

def problem_1_5_9():
    print "Problem 9"
    gcd = polynomial.polynomial_gcd(
        polynomial.polynomial_gcd(
            polynomial.from_string("x^3 + x^2 - 4x - 4"),
            polynomial.from_string("x^3 - x^2 - 4x + 4")),
        polynomial.from_string("x^3 - 2x^2 - x + 2"))
    print polynomial.polynomial_divide(polynomial.from_string("x^2 - 4"), gcd)[0]
