#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_LEN 1000

// 字符串反转
void my_strrev(char *str) {
    int i = 0, j = strlen(str) - 1;
    while (i < j) {
        char temp = str[i];
        str[i] = str[j];
        str[j] = temp;
        i++;
        j--;
    }
}

// 删除前导零
void removeLeadingZeros(char *str) {
    int i = 0;
    while (str[i] == '0' && str[i + 1] != '\0') {
        i++;
    }
    strcpy(str, str + i);
}

// 删除后导零
void removeTrailingZeros(char *str) {
    int len = strlen(str);
    int i = len - 1;
    while (str[i] == '0' && i > 0 && str[i - 1] != '.') {
        str[i] = '\0';
        i--;
    }
}

// 比较两个数的大小，如果 num1 >= num2 返回 1，否则返回 0
int compare(char *num1, char *num2) {
    int len1 = strlen(num1);
    int len2 = strlen(num2);
    if (len1 > len2) {
        return 1;
    } else if (len1 < len2) {
        return 0;
    } else {
        for (int i = 0; i < len1; i++) {
            if (num1[i] > num2[i]) {
                return 1;
            } else if (num1[i] < num2[i]) {
                return 0;
            }
        }
        return 1;
    }
}

// 十进制转换为二进制
char* decimalToBinary(char* decimal) {
    char* result = (char*)malloc(MAX_LEN * sizeof(char));
    char* integerPart = strtok(decimal, ".");
    char* fractionalPart = strtok(NULL, ".");

    // 整数部分转换
    int integer = atoi(integerPart);
    int index = 0;
    while (integer > 0) {
        result[index++] = (integer % 2) + '0';
        integer /= 2;
    }
    if (index == 0) {
        result[index++] = '0';
    }
    result[index] = '\0';
    my_strrev(result);

    // 小数部分转换
    if (fractionalPart != NULL) {
        result[index++] = '.';
        double fraction = atof("0." + strlen(fractionalPart));
        int count = 0;
        while (fraction > 0 && count < 32) { // 控制小数部分位数
            fraction *= 2;
            result[index++] = (int)fraction + '0';
            fraction -= (int)fraction;
            count++;
        }
    }

    return result;
}

// 二进制转换为十进制
char* binaryToDecimal(char* binary) {
    char* result = (char*)malloc(MAX_LEN * sizeof(char));
    char* integerPart = strtok(binary, ".");
    char* fractionalPart = strtok(NULL, ".");

    // 整数部分转换
    int integer = 0;
    int len = strlen(integerPart);
    for (int i = 0; i < len; i++) {
        integer += (integerPart[i] - '0') * pow(2, len - i - 1);
    }
    sprintf(result, "%d", integer);

    // 小数部分转换
    if (fractionalPart != NULL) {
        double fraction = 0;
        len = strlen(fractionalPart);
        for (int i = 0; i < len; i++) {
            fraction += (fractionalPart[i] - '0') * pow(2, -(i + 1));
        }
        char buffer[MAX_LEN];
        sprintf(buffer, ".%lf", fraction);
        strcat(result, buffer + 1); // 去掉前导 0
    }

    return result;
}

// 十进制转换为 N 进制
char* decimalToNary(char* decimal, int n) {
    char* result = (char*)malloc(MAX_LEN * sizeof(char));
    char* integerPart = strtok(decimal, ".");
    char* fractionalPart = strtok(NULL, ".");

    // 整数部分转换
    int integer = atoi(integerPart);
    int index = 0;
    while (integer > 0) {
        int remainder = integer % n;
        if (remainder < 10) {
            result[index++] = remainder + '0';
        } else {
            result[index++] = remainder - 10 + 'A';
        }
        integer /= n;
    }
    if (index == 0) {
        result[index++] = '0';
    }
    result[index] = '\0';
    my_strrev(result);

    // 小数部分转换
    if (fractionalPart != NULL) {
        result[index++] = '.';
        double fraction = atof("0." + strlen(fractionalPart));
        int count = 0;
        while (fraction > 0 && count < 32) { // 控制小数部分位数
            fraction *= n;
            int digit = (int)fraction;
            if (digit < 10) {
                result[index++] = digit + '0';
            } else {
                result[index++] = digit - 10 + 'A';
            }
            fraction -= digit;
            count++;
        }
    }

    return result;
}

