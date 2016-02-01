from enum import *

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
    # <term> := <rational>x^<integer>
    # <rational> := <integer>
    # <rational> := (<integer>/<integer>)
    @classmethod
    def from_string(cls, s):
        scanner = polynomial.__scanner(s)
        print scanner.scan()
        print scanner.scan()
        coefficients = []
        return cls(coefficients)

    tokens = enum('RBRACKET','LBRACKET','SLASH','CARET', 'X', 'PLUS', 'MINUS', 'EOF', 'DIGIT', 'ERROR')
    
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
                '(': (polynomial.tokens.RBRACKET, '('),
                ')': (polynomial.tokens.LBRACKET, ')'),
                '\\': (polynomial.tokens.SLASH, '\\'),
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
                    return (polynomial.tokens.DIGIT, result)

    class __parser(object):

        def __init__(self, s):
            self.__scanner = polynomial.__scanner(s)
            self.__token = self.__scanner.scan()            