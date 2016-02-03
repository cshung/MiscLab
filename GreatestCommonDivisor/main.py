from polynomial_module import *

p1 = polynomial.from_string("(1/2) - (1/3)x")
p2 = polynomial.from_string("(1/2) - (1/3)x")
p3 = polynomial.polynomial_add(p1, p2)

print p3