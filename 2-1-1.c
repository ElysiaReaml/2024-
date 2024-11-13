#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXSIZE 200  // 高精度运算最大位数

// 双向循环链表节点定义
typedef struct Node {
    char data;
    struct Node *next, *prior;
} Node, *LinkList;

// 初始化链表
LinkList InitList() {
    LinkList L = (Node *)malloc(sizeof(Node));
    L->data = '0';  // 初始化为 0
    L->next = L;
    L->prior = L;
    return L;
}

// 销毁链表
void DestroyList(LinkList L) {
    LinkList p = L->next, q;
    while (p != L) {
        q = p->next;
        free(p);
        p = q;
    }
    free(L);
}

// 在链表尾部插入数字
void CreateList_Tail(LinkList L, char num) {
    LinkList n = (Node *)malloc(sizeof(Node));
    n->data = num;
    n->next = L;
    n->prior = L->prior;
    L->prior->next = n;
    L->prior = n;
}

// 打印链表表示的数字
void PrintList(LinkList L) {
    LinkList p = L->next;
    while (p != L) {
        printf("%c", p->data);
        p = p->next;
    }
    printf("\n");
}

// 高精度链表乘法：A * B
LinkList Multiply(LinkList A, LinkList B) {
    // 初始化结果链表
    LinkList result = InitList();
    CreateList_Tail(result, '0');  // 初始结果为 0

    // 遍历 B 的每一位，逐位进行乘法，并累加到结果中
    LinkList pb = B->prior;  // 从 B 的最低位开始
    int shift = 0;  // 记录结果要偏移的位数

    while (pb != B) {
        LinkList temp = InitList();  // 存放单次乘法结果
        int carry = 0;
        LinkList pa = A->prior;  // 从 A 的最低位开始

        // 对 A 的每一位进行乘法
        while (pa != A) {
            int product = (pa->data - '0') * (pb->data - '0') + carry;
            carry = product / 10;
            CreateList_Tail(temp, (product % 10) + '0');
            pa = pa->prior;
        }

        // 处理最后的进位
        if (carry) {
            CreateList_Tail(temp, carry + '0');
        }

        // 对当前结果进行位移，并与总结果相加
        for (int i = 0; i < shift; i++) {
            CreateList_Tail(temp, '0');
        }

        result = Add(result, temp);  // 累加当前乘法结果
        DestroyList(temp);  // 释放临时链表
        pb = pb->prior;
        shift++;
    }

    return result;
}

// 高精度加法：A + B
LinkList Add(LinkList A, LinkList B) {
    LinkList result = InitList();
    int carry = 0;

    LinkList pa = A->prior, pb = B->prior;

    while (pa != A || pb != B || carry) {
        int sum = carry;
        if (pa != A) {
            sum += pa->data - '0';
            pa = pa->prior;
        }
        if (pb != B) {
            sum += pb->data - '0';
            pb = pb->prior;
        }
        carry = sum / 10;
        CreateList_Tail(result, (sum % 10) + '0');
    }

    return result;
}

// 计算分数乘法：num * (p/q)
LinkList FractionMultiply(LinkList num, int p, int q) {
    LinkList numerator = InitList();  // 存放分子 p
    CreateList_Tail(numerator, p + '0');

    LinkList temp = Multiply(num, numerator);  // 先计算 num * p
    DestroyList(numerator);

    // 处理除法 temp / q（只取整数部分）
    LinkList result = InitList();
    int remainder = 0;

    LinkList pt = temp->next;
    while (pt != temp) {
        int value = remainder * 10 + (pt->data - '0');
        CreateList_Tail(result, (value / q) + '0');
        remainder = value % q;
        pt = pt->next;
    }

    DestroyList(temp);
    return result;
}

// 幂函数：x^n（高精度）
LinkList Power(LinkList x, int n) {
    LinkList result = InitList();
    CreateList_Tail(result, '1');  // 初始值为 1

    for (int i = 0; i < n; i++) {
        LinkList temp = Multiply(result, x);
        DestroyList(result);
        result = temp;
    }

    return result;
}

// 多项式求值：f(x) = (3/7)x^3 - (1/3)x^2 + 4
LinkList EvaluatePolynomial(LinkList x) {
    // term1 = (3/7) * x^3
    LinkList term1 = Power(x, 3);
    term1 = FractionMultiply(term1, 3, 7);

    // term2 = -(1/3) * x^2
    LinkList term2 = Power(x, 2);
    term2 = FractionMultiply(term2, 1, 3);

    // 取负号处理
    term2->next->data = (term2->next->data == '0') ? '0' : '-' + '0';

    // term3 = 4
    LinkList term3 = InitList();
    CreateList_Tail(term3, '4');

    // 求和：term1 + term2 + term3
    LinkList result = Add(term1, term2);
    result = Add(result, term3);

    // 释放临时链表
    DestroyList(term1);
    DestroyList(term2);
    DestroyList(term3);

    return result;
}

int main() {
    // 初始化 x = 1.4 的链表表示
    LinkList x = InitList();
    CreateList_Tail(x, '1');
    CreateList_Tail(x, '.');
    CreateList_Tail(x, '4');

    // 计算多项式值
    LinkList result = EvaluatePolynomial(x);

    // 输出结果
    printf("多项式求值结果：");
    PrintList(result);

    // 释放内存
    DestroyList(x);
    DestroyList(result);

    return 0;
}
