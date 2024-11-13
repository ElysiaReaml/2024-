#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// 定义链表节点结构体
typedef struct Node {
    int data;
    struct Node* next;
} Node;

// 创建新节点
Node* createNode(int data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}

// 在链表头部插入节点
void insertNode(Node** head, int data) {
    Node* newNode = createNode(data);
    newNode->next = *head;
    *head = newNode;
}

// 打印链表
void printList(Node* head) {
    while (head != NULL) {
        printf("%d", head->data);
        head = head->next;
        if (head != NULL) {
            printf(" ");
        }
    }
    printf("\n");
}



// 十进制小数转换为二进制小数
Node* decimalToBinary(char* decimal) {
    // 处理小数点
    char* decimal_integer = strtok(decimal, ".");
    char* decimal_fraction = strtok(NULL, ".");

    // 处理整数部分
    Node* head = NULL;
    int integer = atoi(decimal_integer);
    while (integer > 0) {
        insertNode(&head, integer % 2);
        integer /= 2;
    }

    // 处理小数部分
    if (decimal_fraction != NULL) {
        double fraction = atof(decimal_fraction);
        for (int i = 0; i < 32; i++) {
            fraction *= 2;
            if (fraction >= 1) {
                insertNode(&head, 1);
                fraction -= 1;
            } else {
                insertNode(&head, 0);
            }
        }
    }

    return head;
}

// 二进制小数转换为十进制小数
char* binaryToDecimal(Node* head) {
    // 处理整数部分
    Node* current = head;
    int integer = 0;
    int power = 0;
    while (current != NULL) {
        integer += current->data * pow(2, power);
        power++;
        current = current->next;
    }

    // 处理小数部分
    double fraction = 0.0;
    power = -1;
    while (current != NULL) {
        fraction += current->data * pow(2, power);
        power--;
        current = current->next;
    }

    // 转换为字符串
    char* decimal = (char*)malloc(1000 * sizeof(char));
    sprintf(decimal, "%d.%lf", integer, fraction);
    return decimal;
}

// 十进制小数转换为 N 进制小数
Node* decimalToNary(char* decimal, int n) {
    // 处理小数点
    char* decimal_integer = strtok(decimal, ".");
    char* decimal_fraction = strtok(NULL, ".");

    // 处理整数部分
    Node* head = NULL;
    int integer = atoi(decimal_integer);
    while (integer > 0) {
        insertNode(&head, integer % n);
        integer /= n;
    }

    // 处理小数部分
    if (decimal_fraction != NULL) {
        double fraction = atof(decimal_fraction);
        for (int i = 0; i < 32; i++) {
            fraction *= n;
            if (fraction >= 1) {
                insertNode(&head, (int)fraction);
                fraction -= (int)fraction;
            } else {
                insertNode(&head, 0);
            }
        }
    }

    return head;
}

// N 进制小数转换为十进制小数
char* naryToDecimal(Node* head, int n) {
    // 处理整数部分
    Node* current = head;
    int integer = 0;
    int power = 0;
    while (current != NULL) {
        integer += current->data * pow(n, power);
        power++;
        current = current->next;
    }

    // 处理小数部分
    double fraction = 0.0;
    power = -1;
    while (current != NULL) {
        fraction += current->data * pow(n, power);
        power--;
        current = current->next;
    }

    // 转换为字符串
    char* decimal = (char*)malloc(1000 * sizeof(char));
    sprintf(decimal, "%d.%lf", integer, fraction);
    return decimal;
}

// 高精度加法
Node* add(Node* num1, Node* num2) {
    Node* head = NULL;
    int carry = 0;
    while (num1 != NULL || num2 != NULL || carry) {
        int sum = 0;
        if (num1 != NULL) {
            sum += num1->data;
            num1 = num1->next;
        }
        if (num2 != NULL) {
            sum += num2->data;
            num2 = num2->next;
        }
        sum += carry;
        carry = sum / 2;
        insertNode(&head, sum % 2);
    }
    return head;
}

