#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_LEN 1000 // 定义最大长度

// 函数声明
void decimalToBinary(char *decimal, char *binary);
void binaryToDecimal(char *binary, char *decimal);
void decimalToNBase(char *decimal, char *nBaseResult, int n);
void add(char *num1, char *num2, char *result);
void subtract(char *num1, char *num2, char *result);
void multiply(char *num1, char *num2, char *result);
void evaluatePolynomial(double x, int n, double *result);
void reverseString(char *str);
void removeLeadingZeros(char *str);
int findDecimalPoint(char *str);
void removeTrailingZeros(char *str);
void alignDecimalPoint(char *num1, char *num2);
void addDecimal(char *num1, char *num2, char *result);
void subtractDecimal(char *num1, char *num2, char *result);
void multiplyDecimal(char *num1, char *num2, char *result);

int main() {
    char a[MAX_LEN], b[MAX_LEN], c[MAX_LEN];
    char binaryA[MAX_LEN * 32], decimalC[MAX_LEN];
    char sum[MAX_LEN * 2], diff[MAX_LEN * 2], product[MAX_LEN * 2];
    int n;

    printf("请输入第一个高精度十进制小数 a: ");
    scanf("%s", a);
    printf("请输入第二个高精度十进制小数 b: ");
    scanf("%s", b);
    printf("请输入一个高精度二进制小数 c: ");
    scanf("%s", c);
    printf("请输入要转换的进制 n (n < 20): ");
    scanf("%d", &n);

    // 十进制小数转换为二进制小数
    decimalToBinary(a, binaryA);
    printf("a 的二进制表示为: %s\n", binaryA);

    // 二进制小数转换为十进制小数
    binaryToDecimal(c, decimalC);
    printf("c 的十进制表示为: %s\n", decimalC);

    // 高精度加法
    addDecimal(a, b, sum);
    printf("a + b 的十进制结果为: %s\n", sum);
    decimalToNBase(sum, sum, n);
    printf("a + b 的 %d 进制结果为: %s\n", n, sum);

    // 高精度减法
    subtractDecimal(a, b, diff);
    printf("a - b 的十进制结果为: %s\n", diff);
    decimalToNBase(diff, diff, n);
    printf("a - b 的 %d 进制结果为: %s\n", n, diff);

    // 高精度乘法
    multiplyDecimal(a, b, product);
    printf("a * b 的十进制结果为: %s\n", product);
    decimalToNBase(product, product, n);
    printf("a * b 的 %d 进制结果为: %s\n", n, product);

    // 多项式求值
    double result;
    evaluatePolynomial(1.4, 200, &result);
    printf("f(1.4) = %f\n", result);

    return 0;
}

// 十进制小数转换为二进制小数
void decimalToBinary(char *decimal, char *binary) {
    char *intPart = strtok(decimal, ".");
    char *fracPart = strtok(NULL, ".");

    // 整数部分转换
    int intVal = atoi(intPart);//字符转整数类型
    int i = 0;
    while (intVal > 0) {
        binary[i++] = (intVal % 2) + '0';
        intVal /= 2;
    }
    if (i == 0) {
        binary[i++] = '0';
    }
    binary[i] = '\0';
    // 反转字符串
    int len = strlen(binary);
    for (int j = 0; j < len / 2; j++) {
        char temp = binary[j];
        binary[j] = binary[len - j - 1];
        binary[len - j - 1] = temp;
    }

    // 小数部分转换
    if (fracPart != NULL) {
        strcat(binary, ".");
        double fracVal = atof("0.");
        strcat(binary, fracPart);
        int count = 0;
        while (fracVal > 0 && count < MAX_LEN * 32) {
            fracVal *= 2;
            if (fracVal >= 1) {
                strcat(binary, "1");
                fracVal -= 1;
            } else {
                strcat(binary, "0");
            }
            count++;
        }
    }
}

// 二进制小数转换为十进制小数
void binaryToDecimal(char *binary, char *decimal) {
    char *intPart = strtok(binary, ".");
    char *fracPart = strtok(NULL, ".");

    // 整数部分转换
    int intVal = 0;
    int len = strlen(intPart);
    for (int i = 0; i < len; i++) {
        intVal += (intPart[len - i - 1] - '0') * pow(2, i);
    }
    sprintf(decimal, "%d", intVal);

    // 小数部分转换
    if (fracPart != NULL) {
        strcat(decimal, ".");
        double fracVal = 0;
        len = strlen(fracPart);
        for (int i = 0; i < len; i++) {
            fracVal += (fracPart[i] - '0') * pow(2, -(i + 1));
        }
        char fracStr[MAX_LEN];
        sprintf(fracStr, "%f", fracVal);
        strcat(decimal, fracStr + 2); // 跳过 "0."
    }
}


