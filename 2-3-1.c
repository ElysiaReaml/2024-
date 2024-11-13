#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 链表节点定义：用于存储高精度数的每一位
typedef struct Node {
    int digit; // 单个数位
    struct Node *next; // 指向下一个节点
} Node;

// 创建链表节点
Node* create_node(int digit) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    new_node->digit = digit;
    new_node->next = NULL;
    return new_node;
}

// 反转链表（方便操作）
Node* reverse_list(Node* head) {
    Node *prev = NULL, *curr = head, *next;
    while (curr) {
        next = curr->next;
        curr->next = prev;
        prev = curr;
        curr = next;
    }
    return prev;
}

// 打印链表中的数字
void print_list(Node* head) {
    Node* curr = head;
    while (curr) {
        printf("%d", curr->digit);
        curr = curr->next;
    }
    printf("\n");
}

// 将字符串形式的十进制数转换为链表
Node* decimal_to_list(const char* num_str) {
    Node *head = NULL, *tail = NULL;
    for (int i = 0; num_str[i] != '\0'; i++) {
        Node* new_node = create_node(num_str[i] - '0');
        if (!head) {
            head = tail = new_node;
        } else {
            tail->next = new_node;
            tail = new_node;
        }
    }
    return head;
}

// 将链表表示的十进制数转换为整数（用于进制转换）
long long list_to_int(Node* head) {
    long long result = 0;
    Node* curr = head;
    while (curr) {
        result = result * 10 + curr->digit;
        curr = curr->next;
    }
    return result;
}

// 十进制数转换为 N 进制字符串
void decimal_to_n_base(Node* decimal_list, int base, char* n_base_str) {
    long long decimal_value = list_to_int(decimal_list); // 将链表转换为整数
    int index = 0;

    // 将整数转换为 N 进制字符串
    while (decimal_value > 0) {
        int remainder = decimal_value % base;
        n_base_str[index++] = (remainder < 10) ? remainder + '0' : remainder - 10 + 'A';
        decimal_value /= base;
    }
    n_base_str[index] = '\0';

    // 反转 N 进制字符串
    for (int i = 0; i < index / 2; i++) {
        char temp = n_base_str[i];
        n_base_str[i] = n_base_str[index - i - 1];
        n_base_str[index - i - 1] = temp;
    }
}

// 将 N 进制字符串转换为十进制链表
Node* n_base_to_decimal(const char* n_base_str, int base) {
    long long decimal_value = 0;

    // 逐位解析 N 进制字符串并计算十进制值
    for (int i = 0; n_base_str[i] != '\0'; i++) {
        int digit = (n_base_str[i] >= 'A') ? n_base_str[i] - 'A' + 10 : n_base_str[i] - '0';
        decimal_value = decimal_value * base + digit;
    }

    // 将十进制整数转换为链表形式
    char decimal_str[100];
    sprintf(decimal_str, "%lld", decimal_value);
    return decimal_to_list(decimal_str);
}

// 高精度链表的加法：ab 链表相加
Node* add_lists(Node *a, Node *b) {
    a = reverse_list(a);
    b = reverse_list(b);

    Node *result = NULL, *tail = NULL;
    int carry = 0;

    while (a || b || carry) {
        int sum = (a ? a->digit : 0) + (b ? b->digit : 0) + carry;
        carry = sum / 10;
        sum = sum % 10;

        Node *new_node = create_node(sum);
        if (!result) {
            result = tail = new_node;
        } else {
            tail->next = new_node;
            tail = new_node;
        }

        if (a) a = a->next;
        if (b) b = b->next;
    }

    return reverse_list(result);
}


