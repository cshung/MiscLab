from enum import *
from rational_module import *

class polynomial(object):
    # expecting a list of coefficients starting from constant to higher degree
    def __init__(self, coefficients):
        self.__coefficients = coefficients

    # expecting a string like 2x^2 + 3x - 2
    # The grammar of a polynomial is as follow
    # <polynomial> := <term>
    # <polynomial> := <term> + <polynomial>
    # <polynomial> := <term> - <polynomial>
    # <term> := <rational>
    # <term> := <rational>x
    # <term> := <rational>x^<integer>
    # <rational> := <integer>
    # <rational> := (<integer>/<integer>)
    @classmethod
    def from_string(cls, s):
        parser = polynomial.__parser(s)
        (a, b, c) = parser._parser__parse_term()
        print a
        print b
        print c
        return cls([])

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
            (succeed, term) = self.__parse_term()

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
                return (True, rational.from_integer())

            return (False, None)