package pl;

public enum Symbol {
    NUMBER,
    LP,
    RP,
    PLUS,
    MINUS,
    MULTIPLY,
    DIVIDE,
    UNKNOWN;

    public static Symbol getSymbol(char c) {
        switch (c) {
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                return NUMBER;
            case '(':
                return LP;
            case ')':
                return RP;
            case '+':
                return PLUS;
            case '-':
                return MINUS;
            case '*':
                return MULTIPLY;
            case '/':
                return DIVIDE;
            default:
                return UNKNOWN;
        }
    }
}
