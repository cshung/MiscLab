class rational(object):
    def __init__(self,  numerator,  denonminator):
        # __ is a python convention for private fields
        if denonminator == 0:
            raise ValueError("denonminator should not be 0")

        self.__numerator = numerator
        self.__denominator = denonminator
        self.__simplify()

    # Python does not support having multiple constructor
    # This is a pattern for factory method
    @classmethod
    def from_integer(cls, i):
        return cls(i, 1)

    @classmethod
    def from_numerator_denominator(cls,  numerator,  denominator):
        return cls(numerator, denominator)

    @staticmethod
    def add(operand1, operand2):
        return rational((operand1.__numerator * operand2.__denominator + operand1.__denominator * operand2.__numerator), operand1.__denominator * operand2.__denominator)

    @staticmethod
    def subtract(operand1, operand2):
        return rational((operand1.__numerator * operand2.__denominator - operand1.__denominator * operand2.__numerator), operand1.__denominator * operand2.__denominator)

    @staticmethod
    def multiply(operand1, operand2):
        return rational(operand1.__numerator * operand2.__numerator, operand1.__denominator * operand2.__denominator)

    @staticmethod
    def divide(operand1, operand2):
        return rational(operand1.__numerator * operand2.__denominator, operand1.__denominator * operand2.__numerator)

    def isZero(self):
        return self.__numerator == 0

    def isInteger(self):
        return self.__denominator == 1

    def integerValue(self):
        if self.isInteger():
            return self.__numerator
        else:
            raise ValueError("Not an integer")

    def isPositive(self):
        return self.__numerator >= 0

    def __gcd(self, a, b):
        if (a < 0):
            a = -a
        if (b < 0):
            b = -b
        if (b > a):
            return self.__gcd(b, a)
        else:
            if (a % b == 0):
                return b
            else:
                return self.__gcd(b, a % b)

    def __simplify(self):
        if self.__numerator == 0:
            self.__denominator = 1
        else:
            cancel = self.__gcd(self.__numerator, self.__denominator)
            self.__numerator = self.__numerator / cancel
            self.__denominator = self.__denominator / cancel
            if self.__denominator < 0:
                self.__numerator = self.__numerator * -1
                self.__denominator = self.__denominator * -1

    def __str__(self):
        if self.__denominator == 1:
            return str(self.__numerator)
        else:
            return str(self.__numerator) + "/" + str(self.__denominator)