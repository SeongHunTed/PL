package RD_JAVA;


public class States {
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
