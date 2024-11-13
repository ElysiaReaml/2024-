#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_PRECISION 200

// 链表节点结构体
typedef struct Node {
    int data; // 节点数据，表示小数的某一位
    struct Node *next; // 指向下一个节点
} Node;

// 线性表 ADT 链式存储实现
typedef struct LinkedList {
    Node *head; // 指向链表头节点
} LinkedList;

// 初始化链表
LinkedList* init_linked_list() {
    LinkedList *list = (LinkedList*) malloc(sizeof(LinkedList));
    list->head = NULL;
    return list;
}

// 在链表尾部添加节点
void append(LinkedList *list, int data) {
    Node *new_node = (Node*) malloc(sizeof(Node));
    new_node->data = data;
    new_node->next = NULL;

    if (list->head == NULL) {
        list->head = new_node;
    } else {
        Node *current = list->head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_node;
    }
}

// 在指定位置插入节点
void insert(LinkedList *list, int index, int data) {
    Node *new_node = (Node*) malloc(sizeof(Node));
    new_node->data = data;

    if (index == 0) {
        new_node->next = list->head;
        list->head = new_node;
    } else {
        int count = 0;
        Node *current = list->head;
        while (current != NULL && count < index - 1) {
            current = current->next;
            count++;
        }
        if (current != NULL) {
            new_node->next = current->next;
            current->next = new_node;
        }
    }
}

// 删除指定位置的节点
void delete(LinkedList *list, int index) {
    if (index == 0) {
        list->head = list->head->next;
    } else {
        int count = 0;
        Node *current = list->head;
        while (current != NULL && count < index - 1) {
            current = current->next;
            count++;
        }
        if (current != NULL && current->next != NULL) {
            current->next = current->next->next;
        }
    }
}

// 获取指定位置的值
int get(LinkedList *list, int index) {
    int count = 0;
    Node *current = list->head;
    while (current != NULL && count < index) {
        current = current->next;
        count++;
    }
    if (current != NULL) {
        return current->data;
    } else {
        return -1; // 返回-1表示索引无效
    }
}

// 打印链表
void print_list(LinkedList *list) {
    Node *current = list->head;
    while (current != NULL) {
        printf("%d", current->data);
        current = current->next;
    }
    printf("\n");
}

// 将字符数组转换为链表
LinkedList* string_to_linked_list(char *str) {
    LinkedList *list = init_linked_list();
    int len = strlen(str);
    for (int i = 0; i < len; i++) {
        append(list, str[i] - '0');
    }
    return list;
}

// 将链表转换为字符数组
char* linked_list_to_string(LinkedList *list) {
    Node *current = list->head;
    int len = 0;
    while (current != NULL) {
        len++;
        current = current->next;
    }
    char *str = (char*) malloc((len + 1) * sizeof(char));
    current = list->head;
    int i = 0;
    while (current != NULL) {
        str[i++] = current->data + '0';
        current = current->next;
    }
    str[len] = '\0';
    return str;
}


// 十进制转二进制 (修改版)
LinkedList* decimal_to_binary(char *decimal_str) {
    char *integer_part = strtok(decimal_str, ".");
    char *fractional_part = strtok(NULL, ".");

    LinkedList *binary_list = init_linked_list();

    // 整数部分转换
    int num = atoi(integer_part);
    while (num > 0) {
        insert(binary_list, 0, num % 2); // 逆序插入
        num /= 2;
    }
    
    // 小数部分转换
    if (fractional_part != NULL) {

        char fractional_str[MAX_PRECISION];
        strcpy(fractional_str, "0.");
        strcat(fractional_str, fractional_part);
        double fractional = atof(fractional_str);


        for (int i = 0; i < 200 && fractional > 0; i++) {  // 控制精度
            fractional *= 2;
            int digit = (int)fractional;
            append(binary_list, digit);
            fractional -= digit;
        }

    }
    return binary_list;
}

// 十进制转 N 进制 (修改版)
LinkedList* decimal_to_n_base(char *decimal_str, int n) {
    char *integer_part = strtok(decimal_str, ".");
    char *fractional_part = strtok(NULL, ".");

    LinkedList *n_base_list = init_linked_list();

    // 整数部分转换
    int num = atoi(integer_part);
    while (num > 0) {
        insert(n_base_list, 0, num % n); // 逆序插入
        num /= n;
    }
    // 小数部分转换
    if (fractional_part != NULL) {
        
        char fractional_str[MAX_PRECISION];
        strcpy(fractional_str, "0.");
        strcat(fractional_str, fractional_part);
        double fractional = atof(fractional_str);

        for (int i = 0; i < 200 && fractional > 0; i++) {  // 控制精度
            fractional *= n;
            int digit = (int)fractional;
            append(n_base_list, digit);
            fractional -= digit;
        }
    }
    return n_base_list;
}

