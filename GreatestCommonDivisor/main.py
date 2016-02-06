from polynomial_module import *

p1 = polynomial.from_string("1x + 1")
p2 = polynomial.polynomial_multiply(p1, p1)
p3 = polynomial.from_string("1x + 2")
(p4, p5) = polynomial.polynomial_divide(p2, p3)

print p1 # 1x + 1
print p2 # 1x^2 + 2x + 1
print p3 # 1x + 2
print p4 # x 
print p5 # 1