// 十进制转换为 N 进制
void decimalToNBase(char *decimal, char *nBaseResult, int n) {
    char *intPart = strtok(decimal, ".");
    char *fracPart = strtok(NULL, ".");

    // 整数部分转换
    int intVal = atoi(intPart);
    int i = 0;
    if (intVal == 0) {
        nBaseResult[i++] = '0';
    } else {
        while (intVal > 0) {
            int remainder = intVal % n;
            if (remainder < 10) {
                nBaseResult[i++] = remainder + '0';
            } else {
                nBaseResult[i++] = remainder - 10 + 'A'; // 使用 A-F 表示 10-15
            }
            intVal /= n;
        }
    }
    nBaseResult[i] = '\0';
    // 反转字符串
    int len = strlen(nBaseResult);
    for (int j = 0; j < len / 2; j++) {
        char temp = nBaseResult[j];
        nBaseResult[j] = nBaseResult[len - j - 1];
        nBaseResult[len - j - 1] = temp;
    }

    // 小数部分转换
    if (fracPart != NULL) {
        strcat(nBaseResult, ".");
        double fracVal = atof("0.");
        strcat(nBaseResult, fracPart);
        int count = 0;
        while (fracVal > 0 && count < MAX_LEN) {
            fracVal *= n;
            int digit = (int) fracVal;
            if (digit < 10) {
                strncat(nBaseResult, &digit + '0', 1);
            } else {
                char ch = digit - 10 + 'A';
                strncat(nBaseResult, &ch, 1);
            }
            fracVal -= digit;
            count++;
        }
    }
}



// 字符串反转
void reverseString(char *str) {
    int len = strlen(str);
    for (int i = 0; i < len / 2; i++) {
        char temp = str[i];
        str[i] = str[len - i - 1];
        str[len - i - 1] = temp;
    }
}

// 移除前导零
void removeLeadingZeros(char *str) {
    int len = strlen(str);
    int i = 0;
    while (i < len - 1 && str[i] == '0') {
        i++;
    }
    memmove(str, str + i, len - i + 1);
}

// 查找小数点的位置，如果没有，返回-1
int findDecimalPoint(char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] == '.') {
            return i;
        }
    }
    return -1;
}

// 移除前导零和尾随零（针对小数）
void removeTrailingZeros(char *str) {
    int len = strlen(str);
    int i = len - 1;
    while (i >= 0 && str[i] == '0') {
        str[i] = '\0';
        i--;
    }
    if (i >= 0 && str[i] == '.') {
        str[i] = '\0';
    }
}

// 对齐两个字符串的小数点，并在前面补零
void alignDecimalPoint(char *num1, char *num2) {
    int decimal1 = findDecimalPoint(num1);
    int decimal2 = findDecimalPoint(num2);

    // 如果其中一个数没有小数点，则在其末尾添加 ".0"
    if (decimal1 == -1) {
        strcat(num1, ".0");
        decimal1 = strlen(num1) - 2;
    }
    if (decimal2 == -1) {
        strcat(num2, ".0");
        decimal2 = strlen(num2) - 2;
    }

    // 计算小数部分的长度
    int fracLen1 = strlen(num1) - decimal1 - 1;
    int fracLen2 = strlen(num2) - decimal2 - 1;

    // 在较短的小数前面补零，使得两个数的小数部分长度一致
    if (fracLen1 < fracLen2) {
        int diff = fracLen2 - fracLen1;
        char temp[MAX_LEN];
        sprintf(temp, "%.0lf", pow(10, diff));
        strcat(temp, num1);
        strcpy(num1, temp);
    } else if (fracLen2 < fracLen1) {
        int diff = fracLen1 - fracLen2;
        char temp[MAX_LEN];
        sprintf(temp, "%.0lf", pow(10, diff));
        strcat(temp, num2);
        strcpy(num2, temp);
    }
}


// 高精度加法
void add(char *num1, char *num2, char *result) {
    int len1 = strlen(num1);
    int len2 = strlen(num2);
    int maxLen = (len1 > len2) ? len1 : len2;

    // 反转字符串以便从低位到高位相加
    reverseString(num1);
    reverseString(num2);

    int carry = 0;
    for (int i = 0; i < maxLen; i++) {
        int digit1 = (i < len1) ? num1[i] - '0' : 0;
        int digit2 = (i < len2) ? num2[i] - '0' : 0;

        int sum = digit1 + digit2 + carry;
        carry = sum / 10;
        result[i] = (sum % 10) + '0';
    }

    // 处理最高位的进位
    if (carry > 0) {
        result[maxLen] = carry + '0';
        result[maxLen + 1] = '\0';
    } else {
        result[maxLen] = '\0';
    }

    // 反转结果字符串以获得正确顺序
    reverseString(result);

    // 移除前导零
    removeLeadingZeros(result);
}