// 高精度运算函数
// 高精度加法
LinkedList* add_high_precision(LinkedList *num1, LinkedList *num2) {
    LinkedList *result = init_linked_list();
    Node *current1 = num1->head;
    Node *current2 = num2->head;
    int carry = 0;

    while (current1 != NULL || current2 != NULL || carry != 0) {
        int digit1 = current1 != NULL ? current1->data : 0;
        int digit2 = current2 != NULL ? current2->data : 0;
        int sum = digit1 + digit2 + carry;
        append(result, sum % 10);
        carry = sum / 10;
        if (current1 != NULL) {
            current1 = current1->next;
        }
        if (current2 != NULL) {
            current2 = current2->next;
        }
    }
    return result;
}

// 高精度减法
LinkedList* subtract_high_precision(LinkedList *num1, LinkedList *num2) {
    LinkedList *result = init_linked_list();
    Node *current1 = num1->head;
    Node *current2 = num2->head;
    int borrow = 0;

    while (current1 != NULL || current2 != NULL || borrow != 0) {
        int digit1 = current1 != NULL ? current1->data : 0;
        int digit2 = current2 != NULL ? current2->data : 0;
        int diff = digit1 - digit2 - borrow;
        if (diff < 0) {
            diff += 10;
            borrow = 1;
        } else {
            borrow = 0;
        }
        append(result, diff);
        if (current1 != NULL) {
            current1 = current1->next;
        }
        if (current2 != NULL) {
            current2 = current2->next;
        }
    }
    return result;
}

// 高精度乘法
LinkedList* multiply_high_precision(LinkedList *num1, LinkedList *num2) {
    LinkedList *result = init_linked_list();
    Node *current1 = num1->head;
    while (current1 != NULL) {
        Node *current2 = num2->head;
        int carry = 0;
        int i = 0;
        while (current2 != NULL || carry != 0) {
            int digit2 = current2 != NULL ? current2->data : 0;
            int product = current1->data * digit2 + carry;
            // 获取结果链表当前位置的值，没有则为0
            int current_value = i < (result->head) ? 0 : get(result, i);
            int sum = current_value + product % 10;
            if (i < result->head) {
                append(result, sum); // 如果结果链表当前位置没有值，则追加
            } else {
                insert(result, i, sum); // 否则，插入到当前位置
            }
            carry = (product / 10 + sum / 10);
            i++;
            if (current2 != NULL) {
                current2 = current2->next;
            }
        }
        current1 = current1->next;
    }
    return result;
}

// 多项式求值函数
double polynomial_evaluation(double x, double *coefficients, int degree) {
    double result = 0.0;
    for (int i = 0; i <= degree; i++) {
        result += coefficients[i] * pow(x, i);
    }
    return result;
}

int main() {
    // 数制转换测试
    char decimal_str[] = "123.456";
    printf("十进制 %s 转换为二进制: ", decimal_str);
    LinkedList *binary_list = decimal_to_binary(decimal_str);
    print_list(binary_list);

    printf("二进制 ");
    print_list(binary_list);
    printf("转换为十进制: %.3f\n", binary_to_decimal(binary_list));

    int n = 8;
    printf("十进制 %s 转换为 %d 进制: ", decimal_str, n);
    LinkedList *n_base_list = decimal_to_n_base(decimal_str, n);
    print_list(n_base_list);
    printf("\n");

    // 高精度运算测试
    char num1_str[] = "1234567890.1234567890";
    char num2_str[] = "9876543210.9876543210";
    LinkedList *num1 = string_to_linked_list(num1_str);
    LinkedList *num2 = string_to_linked_list(num2_str);

    printf("高精度加法: ");
    LinkedList *result_add = add_high_precision(num1, num2);
    print_list(result_add);
    printf("\n");

    printf("高精度减法: ");
    LinkedList *result_subtract = subtract_high_precision(num1, num2);
    print_list(result_subtract);
    printf("\n");

    printf("高精度乘法: ");
    LinkedList *result_multiply = multiply_high_precision(num1, num2);
    print_list(result_multiply);
    printf("\n");

    // 多项式求值测试
    double coefficients[] = {4, -1.0/3, 3.0/7}; // f(x) = 3/7x^3 - 1/3x^2 + 4
    double x = 1.4;
    int degree = 2;
    double result = polynomial_evaluation(x, coefficients, degree);
    printf("f(x) = %.200f\n", result);

    // 释放内存
    free(binary_list);
    free(n_base_list);
    free(num1);
    free(num2);
    free(result_add);
    free(result_subtract);
    free(result_multiply);

    return 0;
}