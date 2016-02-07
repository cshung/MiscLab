from polynomial_module import *

print "Problem 8a"
print polynomial.polynomial_gcd(
    polynomial.polynomial_gcd(
        polynomial.from_string("x^4 + x^2 + 1"),
        polynomial.from_string("x^4 - x^2 - 2x - 1")),
    polynomial.from_string("x^3 - 1"))

print "Problem 8b"
print polynomial.polynomial_gcd(
    polynomial.polynomial_gcd(
        polynomial.from_string("x^3 + 2x^2 - x - 2"),
        polynomial.from_string("x^3 - 2x^2 - x + 2")),
    polynomial.from_string("x^3 - x^2 - 4x + 4"))

print "Problem 9"
gcd = polynomial.polynomial_gcd(
    polynomial.polynomial_gcd(
        polynomial.from_string("x^3 + x^2 - 4x - 4"),
        polynomial.from_string("x^3 - x^2 - 4x + 4")),
    polynomial.from_string("x^3 - 2x^2 - x + 2"))
print polynomial.polynomial_divide(polynomial.from_string("x^2 - 4"), gcd)[0]

print "Problem 10 - Just checking my extended Euclidean algorithm works"
f = polynomial.from_string("x^4 + x^2 + 1")
g = polynomial.from_string("x^4 - x^2 - 2x - 1")
(a, b, h) = polynomial.polynomial_extended_gcd(f, g)
af = polynomial.polynomial_multiply(a, f)
bg = polynomial.polynomial_multiply(b, g)
s = polynomial.polynomial_add(af, bg)
d = polynomial.polynomial_subtract(h, s)
print d

# TODO, a bug in printing polynomial
print "Bug"
print polynomial.from_string("0 - 1x") # should print -x, but currently print x + 0