// 高精度减法：a - b，假设 a >= b
Node* subtract_lists(Node* a, Node* b) {
    a = reverse_list(a);
    b = reverse_list(b);

    Node *result = NULL, *tail = NULL;
    int borrow = 0;

    // 遍历链表，逐位相减
    while (a || b) {
        int diff = (a ? a->digit : 0) - (b ? b->digit : 0) - borrow;
        if (diff < 0) {
            diff += 10;
            borrow = 1;
        } else {
            borrow = 0;
        }

        Node* new_node = create_node(diff);
        if (!result) {
            result = tail = new_node;
        } else {
            tail->next = new_node;
            tail = new_node;
        }

        if (a) a = a->next;
        if (b) b = b->next;
    }

    // 去掉前导 0
    result = reverse_list(result);
    while (result && result->digit == 0) {
        Node* temp = result;
        result = result->next;
        free(temp);
    }

    return result ? result : create_node(0); // 如果结果为 0，返回单个节点 0
}

// 高精度乘法：返回两个链表的积
Node* multiply_lists(Node* a, Node* b) {
    a = reverse_list(a);
    b = reverse_list(b);

    // 初始化结果为 0
    Node* result = create_node(0);
    Node* b_curr = b;
    int b_pos = 0;

    // 遍历 b 的每一位，与 a 相乘
    while (b_curr) {
        Node* temp_result = NULL;
        Node* tail = NULL;
        int carry = 0;

        // 在当前结果前补 b_pos 个 0
        for (int i = 0; i < b_pos; i++) {
            Node* zero_node = create_node(0);
            if (!temp_result) {
                temp_result = tail = zero_node;
            } else {
                tail->next = zero_node;
                tail = zero_node;
            }
        }

        Node* a_curr = a;
        // 遍历 a 的每一位，与 b_curr->digit 相乘
        while (a_curr || carry) {
            int product = (a_curr ? a_curr->digit : 0) * b_curr->digit + carry;
            carry = product / 10;
            product = product % 10;

            Node* new_node = create_node(product);
            if (!temp_result) {
                temp_result = tail = new_node;
            } else {
                tail->next = new_node;
                tail = new_node;
            }

            if (a_curr) a_curr = a_curr->next;
        }

        // 将当前部分积累加到结果中
        result = add_lists(result, reverse_list(temp_result));

        b_curr = b_curr->next;
        b_pos++;
    }

    return reverse_list(result);
}

// 主函数：用户输入并调用操作
int main() {
    char decimal_str1[100], decimal_str2[100], n_base_str[100];
    int n;

    // 输入两个十进制高精度数
    printf("输入第一个十进制高精度数: ");
    scanf("%s", decimal_str1);
    printf("输入第二个十进制高精度数: ");
    scanf("%s", decimal_str2);

    // 输入要转换的进制 N (N < 20)
    printf("输入要转换的进制 N (N < 20): ");
    scanf("%d", &n);

    // 将输入的十进制字符串转换为链表
    Node* decimal1 = decimal_to_list(decimal_str1);
    Node* decimal2 = decimal_to_list(decimal_str2);

    // 进行加、减、乘操作
    Node* sum = add_lists(decimal1, decimal2);
    Node* diff = subtract_lists(decimal1, decimal2);
    Node* product = multiply_lists(decimal1, decimal2);

    // 打印十进制结果
    printf("加法结果: ");
    print_list(sum);
    printf("减法结果: ");
    print_list(diff);
    printf("乘法结果: ");
    print_list(product);

    // 将加减乘法结果转换为 N 进制并打印
    decimal_to_n_base(sum, n, n_base_str);
    printf("加法结果 (%d 进制): %s\n", n, n_base_str);
    decimal_to_n_base(diff, n, n_base_str);
    printf("减法结果 (%d 进制): %s\n", n, n_base_str);
    decimal_to_n_base(product, n, n_base_str);
    printf("乘法结果 (%d 进制): %s\n", n, n_base_str);

    // 输入一个 N 进制数，转换为十进制
    printf("输入一个 %d 进制数: ", n);
    scanf("%s", n_base_str);
    Node* decimal_from_n = n_base_to_decimal(n_base_str, n);

    // 打印 N 进制转换后的十进制结果
    printf("%d 进制转换为十进制: ", n);
    print_list(decimal_from_n);

    return 0;
}
