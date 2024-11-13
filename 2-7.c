#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_DIGITS 1000 // 定义最大位数

// 定义链表节点结构
typedef struct Node {
    int digit;
    struct Node *next;
} Node;

// 创建一个新的节点
Node* createNode(int digit) {
    Node *newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        printf("Memory allocation failed!\n");
        exit(1);
    }
    newNode->digit = digit;
    newNode->next = NULL;
    return newNode;
}

// 将字符串转换为链表,带小数点
Node* stringToList(char *str) {
    int len = strlen(str);
    Node *head = NULL;
    Node *tail = NULL;
    Node* decimal_point = NULL; // 用于标记小数点位置

    for (int i = 0; i < len; i++) {
        if (str[i] >= '0' && str[i] <= '9') {  //只处理数字字符
            Node *newNode = createNode(str[i] - '0');
            if (head == NULL) {
                head = newNode;
                tail = newNode;
            } else {
                tail->next = newNode;
                tail = newNode;
            }
        } else if (str[i] == '.') {
          decimal_point = tail; // 记录小数点位置
        }
    }

    // 如果没有小数部分，添加 ".0" 
    if (decimal_point == NULL) {
      decimal_point = tail;
      Node *newNode = createNode(0);
      tail->next = newNode;
      tail = newNode;
    }


    return head;
}


// 打印链表
void printList(Node *head) {
    Node *current = head;
    int after_point = 0; // 0: before point; 1: after
    while (current != NULL) {
        if (current->digit == -1) {
          after_point = 1;
          printf(".");
        } else if (after_point == 0) {
          printf("%d", current->digit);
        } else {
          printf("%d", current->digit);
        }

        current = current->next;
    }
    printf("\n");
}

// 释放链表内存
void freeList(Node *head) {
    Node *current = head;
    while (current != NULL) {
        Node *temp = current;
        current = current->next;
        free(temp);
    }
}

// 十进制到N进制的转换 (整数部分)
char* decimalToBaseN_integer(Node* decimal, int N) {
    char baseN[MAX_DIGITS] = {0}; 
    int index = 0;
    Node* temp = decimal;
    int dec_val = 0;

    while (temp!= NULL && temp->digit != -1) {
      dec_val = dec_val * 10 + temp->digit;
      temp = temp->next;
    }

    if (dec_val == 0) {
      baseN[0] = '0';
      index = 1;
    } else {
      while(dec_val > 0) {
        int remainder = dec_val % N;
        baseN[index++] = (remainder < 10) ? remainder + '0' : remainder - 10 + 'A'; // 处理大于10进制的情况
        dec_val /= N;
      }
    }

    // 反转字符串
    int start = 0;
    int end = index-1;
    while(start < end) {
        char temp = baseN[start];
        baseN[start] = baseN[end];
        baseN[end] = temp;
        start++;
        end--;
    }

    return baseN; 
}

// 十进制到N进制的转换 (小数部分)
char* decimalToBaseN_fraction(Node* decimal_point, int N) {
    char baseN[MAX_DIGITS] = {0};
    int index = 0;
    Node* temp = decimal_point;
    double dec_val = 0.0;

    temp = temp->next; // 跳过小数点
    double frac_multiplier = 0.1;
    while (temp != NULL) {
        dec_val += temp->digit * frac_multiplier;
        frac_multiplier /= 10;
        temp = temp->next;
    }

    while (dec_val > 0 && index < MAX_DIGITS - 1) { 
        dec_val *= N; 
        int integer_part = (int)dec_val;
        baseN[index++] = (integer_part < 10) ? integer_part + '0' : integer_part - 10 + 'A';
        dec_val -= integer_part;
    }

    return baseN; 
}

// 十进制到N进制的转换 (完整)
char* decimalToBaseN(Node* decimal, int N) {
  char *integer_part = decimalToBaseN_integer(decimal, N);
  char *fraction_part = decimalToBaseN_fraction(decimal, N);

  // 判断小数部分是否为空
  if (fraction_part[0] == '\0') { 
    return integer_part; 
  } else {
    char *result = malloc(strlen(integer_part) + strlen(fraction_part) + 2); // 考虑小数点和空字符
    sprintf(result, "%s.%s", integer_part, fraction_part);
    free(integer_part);
    free(fraction_part);
    return result;
  }
}

// N进制到十进制的转换 (整数部分)
Node* baseNToDecimal_integer(char *baseN, int N) {
    int decimal = 0;
    int power = 1;
    int len = strlen(baseN);

    for(int i = len - 1; i >= 0; i--) {
        int digit = (baseN[i] >= 'A') ? baseN[i] - 'A' + 10 : baseN[i] - '0';
        decimal += digit * power;
        power *= N;
    }

    Node* head = NULL;
    Node* tail = NULL;
    do {
        Node* newNode = createNode(decimal % 10);
        if (head == NULL) {
            head = newNode;
            tail = newNode;
        } else {
          newNode -> next = head;
          head = newNode;
        }

        decimal /= 10;
    } while(decimal > 0);

    return head;
}

// N进制到十进制的转换 (小数部分)
Node* baseNToDecimal_fraction(char *baseN, int N) {
    double decimal = 0.0;
    double power = 1.0 / N; 
    int len = strlen(baseN);

    for (int i = 0; i < len; i++) {
        int digit = (baseN[i] >= 'A') ? baseN[i] - 'A' + 10 : baseN[i] - '0';
        decimal += digit * power;
        power /= N; 
    }

    Node *head = NULL;
    Node *tail = NULL;
    int digits = 0;

    while (decimal > 0 && digits < MAX_DIGITS) { 
        decimal *= 10;
        int digit = (int)decimal;
        decimal -= digit;

        Node *newNode = createNode(digit);
        if (head == NULL) {
            head = newNode;
            tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }
        digits++;
    }

    return head;
}