// 高精度加法
char* add(char* num1, char* num2) {
    char* result = (char*)malloc(MAX_LEN * sizeof(char));
    int len1 = strlen(num1);
    int len2 = strlen(num2);    int carry = 0;
    int i, j, k;

    // 反转字符串以便从低位到高位相加
    my_strrev(num1);
    my_strrev(num2);

    // 小数点对齐
    int dot1 = strchr(num1, '.') - num1;
    int dot2 = strchr(num2, '.') - num2;
    int dotDiff = abs(dot1 - dot2);
    if (dot1 < dot2) {
        for (i = 0; i < dotDiff; i++) {
            num1[len1 + i] = '0';
        }
        num1[len1 + dotDiff] = '\0';
        len1 += dotDiff;
    } else if (dot1 > dot2) {
        for (i = 0; i < dotDiff; i++) {
            num2[len2 + i] = '0';
        }
        num2[len2 + dotDiff] = '\0';
        len2 += dotDiff;
    }

    // 从低位到高位相加
    for (i = 0, j = 0, k = 0; i < len1 || j < len2 || carry; i++, j++, k++) {
        int digit1 = (i < len1) ? (num1[i] - '0') : 0;
        int digit2 = (j < len2) ? (num2[j] - '0') : 0;
        int sum = digit1 + digit2 + carry;
        carry = sum / 10;
        result[k] = (sum % 10) + '0';
    }

    // 处理小数点
    if (dot1 != -1 || dot2 != -1) {
        result[k++] = '.';
        for (i = k - 2; i >= 0 && result[i] != '.'; i--) {
            result[k++] = result[i];
        }
        result[k] = '\0';
    }

    // 反转字符串以获得正确结果
    my_strrev(result);
    removeLeadingZeros(result);
    removeTrailingZeros(result);

    return result;
}

// 高精度减法
char* subtract(char* num1, char* num2) {
    char* result = (char*)malloc(MAX_LEN * sizeof(char));
    int len1 = strlen(num1);
    int len2 = strlen(num2);
    int borrow = 0;
    int i, j, k;

    // 如果 num2 大于 num1，交换两个数
    if (!compare(num1, num2)) {
        char *temp = num1;
        num1 = num2;
        num2 = temp;
        len1 = strlen(num1);
        len2 = strlen(num2);
    }

    // 反转字符串以便从低位到高位相减
    my_strrev(num1);
    my_strrev(num2);

    // 小数点对齐
    int dot1 = strchr(num1, '.') - num1;
    int dot2 = strchr(num2, '.') - num2;
    int dotDiff = abs(dot1 - dot2);
    if (dot1 < dot2) {
        for (i = 0; i < dotDiff; i++) {
            num1[len1 + i] = '0';
        }
        num1[len1 + dotDiff] = '\0';
        len1 += dotDiff;
    } else if (dot1 > dot2) {
        for (i = 0; i < dotDiff; i++) {
            num2[len2 + i] = '0';
        }
        num2[len2 + dotDiff] = '\0';
        len2 += dotDiff;
    }

    // 从低位到高位相减
    for (i = 0, j = 0, k = 0; i < len1 || j < len2; i++, j++, k++) {
        int digit1 = (i < len1) ? (num1[i] - '0') : 0;
        int digit2 = (j < len2) ? (num2[j] - '0') : 0;
        int diff = digit1 - digit2 - borrow;
        if (diff < 0) {
            diff += 10;
            borrow = 1;
        } else {
            borrow = 0;
        }
        result[k] = diff + '0';
    }

    // 处理小数点
    if (dot1 != -1 || dot2 != -1) {
        result[k++] = '.';
        for (i = k - 2; i >= 0 && result[i] != '.'; i--) {
            result[k++] = result[i];
        }
        result[k] = '\0';
    }

    // 反转字符串以获得正确结果
    my_strrev(result);
    removeLeadingZeros(result);
    removeTrailingZeros(result);

    return result;
}

