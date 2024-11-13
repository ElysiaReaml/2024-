#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 链表节点结构
typedef struct Node {
    int digit;
    struct Node *next;
} Node;

// 创建新节点
Node* create_node(int digit) {
    Node *new_node = (Node *)malloc(sizeof(Node));
    new_node->digit = digit;
    new_node->next = NULL;
    return new_node;
}

// 头插法（用于整数部分）
Node* add_to_head(Node *head, int digit) {
    Node *new_node = create_node(digit);
    new_node->next = head;
    return new_node;
}

// 尾插法（用于小数部分）
Node* add_to_tail(Node *tail, int digit) {
    Node *new_node = create_node(digit);
    if (tail) {
        tail->next = new_node;
    }
    return new_node;
}

// 打印链表
void print_list(Node *head) {
    Node *temp = head;
    while (temp) {
        printf("%d", temp->digit);
        temp = temp->next;
    }
}

// 释放链表内存
void free_list(Node *head) {
    Node *temp;
    while (head) {
        temp = head;
        head = head->next;
        free(temp);
    }
}

// 将十进制整数部分转换为 N 进制（返回链表）
Node* decimal_to_base_n_int(Node *decimal_int, int base) {
    Node *result = NULL;  // 存储结果的链表
    int num = 0;

    // 将链表中的整数部分拼接为一个整型数
    while (decimal_int) {
        num = num * 10 + decimal_int->digit;
        decimal_int = decimal_int->next;
    }

    // 转换为 N 进制
    while (num > 0) {
        int remainder = num % base;
        result = add_to_head(result, remainder);
        num /= base;
    }
    return result;
}

// 将十进制小数部分转换为 N 进制（返回链表）
Node* decimal_to_base_n_frac(Node *decimal_frac, int base, int precision) {
    Node *result = NULL, *tail = NULL;
    double fraction = 0.0;

    // 将链表中的小数部分转为浮点数
    double divider = 1;
    while (decimal_frac) {
        fraction += decimal_frac->digit / (divider *= 10);
        decimal_frac = decimal_frac->next;
    }

    // 转换为 N 进制（保留指定精度）
    for (int i = 0; i < precision; i++) {
        fraction *= base;
        int digit = (int)fraction;
        fraction -= digit;

        // 使用尾插法构建链表
        if (!result) {
            result = tail = create_node(digit);
        } else {
            tail = add_to_tail(tail, digit);
        }
    }
    return result;
}

// 将 N 进制整数部分转换为十进制（返回链表）
Node* base_n_to_decimal_int(Node *base_n_int, int base) {
    Node *result = NULL;
    int decimal_value = 0;

    // N 进制整数部分转十进制
    while (base_n_int) {
        decimal_value = decimal_value * base + base_n_int->digit;
        base_n_int = base_n_int->next;
    }

    // 将结果存为十进制链表
    while (decimal_value > 0) {
        result = add_to_head(result, decimal_value % 10);
        decimal_value /= 10;
    }
    return result;
}

// 将 N 进制小数部分转换为十进制（返回链表）
Node* base_n_to_decimal_frac(Node *base_n_frac, int base, int precision) {
    Node *result = NULL, *tail = NULL;
    double fraction = 0.0, multiplier = 1.0 / base;

    // N 进制小数部分转十进制（按权展开）
    while (base_n_frac) {
        fraction += base_n_frac->digit * multiplier;
        multiplier /= base;
        base_n_frac = base_n_frac->next;
    }

    // 将结果按十进制表示存为链表（保留指定精度）
    for (int i = 0; i < precision; i++) {
        fraction *= 10;
        int digit = (int)fraction;
        fraction -= digit;

        // 使用尾插法构建链表
        if (!result) {
            result = tail = create_node(digit);
        } else {
            tail = add_to_tail(tail, digit);
        }
    }
    return result;
}

int main() {
    // 示例：十进制整数部分转换为 N 进制
    Node *decimal_int = create_node(1);
    decimal_int = add_to_tail(decimal_int, 15);  // 表示 15

    printf("十进制整数部分: ");
    print_list(decimal_int);
    printf("\n");

    Node *base_n_int = decimal_to_base_n_int(decimal_int, 16);  // 转换为二进制
    printf("转换为二进制: ");
    print_list(base_n_int);
    printf("\n");

    // 示例：十进制小数部分转换为 N 进制
    Node *decimal_frac = create_node(1);
    decimal_frac = add_to_tail(decimal_frac, 4);  // 表示 0.14

    printf("十进制小数部分: 0.");
    print_list(decimal_frac);
    printf("\n");

    Node *base_n_frac = decimal_to_base_n_frac(decimal_frac, 16, 10);  // 转换为二进制
    printf("转换为二进制小数部分: 0.");
    print_list(base_n_frac);
    printf("\n");

    // 示例：二进制整数部分转换为十进制
    Node *back_to_decimal_int = base_n_to_decimal_int(base_n_int, 16);
    printf("二进制整数部分转回十进制: ");
    print_list(back_to_decimal_int);
    printf("\n");

    // 示例：二进制小数部分转换为十进制
    Node *back_to_decimal_frac = base_n_to_decimal_frac(base_n_frac, 16, 10);
    printf("二进制小数部分转回十进制: 0.");
    print_list(back_to_decimal_frac);
    printf("\n");

    // 释放内存
    free_list(decimal_int);
    free_list(base_n_int);
    free_list(decimal_frac);
    free_list(base_n_frac);
    free_list(back_to_decimal_int);
    free_list(back_to_decimal_frac);

    return 0;
}