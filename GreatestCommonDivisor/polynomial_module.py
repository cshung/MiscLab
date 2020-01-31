from enum import *
from rational_module import *

class polynomial(object):
    # expecting a list of coefficients starting from constant to higher degree
    def __init__(self, coefficients):
        self.__coefficients = coefficients
        self.__simplify()

    def __simplify(self):
        while len(self.__coefficients) > 1 and self.__coefficients[len(self.__coefficients) - 1].isZero():
            self.__coefficients.pop()
    #
    # expecting a string like 2x^2 + 3x - 2
    # The grammar of a polynomial is as follow:
    #
    # <polynomial> := <positive-polynomial>
    # <polynomial> := -<positive-polynomial>
    # <positive-polynomial> := <term>
    # <positive-polynomial> := <term> + <positive-polynomial>
    # <positive-polynomial> := <term> - <positive-polynomial>
    # <term> := <rational><power>
    # <term> := <power>
    # <power> := x
    # <power> := x^<integer>
    # <rational> := <integer>
    # <rational> := (<integer>/<integer>)
    #
    @classmethod
    def from_string(cls, s):
        parser = polynomial.__parser(s)
        (succeed, terms) = parser.parse()
        if not succeed:
            raise ValueError(s + " is not a well formed polynomial")
        else:
            max_power = -1
            coefficients = {}
            for i in range(0, len(terms)):
                term = terms[i]
                coefficient = term[0]
                power = term[1]
                max_power = max(max_power, power)
                if power in coefficients:
                    coefficients[power] = rational.add(coefficients[power], coefficient)
                else:
                    coefficients[power] = coefficient
            result = []
            for i in range(0, max_power + 1):
                if i in coefficients:
                    result.append(coefficients[i])
                else:
                    result.append(rational.from_integer(0))
            return cls(result)

    def degree(self):
        return len(self.__coefficients) - 1

    def isZero(self):
        return (len(self.__coefficients) == 1) and self.__coefficients[0].isZero()

    @staticmethod
    def polynomial_add(operand1, operand2):
        length1 = operand1.degree() + 1
        length2 = operand2.degree() + 1
        result = []
        if (length1 < length2):
            for i in range(0, length1):
                result.append(rational.add(operand1.__coefficients[i], operand2.__coefficients[i]))
            for i in range(length1, length2):
                result.append(operand2.__coefficients[i])
        else:
            for i in range(0, length2):
                result.append(rational.add(operand1.__coefficients[i], operand2.__coefficients[i]))
            for i in range(length2, length1):
                result.append(operand1.__coefficients[i])
        return polynomial(result)

    @staticmethod
    def polynomial_subtract(operand1, operand2):
        length1 = operand1.degree() + 1
        length2 = operand2.degree() + 1
        result = []
        if (length1 < length2):
            for i in range(0, length1):
                result.append(rational.subtract(operand1.__coefficients[i], operand2.__coefficients[i]))
            for i in range(length1, length2):
                result.append(rational.multiply(rational.from_integer(-1), operand2.__coefficients[i]))
        else:
            for i in range(0, length2):
                result.append(rational.subtract(operand1.__coefficients[i], operand2.__coefficients[i]))
            for i in range(length2, length1):
                result.append(operand1.__coefficients[i])
        return polynomial(result)

    @staticmethod
    def __polynomial_multiply_term(operand1, operand2_coefficient, operand2_power):
        result = []
        # shift the coefficients
        for i in range(0, operand2_power):
            result.append(rational.from_integer(0))
        for i in operand1.__coefficients:
            result.append(rational.multiply(i, operand2_coefficient))
        return polynomial(result)

    @staticmethod
    def polynomial_multiply(operand1, operand2):
        result = polynomial([rational.from_integer(0)])
        for i in range(0, len(operand2.__coefficients)):
            result = polynomial.polynomial_add(result, polynomial.__polynomial_multiply_term(operand1, operand2.__coefficients[i], i))
        return result

    @staticmethod
    def polynomial_divide(operand1, operand2):
        max_power = -1
        quotient_terms = {}
        dividend = operand1
        divisor = operand2
        while divisor.degree() <= dividend.degree() and (not dividend.isZero()):
            divisor_degree = divisor.degree()
            dividend_degree = dividend.degree()
            divisor_leading_term_coefficient = divisor.__coefficients[divisor_degree]
            dividend_leading_term_coefficient = dividend.__coefficients[dividend_degree]
            quotient_term_coefficient = rational.divide(dividend_leading_term_coefficient, divisor_leading_term_coefficient)
            quotient_term_power = dividend_degree - divisor_degree
            max_power = max(max_power, quotient_term_power)
            quotient_terms[quotient_term_power] = quotient_term_coefficient
            dividend = polynomial.polynomial_subtract(dividend, polynomial.__polynomial_multiply_term(divisor, quotient_term_coefficient, quotient_term_power))

        quotient_coefficients = []
        for i in range(0, max_power + 1):
            if i in quotient_terms:
                quotient_coefficients.append(quotient_terms[i])
            else:
                quotient_coefficients.append(rational.from_integer(0))
        return (polynomial(quotient_coefficients), dividend)

    @staticmethod
    def polynomial_gcd(operand1, operand2):
        result = polynomial.__polynomial_gcd(operand1, operand2)
        if len(result.__coefficients) > 0:
            leading_term_coefficient = result.__coefficients[result.degree()]
            result = polynomial.__polynomial_multiply_term(result, rational.divide(rational.from_integer(1), leading_term_coefficient), 0)
        return result

    @staticmethod
    def __polynomial_gcd(operand1, operand2):
        if (operand1.degree() < operand2.degree()):
            return polynomial.polynomial_gcd(operand2, operand1)
        else:
            (quotient, remainder) = polynomial.polynomial_divide(operand1, operand2)
            if remainder.isZero():
                return operand2
            elif remainder.degree() == 0:
                return polynomial.from_string("1")
            else:
                return polynomial.__polynomial_gcd(operand2, remainder)

    @staticmethod
    def polynomial_extended_gcd(operand1, operand2):
        (p, q, h) = polynomial.__polynomial_extended_gcd(operand1, operand2)
        if len(h.__coefficients) > 0:
            leading_term_coefficient = h.__coefficients[h.degree()]
            # p * operand1 + q * operand2 = h
            # p * a * operand1 + q * a * operand2 = h * a
            a = rational.divide(rational.from_integer(1), leading_term_coefficient)
            p = polynomial.__polynomial_multiply_term(p, a, 0)
            q = polynomial.__polynomial_multiply_term(q, a, 0)
            h = polynomial.__polynomial_multiply_term(h, a, 0)
        return (p, q, h)

    @staticmethod
    def __polynomial_extended_gcd(operand1, operand2):
        if (operand1.degree() < operand2.degree()):
            return polynomial.polynomial_extended_gcd(operand2, operand1)
        else:
            (quotient, remainder) = polynomial.polynomial_divide(operand1, operand2)
            if remainder.isZero():
                # operand1 = operand2 * quotient
                # operand1 - operand2 * quotient = 0
                # operand1 + operand 2 - operand2 * quotient = 0
                # operand1 + operand2 * (1 - quotient) = operand2
                one = polynomial.from_string("1")
                return (one, polynomial.polynomial_subtract(one, quotient),  operand2)
            else:
                (p, q, h) = polynomial.__polynomial_extended_gcd(operand2, remainder)
                # operand1 - operand2 * quotient = remainder
                # p * operand2 + q * remainder = h
                # p * operand2 + q * (operand1 - operand2 * quotient) = h
                # q * operand1 + (p - q * quotient) * operand2 = h
                return (q, polynomial.polynomial_subtract(p, polynomial.polynomial_multiply(q, quotient)), h)

    def derivative(self):
        result = []
        for i in range(1, self.degree() + 1):
            result.append(rational.multiply(self.__coefficients[i], rational.from_integer(i)))
        return polynomial(result)

    def __str__(self):
        result = ""
        for j in range(0, len(self.__coefficients)):
            power = len(self.__coefficients) - 1 - j
            coefficient = self.__coefficients[power]

            if power == 0:
                displayCoefficient = True
                if (self.degree() > 0):
                    if coefficient.isInteger():
                        if (coefficient.integerValue() == 0):
                            displayCoefficient = False
                displayPower = False
            else:
                if (not coefficient.isInteger()):
                    displayPower = True
                    displayCoefficient = True
                elif coefficient.integerValue() == 0:
                    displayPower = False
                    displayCoefficient = False
                elif coefficient.integerValue() == 1:
                    displayPower = True
                    displayCoefficient = False
                elif coefficient.integerValue() == -1:
                    displayPower = True
                    displayCoefficient = False
                else:
                    displayPower = True
                    displayCoefficient = True

            if displayPower or displayCoefficient:
                if j == 0:
                    if (coefficient.isInteger()):
                        if coefficient.integerValue() == -1:
                            result += "-"
                else:
                    if (coefficient.isPositive()):
                        result += " + "
                    else:
                        coefficient = rational.multiply(coefficient, rational.from_integer(-1))
                        result += " - "

            if displayCoefficient:
                result += str(coefficient)

            if displayPower:
                result += "x"
                if not power == 1:
                    result += "^"
                    result += str(power)

        return result

    tokens = enum('RBRACKET','LBRACKET','SLASH','CARET', 'X', 'PLUS', 'MINUS', 'EOF', 'INTEGER', 'ERROR')

    class __scanner(object):
        def __init__(self, s):
            self.__s = s
            # position represents the position of the yet to read character
            self.__position = 0

        def scan(self):
            # Skip all spaces
            while (self.__position < len(self.__s)) and (self.__s[self.__position] == ' '):
                self.__position = self.__position + 1

            # Check EOF early to make sure we read within bound
            if self.__position == len(self.__s):
                return (polynomial.tokens.EOF, 0)

            scanned_character = self.__s[self.__position]

            single_character_map = {
                '(': (polynomial.tokens.LBRACKET, '('),
                ')': (polynomial.tokens.RBRACKET, ')'),
                '/': (polynomial.tokens.SLASH, '/'),
                '^': (polynomial.tokens.CARET, '^'),
                'x': (polynomial.tokens.X, 'x'),
                '+': (polynomial.tokens.PLUS, '+'),
                '-': (polynomial.tokens.MINUS, '-'),
            }

            # all the single character cases
            if (scanned_character in single_character_map):
                self.__position = self.__position + 1
                return single_character_map[scanned_character]
            else:
                if not (scanned_character.isdigit()):
                    return (polynomial.tokens.ERROR, -1)
                else:
                    result = 0
                    while (self.__position < len(self.__s)) and (self.__s[self.__position].isdigit()):
                        result *= 10
                        result += ord(self.__s[self.__position]) - ord('0')
                        self.__position = self.__position + 1
                    return (polynomial.tokens.INTEGER, result)

    class __parser(object):
        def __init__(self, s):
            self.__scanner = polynomial._polynomial__scanner(s)
            self.__scan()

        def __scan(self):
            self.__token = self.__scanner.scan()

        def parse(self):
            if self.__token[0] == polynomial.tokens.MINUS:
                self.__scan()
                (succeed, result) = self.__parse_positive_polynomial()
                if succeed:
                    leading_term = result[len(result) - 1]
                    leading_term_coefficient = leading_term[0]
                    leading_term_coefficient = rational.multiply(rational.from_integer(-1), leading_term_coefficient)
                    leading_term = (leading_term_coefficient, leading_term[1])
                    result[len(result) - 1] = leading_term
                    return (True, result)
                else:
                    return (False, None)
            else:
                return self.__parse_positive_polynomial();

        def __parse_positive_polynomial(self):
            (succeed, term_coefficient, term_power) = self.__parse_term()
            first_term = (term_coefficient, term_power)
            if succeed:
                if (self.__token[0] == polynomial.tokens.PLUS):
                    self.__scan()
                    (succeed, remaining_terms) = self.__parse_positive_polynomial()
                    if succeed:
                        remaining_terms.append(first_term)
                        return (True, remaining_terms)
                elif (self.__token[0] == polynomial.tokens.MINUS):
                    self.__scan()
                    (succeed, remaining_terms) = self.__parse_positive_polynomial()
                    if succeed:
                        remaining_leading_term = remaining_terms[len(remaining_terms) - 1]
                        remaining_leading_term_coefficient = remaining_leading_term[0]
                        remaining_leading_term_coefficient = rational.multiply(rational.from_integer(-1), remaining_leading_term_coefficient)
                        remaining_leading_term = (remaining_leading_term_coefficient, remaining_leading_term[1])
                        remaining_terms[len(remaining_terms) - 1] = remaining_leading_term
                        remaining_terms.append(first_term)
                        return (True, remaining_terms)
                elif (self.__token[0] == polynomial.tokens.EOF):
                    return (True, [first_term])
            else:
                return (False, None)

        def __parse_term(self):
            if (self.__token[0] == polynomial.tokens.X):
                return self.__parse_power(rational.from_integer(1))
            else:
                (succeed, coefficient) = self.__parse_rational()
                if (succeed):
                    return self.__parse_power(coefficient)

            return (False, None, None)

        def __parse_power(self, coefficient):
            if (self.__token[0] == polynomial.tokens.X):
                self.__scan()
                if (self.__token[0] == polynomial.tokens.CARET):
                    self.__scan()
                    if (self.__token[0] == polynomial.tokens.INTEGER):
                        power = self.__token[1]
                        self.__scan()
                        return (True, coefficient, power)
                else:
                    return (True, coefficient, 1)
            else:
                return (True, coefficient, 0)

        def __parse_rational(self):
            if (self.__token[0] == polynomial.tokens.LBRACKET):
                self.__scan()
                if (self.__token[0] == polynomial.tokens.INTEGER):
                    numerator = self.__token[1]
                    self.__scan()
                    if (self.__token[0] == polynomial.tokens.SLASH):
                        self.__scan()
                        if (self.__token[0] == polynomial.tokens.INTEGER):
                            denominator = self.__token[1]
                            self.__scan()
                            if (self.__token[0] == polynomial.tokens.RBRACKET):
                                self.__scan()
                                return (True, rational.from_numerator_denominator(numerator, denominator))
            elif (self.__token[0] == polynomial.tokens.INTEGER):
                value = self.__token[1]
                self.__scan()
                return (True, rational.from_integer(value))

            return (False, None)