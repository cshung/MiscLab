from polynomial_module import *

p1 = polynomial.from_string("1x^2 + 7x + 6")
p2 = polynomial.from_string("1x^2 - 5x - 6")

p3 = polynomial.polynomial_gcd(p1, p2)

print p3
print polynomial.polynomial_divide(p1, p3)[1]
print polynomial.polynomial_divide(p2, p3)[1]