// N进制到十进制的转换 (完整)
Node* baseNToDecimal(char *baseN, int N) {
  // 分割整数和小数部分
  char *integer_str = strtok(baseN, ".");
  char *fraction_str = strtok(NULL, ".");

  // 转换为十进制
  Node *integer_part = baseNToDecimal_integer(integer_str, N);
  Node *fraction_part = baseNToDecimal_fraction(fraction_str, N);

  if (fraction_part != NULL) {
    // 连接整数和小数部分
    Node *temp = integer_part;
    while (temp->next != NULL) {
      temp = temp->next;
    }
    temp->digit = -1; // 使用 -1 作为小数点的标记
    temp->next = fraction_part;
  }

  return integer_part;
}

// 高精度加法
Node* add(Node* num1, Node* num2) {
    Node *result = NULL;
    Node *tail = NULL;
    int carry = 0;

    while (num1 != NULL || num2 != NULL || carry != 0) {
        int sum = carry;
        if (num1 != NULL) {
            sum += num1->digit;
            num1 = num1->next;
        }
        if (num2 != NULL) {
            sum += num2->digit;
            num2 = num2->next;
        }

        carry = sum / 10;
        Node *newNode = createNode(sum % 10);

        if (result == NULL) {
            result = newNode;
            tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }
    }

    return result;
}

// 高精度减法 (假设 num1 >= num2)
Node* subtract(Node* num1, Node* num2) {
    Node *result = NULL;
    Node *tail = NULL;
    int borrow = 0;

    while (num1 != NULL || num2 != NULL) {
        int diff = borrow;
        if (num1 != NULL) {
            diff += num1->digit;
            num1 = num1->next;
        }
        if (num2 != NULL) {
            diff -= num2->digit;
            num2 = num2->next;
        }

        if (diff < 0) {
            borrow = -1;
            diff += 10;
        } else {
            borrow = 0;
        }

        Node *newNode = createNode(diff);

        if (result == NULL) {
            result = newNode;
            tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }
    }

    // 删除前导零
    while (result->next != NULL && result->digit == 0) {
        Node *temp = result;
        result = result->next;
        free(temp);
    }

    return result;
}

// 高精度乘法
Node* multiply(Node* num1, Node* num2) {
    Node *result = createNode(0); 

    Node *temp1 = num1;
    int shift = 0; // 用于位移

    while (temp1 != NULL) {
        Node *temp2 = num2;
        Node *tempResult = NULL;
        Node *tempTail = NULL;
        int carry = 0;

        while (temp2 != NULL) {
            int product = (temp1->digit * temp2->digit) + carry;
            carry = product / 10;
            Node *newNode = createNode(product % 10);

            if (tempResult == NULL) {
                tempResult = newNode;
                tempTail = newNode;
            } else {
                tempTail->next = newNode;
                tempTail = newNode;
            }

            temp2 = temp2->next;
        }

        if (carry > 0) {
            tempTail->next = createNode(carry);
        }

        // 位移
        for (int i = 0; i < shift; i++) {
            Node *zeroNode = createNode(0);
            zeroNode->next = tempResult;
            tempResult = zeroNode;
        }

        result = add(result, tempResult); 
        freeList(tempResult); 

        temp1 = temp1->next;
        shift++;
    }

    return result;
}

// 多项式求值
double evaluatePolynomial(double coefficients[], int degree, double x) {
    double result = 0;
    for (int i = degree; i >= 0; i--) {
        result = result * x + coefficients[i];
    }
    return result;
}


int main() {
    char decimal_str[MAX_DIGITS];
    char baseN_str[MAX_DIGITS];
    int N;

    // 十进制到N进制
    printf("输入一个十进制数: ");
    scanf("%s", decimal_str);
    printf("输入目标进制 (N): ");
    scanf("%d", &N);

    Node* decimal_num = stringToList(decimal_str);

    printf("十进制: ");
    printList(decimal_num);

    char *baseN_result = decimalToBaseN(decimal_num, N);
    printf("%d进制: %s\n", N, baseN_result);
    free(baseN_result);

    // N进制到十进制
    printf("输入一个 %d 进制数(例如: 101.11): ", N);
    scanf("%s", baseN_str);

    Node *decimal_from_baseN = baseNToDecimal(baseN_str, N);
    printf("转换为十进制: ");
    printList(decimal_from_baseN);
    freeList(decimal_from_baseN);

    // 高精度加减乘
    printf("输入第一个十进制数: ");
    scanf("%s", decimal_str);
    Node* num1 = stringToList(decimal_str);

    printf("输入第二个十进制数: ");
    scanf("%s", decimal_str);
    Node* num2 = stringToList(decimal_str);

    Node *sum = add(num1, num2);
    Node *diff = subtract(num1, num2); 
    Node *product = multiply(num1, num2);

    printf("加法结果: ");
    printList(sum);
    printf("减法结果: ");
    printList(diff);
    printf("乘法结果: ");
    printList(product);

    freeList(num1);
    freeList(num2);
    freeList(sum);
    freeList(diff);
    freeList(product);

    // 多项式求值
    double coefficients[] = {4, -1.0/3, 0, 3.0/7}; 
    int degree = 3;
    double x = 1.4;

    double polynomial_result = evaluatePolynomial(coefficients, degree, x);
    printf("多项式结果: %.200lf\n", polynomial_result);


    // 释放内存
    freeList(decimal_num);

    return 0;
}