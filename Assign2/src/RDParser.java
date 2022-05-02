import java.util.Scanner;

public class RDParser {
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

    public static class States {
        String trimFormula; // 수식
        int currentIndex; // 현재 인덱스
        int countParens; // 괄호 카운트

        public States(String formula) {
            trimFormula = formula.trim().replace(" ", "");
            currentIndex = 0;
            countParens = 0;
        }

        public void nextToken() {
            if (currentIndex < trimFormula.length()) {
                char ch = trimFormula.charAt(currentIndex);
                if (ch == '(') {
                    countParens++;
                } else if (ch == ')') {
                    countParens--;
                }
                currentIndex++;
            }
        }

        public char getToken() {
            if (currentIndex < trimFormula.length()) {
                return trimFormula.charAt(currentIndex);
            } else {
                return 0;
            }
        }
    }

    public static void main(String[] args) {
        System.out.print(">>");
        Scanner scanner = new Scanner(System.in);
        States states = new States(scanner.nextLine());
        System.out.println(expr(states));
    }

    public static int expr(States states) {
        int a = term(states);
        while (true) {
            if (Symbol.getSymbol(states.getToken()) == Symbol.PLUS) {
                states.nextToken();
                a = a + term(states);
            } else if (Symbol.getSymbol(states.getToken()) == Symbol.MINUS) {
                states.nextToken();
                a = a - term(states);
            } else {
                return a;
            }
        }
    }

    public static int term(States states) {
        int a = factor(states);
        while (true) {
            if (Symbol.getSymbol(states.getToken()) == Symbol.MULTIPLY) {
                states.nextToken();
                a = a * factor(states);
            } else if (Symbol.getSymbol(states.getToken()) == Symbol.DIVIDE) {
                states.nextToken();
                int b = factor(states);
                if (b == 0) {
                    throw new RuntimeException("Division by zero");
                }
                a = a / b;
            } else {
                return a;
            }
        }
    }

    public static int factor(States states) {
        int expression;

        if (Symbol.getSymbol(states.getToken()) == Symbol.NUMBER) {
            return number(states);
        } else if (Symbol.getSymbol(states.getToken()) == Symbol.LP) {
            states.nextToken();

            expression = expr(states);

            if (Symbol.getSymbol(states.getToken()) != Symbol.RP) {
                error();
            }

            states.nextToken();
            return expression;

        } else {
            error();
            return 0;
        }
    }

    public static int number(States states) {
        int result = 0;
        while (Symbol.getSymbol(states.getToken()) == Symbol.NUMBER) {
            result = result * 10 + states.getToken() - '0';
            states.nextToken();
        }
        if (Symbol.getSymbol(states.getToken()) == Symbol.LP) {
            error();
        } else if (Symbol.getSymbol(states.getToken()) == Symbol.RP && states.countParens == 0) {
            error();
        }
        return result;
    }

    public static void error(){
        System.out.println("Syntax Error!");
        System.exit(0);
    }
}

