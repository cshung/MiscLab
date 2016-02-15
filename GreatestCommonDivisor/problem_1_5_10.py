from polynomial_module import *

def problem_1_5_10():
    print "Problem 10 - Just checking my extended Euclidean algorithm works"
    f = polynomial.from_string("x^4 + x^2 + 1")
    g = polynomial.from_string("x^4 - x^2 - 2x - 1")
    (a, b, h) = polynomial.polynomial_extended_gcd(f, g)
    af = polynomial.polynomial_multiply(a, f)
    bg = polynomial.polynomial_multiply(b, g)
    s = polynomial.polynomial_add(af, bg)
    d = polynomial.polynomial_subtract(h, s)
    print d