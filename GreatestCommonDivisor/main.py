from polynomial_module import *

# Problem 8a
print polynomial.polynomial_gcd(
    polynomial.polynomial_gcd(
        polynomial.from_string("x^4 + x^2 + 1"),
        polynomial.from_string("x^4 - x^2 - 2x - 1")),
    polynomial.from_string("x^3 - 1"))

# Problem 8b
print polynomial.polynomial_gcd(
    polynomial.polynomial_gcd(
        polynomial.from_string("x^3 + 2x^2 - x - 2"),
        polynomial.from_string("x^3 - 2x^2 - x + 2")),
    polynomial.from_string("x^3 - x^2 - 4x + 4"))