// 高精度减法
Node* subtract(Node* num1, Node* num2) {
    Node* head = NULL;
    int borrow = 0;
    while (num1 != NULL || num2 != NULL || borrow) {
        int difference = 0;
        if (num1 != NULL) {
            difference += num1->data;
            num1 = num1->next;
        }
        if (num2 != NULL) {
            difference -= num2->data;
            num2 = num2->next;
        }
        difference -= borrow;
        if (difference < 0) {
            borrow = 1;
            difference += 2;
        } else {
            borrow = 0;
        }
        insertNode(&head, difference);
    }
    return head;
}

// 高精度乘法
Node* multiply(Node* num1, Node* num2) {
    Node* head = NULL;
    Node* current = NULL;
    int carry = 0;
    while (num2 != NULL) {
        Node* temp = NULL;
        int digit = num2->data;
        Node* num1_copy = num1;
        int power = 0;
        while (num1_copy != NULL) {
            int product = digit * num1_copy->data + carry;
            carry = product / 2;
            insertNode(&temp, product % 2);
            num1_copy = num1_copy->next;
            power++;
        }
        if (carry) {
            insertNode(&temp, carry);
            carry = 0;
        }
        // 将乘积结果添加到链表末尾
        for (int i = 0; i < power; i++) {
            insertNode(&temp, 0);
        }
        if (head == NULL) {
            head = temp;
        } else {
            current = head;
            while (current->next != NULL) {
                current = current->next;
            }
            current->next = temp;
        }
        num2 = num2->next;
    }
    return head;
}

// 计算多项式值
void evaluatePolynomial(double x, int n) {
    double result = 0.0;
    // 计算多项式每一项的值并累加
    for (int i = 3; i >= 0; i--) {
        result = result * x + (3.0 / 7.0) * pow(x, i) - (1.0 / 3.0) * pow(x, i - 1) + 4;
    }
    // 输出结果
    printf("多项式 f(x) = (3/7)x^3 - (1/3)x^2 + 4 在 x = %.1lf 时的值为：%.200lf\n", x, result);
}

int main() {
    char decimal1[1000], decimal2[1000], binary[1000];
    int n;

    // 输入两个高精度十进制小数
    printf("输入第一个高精度十进制小数：");
    scanf("%s", decimal1);
    printf("输入第二个高精度十进制小数：");
    scanf("%s", decimal2);

    // 输入一个高精度二进制小数
    printf("输入一个高精度二进制小数：");
    scanf("%s", binary);

    // 输入进制数
    printf("输入进制数 (小于 20)：");
    scanf("%d", &n);

    // 将第一个十进制小数转换为二进制
    Node* binary1 = decimalToBinary(decimal1);
    printf("第一个十进制小数转换为二进制：");
    printList(binary1);

    // 将第二个十进制小数转换为二进制
    Node* binary2 = decimalToBinary(decimal2);
    printf("第二个十进制小数转换为二进制：");
    printList(binary2);

    // 将二进制小数转换为十进制
    char* decimal3 = binaryToDecimal(binary);
    printf("二进制小数转换为十进制：%s\n", decimal3);

    // 将第一个十进制小数转换为 N 进制
    Node* nary1 = decimalToNary(decimal1, n);
    printf("第一个十进制小数转换为 %d 进制：", n);
    printList(nary1);

    // 将第二个十进制小数转换为 N 进制
    Node* nary2 = decimalToNary(decimal2, n);
    printf("第二个十进制小数转换为 %d 进制：", n);
    printList(nary2);

    // 高精度加法
    Node* sum = add(binary1, binary2);
    printf("二进制加法结果：");
    printList(sum);
    printf("十进制加法结果：%s\n", binaryToDecimal(sum));
    printf("%d 进制加法结果：", n);
    printList(naryToDecimal(sum, n));

    // 高精度减法
    Node* difference = subtract(binary1, binary2);
    printf("二进制减法结果：");
    printList(difference);
    printf("十进制减法结果：%s\n", binaryToDecimal(difference));
    printf("%d 进制减法结果：", n);
    printList(naryToDecimal(difference, n));

    // 高精度乘法
    Node* product = multiply(binary1, binary2);
    printf("二进制乘法结果：");
    printList(product);
    printf("十进制乘法结果：%s\n", binaryToDecimal(product));
    printf("%d 进制乘法结果：", n);
    printList(naryToDecimal(product, n));

    // 多项式求值
    evaluatePolynomial(1.4, 200);

    return 0;
}