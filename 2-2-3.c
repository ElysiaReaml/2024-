#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 定义链表节点
typedef struct Node {
    int digit; // 存储每一位数字
    struct Node *next; 
} Node;

// 创建新的节点
Node* create_node(int digit) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    new_node->digit = digit;
    new_node->next = NULL;
    return new_node;
}

// 将字符串转换为链表（用于初始化）
Node* string_to_list(char* str) {
    Node* head = NULL;
    Node* tail = NULL;
    int len = strlen(str);
    for (int i = 0; i < len; i++) {
        if (str[i] >= '0' && str[i] <= '9') {
            Node* new_node = create_node(str[i] - '0');
            if (head == NULL) {
                head = new_node;
                tail = new_node;
            } else {
                tail->next = new_node;
                tail = new_node;
            }
        }
    }
    return head;
}

// 打印链表
void print_list(Node* head) {
    Node* current = head;
    while (current != NULL) {
        printf("%d", current->digit);
        current = current->next;
    }
    printf("\n");
}

// 高精度小数加法
Node* add_high_precision(Node* num1, Node* num2) {
    Node* head = NULL;
    Node* tail = NULL;
    int carry = 0; // 进位
    
    while (num1 != NULL || num2 != NULL || carry) {
        int digit1 = (num1 != NULL) ? num1->digit : 0;
        int digit2 = (num2 != NULL) ? num2->digit : 0;
        int sum = digit1 + digit2 + carry;
        carry = sum / 10;
        sum %= 10;

        Node* new_node = create_node(sum);
        if (head == NULL) {
            head = new_node;
            tail = new_node;
        } else {
            tail->next = new_node;
            tail = new_node;
        }

        if (num1 != NULL) {
            num1 = num1->next;
        }
        if (num2 != NULL) {
            num2 = num2->next;
        }
    }

    return head;
}

// 高精度小数减法
Node* subtract_high_precision(Node* num1, Node* num2) {
    // 为了方便，这里先判断 num1 是否大于 num2
    // 如果 num1 小于 num2，则将结果取反
    if (compare_high_precision(num1, num2) < 0) {
        return negate_high_precision(subtract_high_precision(num2, num1));
    }

    Node* head = NULL;
    Node* tail = NULL;
    int borrow = 0; // 借位

    while (num1 != NULL || num2 != NULL || borrow) {
        int digit1 = (num1 != NULL) ? num1->digit : 0;
        int digit2 = (num2 != NULL) ? num2->digit : 0;
        int diff = digit1 - digit2 - borrow;
        if (diff < 0) {
            diff += 10;
            borrow = 1;
        } else {
            borrow = 0;
        }

        Node* new_node = create_node(diff);
        if (head == NULL) {
            head = new_node;
            tail = new_node;
        } else {
            tail->next = new_node;
            tail = new_node;
        }

        if (num1 != NULL) {
            num1 = num1->next;
        }
        if (num2 != NULL) {
            num2 = num2->next;
        }
    }

    return head;
}

// 高精度小数乘法 (简易版)
Node* multiply_high_precision(Node* num1, Node* num2) {
    Node* head = NULL;
    Node* tail = NULL;

    // 使用循环遍历 num2 的每一位
    Node* current = num2;
    int multiplier = 1; // 当前位的乘数
    while (current != NULL) {
        // 创建一个新链表，用于存储当前位的乘积
        Node* product = multiply_by_digit(num1, current->digit);

        // 将新链表乘以 multiplier，并插入到结果链表中
        product = multiply_by_factor(product, multiplier);
        head = add_high_precision(head, product);

        current = current->next;
        multiplier *= 10;
    }

    return head;
}

// 高精度小数乘以一位数字
Node* multiply_by_digit(Node* num1, int digit) {
    Node* head = NULL;
    Node* tail = NULL;
    int carry = 0;

    Node* current = num1;
    while (current != NULL || carry) {
        int product = (current != NULL) ? current->digit * digit + carry : carry;
        carry = product / 10;
        product %= 10;

        Node* new_node = create_node(product);
        if (head == NULL) {
            head = new_node;
            tail = new_node;
        } else {
            tail->next = new_node;
            tail = new_node;
        }

        if (current != NULL) {
            current = current->next;
        }
    }

    return head;
}

// 高精度小数乘以一个因子
Node* multiply_by_factor(Node* num, int factor) {
    Node* head = NULL;
    Node* tail = NULL;
    int carry = 0;

    Node* current = num;
    while (current != NULL || carry) {
        int product = (current != NULL) ? current->digit * factor + carry : carry;
        carry = product / 10;
        product %= 10;

        Node* new_node = create_node(product);
        if (head == NULL) {
            head = new_node;
            tail = new_node;
        } else {
            tail->next = new_node;
            tail = new_node;
        }

        if (current != NULL) {
            current = current->next;
        }
    }

    return head;
}

// 比较两个高精度小数的大小
int compare_high_precision(Node* num1, Node* num2) {
    // 先比较长度
    int len1 = list_length(num1);
    int len2 = list_length(num2);
    if (len1 > len2) {
        return 1; // num1 大于 num2
    } else if (len1 < len2) {
        return -1; // num1 小于 num2
    } else {
        // 长度相等，逐位比较
        Node* current1 = num1;
        Node* current2 = num2;
        while (current1 != NULL && current2 != NULL) {
            if (current1->digit > current2->digit) {
                return 1; // num1 大于 num2
            } else if (current1->digit < current2->digit) {
                return -1; // num1 小于 num2
            }
            current1 = current1->next;
            current2 = current2->next;
        }
        // 所有位都相同，则相等
        return 0;
    }
}

// 返回链表长度
int list_length(Node* head) {
    int length = 0;
    Node* current = head;
    while (current != NULL) {
        length++;
        current = current->next;
    }
    return length;
}

// 对高精度小数取反
Node* negate_high_precision(Node* num) {
    Node* head = NULL;
    Node* tail = NULL;
    int borrow = 1; // 借位
    Node* current = num;

    while (current != NULL || borrow) {
        int digit = (current != NULL) ? current->digit : 0;
        int diff = 10 - digit - borrow;
        if (diff < 0) {
            diff += 10;
            borrow = 1;
        } else {
            borrow = 0;
        }

        Node* new_node = create_node(diff);
        if (head == NULL) {
            head = new_node;
            tail = new_node;
        } else {
            tail->next = new_node;
            tail = new_node;
        }

        if (current != NULL) {
            current = current->next;
        }
    }

    return head;
}

// 释放链表
void free_list(Node* head) {
    Node* current = head;
    while (current != NULL) {
        Node* temp = current;
        current = current->next;
        free(temp);
    }
}

int main() {
    char num1_str[] = "1234567890";
    char num2_str[] = "9876543210";
    Node* num1 = string_to_list(num1_str);
    Node* num2 = string_to_list(num2_str);

    printf("num1: ");
    print_list(num1);
    printf("num2: ");
    print_list(num2);

    printf("num1 + num2: ");
    Node* sum = add_high_precision(num1, num2);
    print_list(sum);
    free_list(sum);

    printf("num1 - num2: ");
    Node* diff = subtract_high_precision(num1, num2);
    print_list(diff);
    free_list(diff);

    printf("num1 * num2: ");
    Node* product = multiply_high_precision(num1, num2);
    print_list(product);
    free_list(product);

    free_list(num1);
    free_list(num2);

    return 0;
}
