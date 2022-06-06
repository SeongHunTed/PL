from enum import Enum


class Symbol(Enum):
    NUMBER = 1
    LP = 2
    RP = 3
    PLUS = 4
    MINUS = 5
    MULTIPLY = 6
    DIVIDE = 7
    UNKNOWN = 8

    def get_symbol(c):
        if (c == '0' or c == '1' or c == '2' or c == '3' or c == '4' or c == '5' or c == '6' or c == '7' or c == '8' or c == '9'):
            return Symbol.NUMBER
        elif (c == '('):
            return Symbol.LP
        elif (c == ')'):
            return Symbol.RP
        elif (c == '+'):
            return Symbol.PLUS
        elif (c == '-'):
            return Symbol.MINUS
        elif (c == '*'):
            return Symbol.MULTIPLY
        elif (c == '/'):
            return Symbol.DIVIDE
        else:
            return Symbol.UNKNOWN
