from polynomial_module import *

def problem_1_5_15():
    print "Problem 15"
    f = polynomial.from_string("x^11 - x^10 + 2x^8 - 4x^7 + 3x^5 - 3x^4 + x^3 + 3x^2 - x - 1")
    fd = f.derivative()
    g = polynomial.polynomial_gcd(f, fd)
    (q, r) = polynomial.polynomial_divide(f, g)
    print q
