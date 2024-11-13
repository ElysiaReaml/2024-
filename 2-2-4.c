#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    char digit; // 存储数字字符
    struct Node* next;
} Node;

typedef struct BigDecimal {
    Node* head; // 链表头
    int decimal_point; // 小数点位置
} BigDecimal;

// 创建新节点
Node* createNode(char digit) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->digit = digit;
    newNode->next = NULL;
    return newNode;
}

// 创建高精度小数
BigDecimal* createBigDecimal(const char* number) {
    BigDecimal* bd = (BigDecimal*)malloc(sizeof(BigDecimal));
    bd->head = NULL;
    bd->decimal_point = 0;

    int len = strlen(number);
    Node* current = NULL;

    for (int i = 0; i < len; i++) {
        if (number[i] == '.') {
            bd->decimal_point = i;
            continue;
        }
        Node* newNode = createNode(number[i]);
        if (bd->head == NULL) {
            bd->head = newNode;
            current = bd->head;
        } else {
            current->next = newNode;
            current = current->next;
        }
    }

    return bd;
}

// 打印高精度小数
void printBigDecimal(BigDecimal* bd) {
    Node* current = bd->head;
    int index = 0;
    while (current != NULL) {
        printf("%c", current->digit);
        index++;
        if (index == bd->decimal_point) {
            printf(".");
        }
        current = current->next;
    }
    printf("\n");
}

// 加法运算
BigDecimal* addBigDecimal(BigDecimal* a, BigDecimal* b) {
    BigDecimal* result = (BigDecimal*)malloc(sizeof(BigDecimal));
    result->head = NULL;
    result->decimal_point = a->decimal_point > b->decimal_point ? a->decimal_point : b->decimal_point;

    Node* a_curr = a->head;
    Node* b_curr = b->head;
    Node* lastNode = NULL;

    int carry = 0;
    int pos = 0;

    while (a_curr || b_curr || carry) {
        int sum = carry;
        if (pos < a->decimal_point && a_curr) {
            sum += a_curr->digit - '0';
            a_curr = a_curr->next;
        }
        if (pos < b->decimal_point && b_curr) {
            sum += b_curr->digit - '0';
            b_curr = b_curr->next;
        }
        if (pos >= a->decimal_point && a_curr) {
            sum += a_curr->digit - '0';
            a_curr = a_curr->next;
        }
        if (pos >= b->decimal_point && b_curr) {
            sum += b_curr->digit - '0';
            b_curr = b_curr->next;
        }

        carry = sum / 10;
        Node* newNode = createNode((sum % 10) + '0');
        if (!result->head) {
            result->head = newNode;
            lastNode = result->head;
        } else {
            lastNode->next = newNode;
            lastNode = lastNode->next;
        }
        pos++;
    }

    return result;
}

// 减法运算（假设 a >= b）
BigDecimal* subtractBigDecimal(BigDecimal* a, BigDecimal* b) {
    BigDecimal* result = (BigDecimal*)malloc(sizeof(BigDecimal));
    result->head = NULL;
    result->decimal_point = a->decimal_point; // 小数点位置与被减数相同

    Node* a_curr = a->head;
    Node* b_curr = b->head;
    Node* lastNode = NULL;

    int borrow = 0;
    int pos = 0;

    while (a_curr || b_curr || borrow) {
        int diff = borrow;
        if (pos < a->decimal_point && a_curr) {
            diff += a_curr->digit - '0';
            a_curr = a_curr->next;
        }
        if (pos < b->decimal_point && b_curr) {
            diff -= (b_curr->digit - '0');
            b_curr = b_curr->next;
        }
        if (pos >= a->decimal_point && a_curr) {
            diff += a_curr->digit - '0';
            a_curr = a_curr->next;
        }
        if (pos >= b->decimal_point && b_curr) {
            diff -= (b_curr->digit - '0');
            b_curr = b_curr->next;
        }

        if (diff < 0) {
            diff += 10;
            borrow = 1;
        } else {
            borrow = 0;
        }

        Node* newNode = createNode(diff + '0');
        if (!result->head) {
            result->head = newNode;
            lastNode = result->head;
        } else {
            lastNode->next = newNode;
            lastNode = lastNode->next;
        }
        pos++;
    }

    return result;
}

// 乘法运算
BigDecimal* multiplyBigDecimal(BigDecimal* a, BigDecimal* b) {
    BigDecimal* result = (BigDecimal*)malloc(sizeof(BigDecimal));
    result->head = NULL;
    result->decimal_point = a->decimal_point + b->decimal_point; // 小数点位置为两数小数点之和

    Node* a_curr = a->head;
    Node* b_curr = b->head;

    int res_len = 0; // 结果链表长度
    Node* temp_result[100] = {0}; // 临时结果数组

    for (int i = 0; a_curr != NULL; i++) {
        b_curr = b->head;
        int j = 0;
        while (b_curr != NULL) {
            int product = (a_curr->digit - '0') * (b_curr->digit - '0');

            // 累加到对应位置
            if (temp_result[i + j] == NULL) {
                temp_result[i + j] = createNode(0);
            }
            temp_result[i + j]->digit += product;

            // 处理进位
            if (temp_result[i + j]->digit >= 10) {
                int carry = temp_result[i + j]->digit / 10;
                temp_result[i + j]->digit %= 10;

                if (temp_result[i + j + 1] == NULL) {
                    temp_result[i + j + 1] = createNode(carry);
                } else {
                    temp_result[i + j + 1]->digit += carry;
                }
            }

            j++;
            b_curr = b_curr->next;
        }
        a_curr = a_curr->next;
    }

    // 构建结果链表
    Node* lastNode = NULL;
    for (int k = 0; k < 100; k++) {
        if (temp_result[k] != NULL) {
            if (result->head == NULL) {
                result->head = temp_result[k];
                lastNode = result->head;
            } else {
                lastNode->next = temp_result[k];
                lastNode = lastNode->next;
            }
        }
    }

    return result;
}

// 主函数示例
int main() {
    BigDecimal* num1 = createBigDecimal("123.456");
    BigDecimal* num2 = createBigDecimal("77.654");

    printf("Number 1: ");
    printBigDecimal(num1);
    printf("Number 2: ");
    printBigDecimal(num2);

    BigDecimal* sum = addBigDecimal(num1, num2);
    printf("Sum: ");
    printBigDecimal(sum);

    BigDecimal* difference = subtractBigDecimal(num1, num2);
    printf("Difference: ");
    printBigDecimal(difference);

    BigDecimal* product = multiplyBigDecimal(num1, num2);
    printf("Product: ");
    printBigDecimal(product);

    // 释放内存（略）

    return 0;
}