// 高精度乘法
char* multiply(char* num1, char* num2) {
    int len1 = strlen(num1);
    int len2 = strlen(num2);
    int resultLen = len1 + len2;
    char* result = (char*)malloc(resultLen * sizeof(char));
    memset(result, '0', resultLen);

    // 小数点处理
    int dot1 = strchr(num1, '.') - num1;
    int dot2 = strchr(num2, '.') - num2;
    int decimalPlaces = 0;
    if (dot1 != -1) {
        decimalPlaces += len1 - dot1 - 1;
        num1[dot1] = '\0'; 
        len1--;
    }
    if (dot2 != -1) {
        decimalPlaces += len2 - dot2 - 1;
        num2[dot2] = '\0'; 
        len2--;
    }

    // 从低位到高位相乘
    for (int i = len1 - 1; i >= 0; i--) {
        int carry = 0;
        for (int j = len2 - 1; j >= 0; j--) {
            int product = (num1[i] - '0') * (num2[j] - '0') + (result[i + j + 1] - '0') + carry;
            carry = product / 10;
            result[i + j + 1] = (product % 10) + '0';
        }
        result[i] += carry; 
    }

    // 处理小数点
    if (decimalPlaces > 0) {
        memmove(result + (resultLen - decimalPlaces), result + (resultLen - decimalPlaces - 1), decimalPlaces + 1);
        result[resultLen - decimalPlaces - 1] = '.';
    }

    // 清理前导零
    removeLeadingZeros(result);
    removeTrailingZeros(result);

    return result;
}

// 单变量多项式求值
char* evaluatePolynomial(double x, int n) {
    // 实现多项式求值逻辑
    // f(x) = (3/7)x^3 - (1/3)x^2 + 4
    double result = (3.0 / 7.0) * pow(x, 3) - (1.0 / 3.0) * pow(x, 2) + 4;

    // 将结果转换为字符串，并保留指定精度
    char* strResult = (char*)malloc(MAX_LEN * sizeof(char));
    sprintf(strResult, "%.*lf", n, result);

    return strResult;
}

int main() {
    char a[MAX_LEN], b[MAX_LEN], c[MAX_LEN];
    int n;

    // 用户输入
    printf("请输入第一个高精度十进制小数 a: ");
    scanf("%s", a);
    printf("请输入第二个高精度十进制小数 b: ");
    scanf("%s", b);
    printf("请输入一个高精度二进制小数 c: ");
    scanf("%s", c);
    printf("请输入进制 n (n < 20): ");
    scanf("%d", &n);

    // 十进制转换为二进制
    char* aBinary = decimalToBinary(a);
    printf("a 的二进制表示: %s\n", aBinary);


    // 二进制转换为十进制
    char* cDecimal = binaryToDecimal(c);
    printf("c 的十进制表示: %s\n", cDecimal);

    // 高精度加法
    char* sum = add(a, b);
    printf("a + b = %s (十进制)\n", sum);
    printf("a + b = %s (%d 进制)\n", decimalToNary(sum, n), n);

    // 高精度减法
    char* difference = subtract(a, b);
    printf("a - b = %s (十进制)\n", difference);
    printf("a - b = %s (%d 进制)\n", decimalToNary(difference, n), n);

    // 高精度乘法
    char* product = multiply(a, b);
    printf("a * b = %s (十进制)\n", product);
    printf("a * b = %s (%d 进制)\n", decimalToNary(product, n), n);

    // 多项式求值
    double x = 1.4;
    int precision = 200;
    char* result = evaluatePolynomial(x, precision);
    printf("f(%.1lf) = %s (精度: %d)\n", x, result, precision);

    // 释放内存
    free(aBinary);
    free(cDecimal);
    free(sum);
    free(difference);
    free(product);
    free(result);

    return 0;
}