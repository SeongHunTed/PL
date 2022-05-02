
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




