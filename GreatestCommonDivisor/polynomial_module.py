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
    # Currently the grammar has a bug, it does not support representing -2 as a
    # literal
    # One also need to enter 1x, which is kind of odd
    #
    # <polynomial> := <term>
    # <polynomial> := <term> + <polynomial>
    # <polynomial> := <term> - <polynomial>
    # <term> := <rational>
    # <term> := <rational>x
    # <term> := <rational>x^<integer>
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
                result.append(rational.add(operand1.__coefficients[i]))
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
                result.append(rational.subtract(operand2.__coefficients[i]))
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
        quoient_terms = {}
        dividend = operand1
        divisor = operand2
        while divisor.degree() <= dividend.degree():
            divisor_degree = divisor.degree()
            dividend_degree = dividend.degree()
            divisor_leading_term_coefficient = divisor.__coefficients[divisor_degree]
            dividend_leading_term_coefficient = dividend.__coefficients[dividend_degree]
            quoient_term_coefficient = rational.divide(dividend_leading_term_coefficient, divisor_leading_term_coefficient)
            quoient_term_power = dividend_degree - divisor_degree
            max_power = max(max_power, quoient_term_power)
            quoient_terms[quoient_term_power] = quoient_term_coefficient
            dividend = polynomial.polynomial_subtract(dividend, polynomial.__polynomial_multiply_term(divisor, quoient_term_coefficient, quoient_term_power))

        quoient_coefficients = []
        for i in range(0, max_power + 1):
            if i in quoient_terms:
                quoient_coefficients.append(quoient_terms[i])
            else:
                quoient_coefficients.append(rational.from_integer(0))
        return (polynomial(quoient_coefficients), dividend)

    @staticmethod
    def polynomial_gcd(operand1, operand2):
        if (operand1.degree() < operand2.degree()):
            return polynomial.polynomial_gcd(operand2, operand1)
        else:
            (quoient, remainder) = polynomial.polynomial_divide(operand1, operand2)
            if remainder.isZero():
                return operand2
            else:
                return polynomial.polynomial_gcd(operand2, remainder)

    # TODO: for subtract term, do not output as adding a negative number
    # TODO: do not output the coefficient if it is just 1
    def __str__(self):
        result = ""
        for j in range(0, len(self.__coefficients)):
            i = len(self.__coefficients) - 1 - j
            result += str(self.__coefficients[i])
            if not i == 0:
                result += "x"
                if not i == 1:
                    result += "^"
                    result += str(i)
                result += " + " 
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
            (succeed, term_coefficient, term_power) = self.__parse_term()
            first_term = (term_coefficient, term_power)
            if succeed:
                if (self.__token[0] == polynomial.tokens.PLUS):
                    self.__scan()
                    (succeed, remaining_terms) = self.parse()
                    if succeed:
                        remaining_terms.append(first_term)
                        return (True, remaining_terms)
                elif (self.__token[0] == polynomial.tokens.MINUS):
                    self.__scan()
                    (succeed, remaining_terms) = self.parse()
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

        def __parse_term(self):
            (succeed, coefficient) = self.__parse_rational()
            if (succeed):
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

            return (False, None, None)

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