#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum { INTEGER, REALNUMBER } TYPE;
struct{
    TYPE type;
    union { int i; double y; } realValue;
}typedef value;

value num;
char ch;
enum { NUMBER, PLUS, STAR, LP, RP, END } token;

value expression();
value term();
value factor();
void get_token();
void error(int);
 
value expression() {

    value result;
    result = term();

    while (token == PLUS) {

        get_token();
        value v = term();
        if (result.type == REALNUMBER && v.type == REALNUMBER) {
            result.realValue.y = result.realValue.y + v.realValue.y;
            result.type = REALNUMBER;
        }
        else if (result.type == REALNUMBER && v.type == INTEGER) {
            result.realValue.y = result.realValue.y + (double)v.realValue.i;
            result.type = REALNUMBER;
        }
        else if (result.type == INTEGER && v.type == REALNUMBER) {
            result.realValue.y = (double)result.realValue.i + v.realValue.y;
            result.type = REALNUMBER;
        }
        else {
            result.realValue.i = result.realValue.i + v.realValue.i;
            result.type = INTEGER;
        }
    }
    return (result);
}

 
value term() {
    value result;
    result = factor();
    while (token == STAR) {
        get_token();
        value v = factor();
        if (result.type == REALNUMBER && v.type == REALNUMBER) {
            result.realValue.y = result.realValue.y*v.realValue.y;
            result.type = REALNUMBER;
        }else if (result.type == REALNUMBER && v.type == INTEGER) {
            result.realValue.y = result.realValue.y*(double)v.realValue.i;
            result.type = REALNUMBER;
        }else if (result.type == INTEGER && v.type == REALNUMBER) {
            result.realValue.y = (double)result.realValue.i*v.realValue.y;
            result.type = REALNUMBER;
        }else {
            result.realValue.i = result.realValue.i * v.realValue.i;
            result.type = INTEGER;
        }
    }
    return (result);
}

 

value factor() {
    value result;
    if (token == NUMBER) {
        result = num;
        get_token();
    }else if (token == LP) {
        get_token();
        result = expression();
        if (token == RP) get_token();
        else error(2);
    }else error(1);
    return (result);
}


void get_token() {
    int val = 0;
    double val2;
    int index = 0;
    char str[100];
 
    while ((ch == ' ') || (ch == '\t')) ch = getchar(); // 토큰앞의 빈칸을 무시
        if (ch == '+') { ch = getchar(); token = PLUS; return; }
        if (ch == '*') { ch = getchar(); token = STAR; return; }
        if (ch == '(') { ch = getchar(); token = LP; return; }
        if (ch == ')') { ch = getchar(); token = RP; return; }
        if (ch == '\n') { token = END; return; }
 
        if ((ch >= '0') && (ch <= '9')) {
            do {
                val = val * 10 + ch - '0'; ch = getchar();
            } while ((ch >= '0') && (ch <= '9'));
                num.realValue.i = val;
                num.type = INTEGER;
                token = NUMBER;

        if (ch == '.') {
            val2 = (double)val;
            str[index++] = '0';
            do {
                str[index++] = ch;
                ch = getchar();
                } while ((ch >= '0') && (ch <= '9'));
                str[index] = '\0';
                num.realValue.y = val2+atof(str);
                num.type = REALNUMBER;
        }
    }
}

void error(int i)
{
    switch (i) {
    case 1: printf("여는 괄호 없음\n"); break;
    case 2: printf("닫는 괄호 없음\n"); break;
    case 3: printf("EOF가 안나옴\n"); break;
    }
    exit(1);
}

 

int main(void)
{
    value result;
    ch = ' ';
    get_token();
    result = expression();
    if (token != END) error(3);
    else result.type==INTEGER?printf("%d\n", result.realValue.i):printf("%lf\n", result.realValue.y);
    return 0;
}