// 高精度减法 (假设 num1 >= num2)
void subtract(char *num1, char *num2, char *result) {
    int len1 = strlen(num1);
    int len2 = strlen(num2);

    // 反转字符串以便从低位到高位相减
    reverseString(num1);
    reverseString(num2);

    int borrow = 0;
    for (int i = 0; i < len1; i++) {
        int digit1 = num1[i] - '0';
        int digit2 = (i < len2) ? num2[i] - '0' : 0;

        int diff = digit1 - digit2 - borrow;
        if (diff < 0) {
            diff += 10;
            borrow = 1;
        } else {
            borrow = 0;
        }

        result[i] = diff + '0';
    }

    result[len1] = '\0';

    // 反转结果字符串以获得正确顺序
    reverseString(result);

    // 移除前导零
    removeLeadingZeros(result);
}

// 高精度乘法
void multiply(char *num1, char *num2, char *result) {
    int len1 = strlen(num1);
    int len2 = strlen(num2);
    int len = len1 + len2;

    // 初始化 result 数组
    for (int i = 0; i < len; i++) {
        result[i] = '0';
    }
    result[len] = '\0';

    // 从低位到高位进行乘法运算
    for (int i = len2 - 1; i >= 0; i--) {
        int carry = 0;
        for (int j = len1 - 1; j >= 0; j--) {
            int product = (num1[j] - '0') * (num2[i] - '0') + (result[i + j + 1] - '0') + carry;
            carry = product / 10;
            result[i + j + 1] = (product % 10) + '0';
        }
        result[i] += carry;
    }

    // 移除前导零
    removeLeadingZeros(result);
}


// 高精度小数加法
void addDecimal(char *num1, char *num2, char *result) {
    // 对齐小数点
    alignDecimalPoint(num1, num2);

    // 调用整数加法函数进行计算
    add(num1, num2, result); 

    // 在结果字符串中添加小数点
    int decimalPos = findDecimalPoint(result);
    if (decimalPos == -1) {
        strcat(result, ".0"); // 如果结果是整数，则添加 ".0"
    } 
}

// 高精度小数减法 (假设 num1 >= num2)
void subtractDecimal(char *num1, char *num2, char *result) {
    // 对齐小数点
    alignDecimalPoint(num1, num2);

    // 调用整数减法函数进行计算
    subtract(num1, num2, result); 

    // 在结果字符串中添加小数点
    int decimalPos = findDecimalPoint(result);
    if (decimalPos == -1) {
        strcat(result, ".0"); // 如果结果是整数，则添加 ".0"
    } 
}

// 高精度小数乘法
void multiplyDecimal(char *num1, char *num2, char *result) {
    // 查找小数点位置
    int decimal1 = findDecimalPoint(num1);
    int decimal2 = findDecimalPoint(num2);
    int fracLen1 = (decimal1 == -1) ? 0 : strlen(num1) - decimal1 - 1;
    int fracLen2 = (decimal2 == -1) ? 0 : strlen(num2) - decimal2 - 1;

    // 移除小数点，转换为整数进行乘法
    if (decimal1 != -1) {
        num1[decimal1] = '\0';
    }
    if (decimal2 != -1) {
        num2[decimal2] = '\0';
    }

    // 调用整数乘法函数进行计算
    multiply(num1, num2, result); 

    // 在结果中添加小数点
    int resultLen = strlen(result);
    int decimalPos = resultLen - (fracLen1 + fracLen2);
    if (decimalPos <= 0) {
        // 处理小数点位于结果字符串开头的情况
        char temp[MAX_LEN];
        sprintf(temp, "0.%0*d%s", -decimalPos, 0, result);
        strcpy(result, temp);
    } else {
        memmove(result + decimalPos + 1, result + decimalPos, resultLen - decimalPos);
        result[decimalPos] = '.';
    }

    // 移除尾随零
    removeTrailingZeros(result);
}

// 单变量多项式求值
void evaluatePolynomial(double x, int n, double *result) {
    // 初始化结果
    *result = 0.0;

    // 计算多项式的每一项并累加到结果中
    for (int i = 0; i <= n; i++) {
        double term = pow(x, i); // 计算 x 的 i 次方
        // 根据 i 的值计算系数
        if (i == 0) {
            term *= 4;
        } else if (i == 1) {
            term *= -1.0 / 3.0;
        } else if (i == 2) {
            term *= 0;
        } else if (i == 3) {
            term *= 3.0 / 7.0;
        } else {
            term = 0; // 其他项系数为 0
        }
        *result += term; // 累加当前项的值
    }
}