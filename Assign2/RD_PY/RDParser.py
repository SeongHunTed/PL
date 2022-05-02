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


class States:

    def __init__(self, formula):
        self.trim_Formula = formula
        self.current_Index = 0
        self.count_Parens = 0

    def nextToken(self):
        if self.current_Index < len(self.trim_Formula):
            ch = self.trim_Formula[self.current_Index]
            if ch == "(":
                self.count_Parens += 1
            elif ch == ')':
                self.count_Parens -= 1
            self.current_Index += 1

    def getToken(self):
        if self.current_Index < len(self.trim_Formula):
            return self.trim_Formula[self.current_Index]
        else:
            return 0

def expr(States):
    a = term(states)
    while(True):
        if Symbol.get_symbol(states.getToken()) == Symbol.PLUS:
            states.nextToken()
            a = a + term(states)
        elif Symbol.get_symbol(states.getToken()) == Symbol.MINUS:
            states.nextToken()
            a = a - term(states)
        else:
            return a

def term(States):
    a = factor(states)
    while(True):
        if Symbol.get_symbol(states.getToken()) == Symbol.MULTIPLY:
            states.nextToken()
            a = a * factor(states)
        elif Symbol.get_symbol(states.getToken()) == Symbol.DIVIDE:
            states.nextToken()
            b = factor(states)
            if b == 0:
                print("Division by zero!")
                exit(0)
            a = a / b
        else:
            return a

def factor(States):
    if Symbol.get_symbol(states.getToken()) == Symbol.NUMBER:
        return number(states)
    elif Symbol.get_symbol(states.getToken()) == Symbol.LP:
        states.nextToken()
        expression = expr(states)
        if Symbol.get_symbol(states.getToken()) != Symbol.RP:
            print("Syntax Error!")
            exit()
        states.nextToken()
        return expression
    else:
        print("here")
        return 0

def number(States):
    result = 0
    while Symbol.get_symbol(states.getToken()) == Symbol.NUMBER:
        result = result * 10 + int(states.getToken())
        states.nextToken()
    if Symbol.get_symbol(states.getToken()) == Symbol.LP:
        print("Syntax Error!")
        exit()
    elif Symbol.get_symbol(states.getToken()) == Symbol.RP and states.count_Parens == 0:
        print("Syntax Error!")
        exit()
    return result

example = input(">> ")
example = example.replace(" ", "")
states = States(example)
print(expr(states))
