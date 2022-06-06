#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

//토큰 구조체
typedef struct states{ 
    char cur_char; // 현재 인덱스의 문자
    char *b_input; // 공백 제거된 수식
    int cur_idx; // 현재인덱스
    int count_paren; // 괄호 카운트
} states;

// 토큰 분류
enum token{ NUMBER, LP, RP, PLUS, MINUS, MULTIPLY, DIVIDE, UNKNOWN};

// 토큰 구분 함수
enum token symbol(char c){
    switch(c){
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':   return NUMBER;
        case '(':   return LP;
        case ')':   return RP;
        case '+':   return PLUS;
        case '-':   return MINUS;
        case '*':   return MULTIPLY;
        case '/':   return DIVIDE;
        default:    return UNKNOWN;
    }
};

char input[1000] = {0}; // 입력 된 식
char output[1000] = {0}; // 공백제거 식

int expr(states *ptr);
int term(states *ptr);
int factor(states *ptr);
enum token tkCheck(states *ptr);
void nextToken(states *ptr);
int number(states *ptr);
void error();
void rmSpace();

int main()
{
    printf(">> ");
    fgets(input, sizeof(input), stdin);
    rmSpace();
    states st = {output[0], output, 0, 0};
    printf("%d\n", expr(&st));

    return 0;
}

void error()
{
    fprintf(stderr, "Syntax Error!\n");
    exit(1);
}

void rmSpace()
{
    int i, j = 0;
    for(i = 0; i<strlen(input); i++){
        if(input[i] != ' '){
            output[j++] = input[i];
        }
    }
    output[j] = 0;
}

enum token tkCheck(states *ptr) // 현재 인덱스 토큰 형식
{
    return symbol(ptr->b_input[ptr->cur_idx]);
}

void nextToken(states *ptr)
{
    ptr->cur_idx += 1; // 인덱스 증가
    ptr->cur_char = ptr->b_input[ptr->cur_idx]; // 다음 문자로 갱신
}

int expr(states *ptr)                           // <expr> = <term>{ + <term> | - <term>}
{
    int _term = term(ptr);
    while(1)
    {
        if(tkCheck(ptr) == PLUS){               // + 경우
            nextToken(ptr);                     // 토큰 이동
            _term = _term + term(ptr);          // 결과 갱신
        } else if(tkCheck(ptr) == MINUS){       // - 경우
            nextToken(ptr);                     // 토큰 이동
            _term = _term - term(ptr);          // 결과 갱신
        } else return _term;                    // 결과 반환
    }
}

int term(states *ptr)                           // <term> = <factor> {* <factor> | / <factor>}
{
    int _factor = factor(ptr);                  
    while(1)
    {
        if(tkCheck(ptr) == MULTIPLY){           // * 경우
            nextToken(ptr);                     // 토큰 이동
            _factor = _factor * factor(ptr);    // 결과 갱신
        } else if(tkCheck(ptr) == DIVIDE){      // / 경우
            nextToken(ptr);                     // 토큰 이동
            int b = factor(ptr);
            if(b == 0){
                printf("Division by zero\n");
                exit(0);
            }
            _factor = _factor / b;              // 결과 갱신
        } else return _factor;                  // 결과 반환
    }
}

int factor(states *ptr)
{
    int expression = 0;

    if(tkCheck(ptr) == NUMBER){                 // number인 경우
        return number(ptr);
    } else if(tkCheck(ptr) == LP){              // (expr인경우)
        ptr->count_paren = 1;                   // paren count 증가
        nextToken(ptr);                         // 토큰 이동
        expression = expr(ptr);
        if(tkCheck(ptr) != RP){                 // rigth paren 없는 경우 에러
            error();
        }
        nextToken(ptr);
        ptr->count_paren = 0;                   // paren count 초기화
        return expression;
    } else {
        error();
        return 0;
    }
}

int number(states *ptr)
{
    int result = 0;
    while(tkCheck(ptr) == NUMBER){
        result = result * 10 + (ptr->cur_char - '0');           // 정수 읽기
        nextToken(ptr);                                         // 토큰 이동
    }
    if(tkCheck(ptr) == LP) error();
    else if(tkCheck(ptr) == RP && !ptr->count_paren) error();

    return result;
}