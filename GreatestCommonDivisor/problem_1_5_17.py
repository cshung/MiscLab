from polynomial_module import *

def problem_1_5_17():
    print "Problem 17"
    f = polynomial.from_string("x^5 - 2x^4 + 2x^2 - x")
    g = polynomial.from_string("x^5 - x^4 - 2x^3 + 2x^2 + x - 1")
    gcd = polynomial.polynomial_gcd(f, g)
    gcdd = gcd.derivative()
    d = polynomial.polynomial_gcd(gcd, gcdd)
    (gred, r) = polynomial.polynomial_divide(gcd, d)
    print gcd
    print gcdd
    print d
    print gred