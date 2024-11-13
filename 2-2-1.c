#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// 定义项的结构体，保存多项式中的每一项
typedef struct Term {
    long double coefficient;  // 系数（支持高精度）
    int exponent;             // 幂次
    struct Term* next;        // 链表结构，指向下一项
} Term;

// 创建新项（系数 + 幂次）
Term* createTerm(long double coefficient, int exponent) {
    Term* newTerm = (Term*)malloc(sizeof(Term));
    if (newTerm == NULL) {
        fprintf(stderr, "内存分配失败！\n");
        exit(1); // 退出程序
    }
    newTerm->coefficient = coefficient;
    newTerm->exponent = exponent;
    newTerm->next = NULL;
    return newTerm;
}

// 将解析的项添加到链表
void addTerm(Term** head, long double coefficient, int exponent) {
    Term* newTerm = createTerm(coefficient, exponent);
    newTerm->next = *head;
    *head = newTerm;
}

// 解析形如 "3/7*x^3" 的字符串，并返回系数与幂次
void parseTerm(const char* termStr, long double* coefficient, int* exponent) {
    char coefPart[50] = {0};
    char expPart[50] = {0};
    int hasFraction = 0;

    // 查找分数部分和幂次部分
    const char* xPos = strchr(termStr, 'x');
    if (xPos) {
        strncpy(coefPart, termStr, xPos - termStr);  // 提取系数部分
        const char* expPos = strchr(xPos, '^');
        if (expPos) {
            strcpy(expPart, expPos + 1);  // 提取幂次部分
            *exponent = atoi(expPart);
        } else {
            *exponent = 1;  // 如果没有幂次，则默认为 1
        }
    } else {
        strcpy(coefPart, termStr);  // 如果没有变量，整个就是系数
        *exponent = 0;
    }

    // 解析系数部分（支持分数）
    if (strchr(coefPart, '/')) {
        hasFraction = 1;
    }
    if (hasFraction) {
        int num, denom;
        sscanf(coefPart, "%d/%d", &num, &denom);
        if (denom == 0) {
            fprintf(stderr, "错误：分母不能为 0！\n");
            exit(1); // 退出程序
        }
        *coefficient = (long double)num / denom;
    } else {
        *coefficient = atof(coefPart);
    }
}

// 解析多项式字符串，将其拆分为链表项
void parsePolynomial(const char* polyStr, Term** head) {
    char buffer[256];
    strcpy(buffer, polyStr);

    // 按空格或加减号拆分多项式
    char* termStr = strtok(buffer, "+-");
    while (termStr != NULL) {
        long double coefficient;
        int exponent;
        parseTerm(termStr, &coefficient, &exponent);
        addTerm(head, coefficient, exponent);
        termStr = strtok(NULL, "+-");
    }
}

// 计算多项式在 x 处的值
long double evaluatePolynomial(Term* head, long double x) {
    long double result = 0.0;
    Term* temp = head;

    while (temp != NULL) {
        // 先计算系数，再乘以 x 的幂次
        result += temp->coefficient * powl(x, temp->exponent);  
        temp = temp->next;
    }

    return result;
}

// 释放链表
void freePolynomial(Term* head) {
    while (head) {
        Term* temp = head;
        head = head->next;
        free(temp);
    }
}

// 主函数：测试多项式解析和求值
int main() {
    char polyStr[256];
    long double x;
    Term* polynomial = NULL;

    // 输入多项式字符串
    printf("请输入多项式（如 '3/7*x^3 - 1/3*x^2 + 4'）: ");
    fgets(polyStr, sizeof(polyStr), stdin);
    polyStr[strcspn(polyStr, "\n")] = 0;  // 移除换行符

    // 输入 x 的值
    printf("请输入 x 的值: ");
    scanf("%Lf", &x);

    // 解析多项式字符串并求值
    parsePolynomial(polyStr, &polynomial);
    long double result = evaluatePolynomial(polynomial, x);

    // 输出结果
    printf("多项式在 x = %.10Lf 处的值为: %.10Lf\n", x, result);

    // 释放内存
    freePolynomial(polynomial);

    return 0;
}