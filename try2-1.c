#include <stdio.h>
#include <stdlib.h>
#include <gmp.h> // 使用 GMP 库进行高精度计算

// 定义多项式求值的函数
void polynomial_evaluation(mpf_t result, mpf_t x) {
    // 初始化 GMP 浮点数变量
    mpf_t term1, term2, term3, constant, coefficient1, coefficient2;
    mpf_init2(term1, 200); // 初始化 term1，精度为 200 位
    mpf_init2(term2, 200); // 初始化 term2，精度为 200 位
    mpf_init2(term3, 200); // 初始化 term3，精度为 200 位
    mpf_init2(constant, 200); // 初始化常数项，精度为 200 位
    mpf_init2(coefficient1, 200); // 初始化系数1，精度为 200 位
    mpf_init2(coefficient2, 200); // 初始化系数2，精度为 200 位
    mpf_init2(result, 200); // 初始化结果，精度为 200 位

    // 设置系数和常数项
    mpf_set_d(coefficient1, 3.0/7.0); // 设置系数 3/7
    mpf_set_d(coefficient2, -1.0/3.0); // 设置系数 -1/3
    mpf_set_d(constant, 4.0); // 设置常数项 4

    // 计算 x^3
    mpf_pow_ui(term1, x, 3); // 计算 x 的三次方
    mpf_mul(term1, term1, coefficient1); // 乘以系数 3/7

    // 计算 x^2
    mpf_pow_ui(term2, x, 2); // 计算 x 的平方
    mpf_mul(term2, term2, coefficient2); // 乘以系数 -1/3


    // 计算最终结果
    mpf_add(result, term1, term2); // 将两项相加
    mpf_add(result, result, constant); // 加上常数项

    // 清理 GMP 浮点数变量
    mpf_clear(term1);
    mpf_clear(term2);
    mpf_clear(term3);
    mpf_clear(constant);
    mpf_clear(coefficient1);
    mpf_clear(coefficient2);

}


int main() {
    mpf_t x, result;
    mpf_init2(x, 200); // 初始化 x，精度为 200 位
    mpf_init2(result, 200); // 初始化 result，精度为 200 位


    mpf_set_d(x, 1.4); // 设置 x 的值为 1.4

    polynomial_evaluation(result, x); // 调用函数计算多项式的值

    gmp_printf("%.200Ff\n", result); // 打印结果，保留 200 位小数

    // 清理 GMP 浮点数变量
    mpf_clear(x);
    mpf_clear(result);

    return 0;
}
/*

void DecimalToNBase(char* number, int base, char* result) {//十进制转任意进制
    // 检查基数范围
    if (base < 2 || base > 20) {
        strcpy(result, "Invalid base");
        return;
    }

    int isNegative = 0;  // 处理负数
    if (number[0] == '-') {
        isNegative = 1;
        number++;  // 跳过负号
    }

    // 分割整数部分和小数部分
    char* point = strchr(number, '.');
    long long integerPart = 0;
    double fractionalPart = 0.0;

    if (point != NULL) {  // 存在小数点
        *point = '\0';  // 暂时切断整数部分和小数部分
        integerPart = atoll(number);  // 转换整数部分
        fractionalPart = atof(point + 1) / pow(10, strlen(point + 1));  // 转换小数部分
    } else {
        integerPart = atoll(number);  // 只有整数部分
    }

    char temp[200];  // 存储转换后的结果
    int idx = 0;

    // 整数部分的转换
    do {
        int remainder = integerPart % base;
        temp[idx++] = (remainder < 10) ? (remainder + '0') : (remainder - 10 + 'A');
        integerPart /= base;
    } while (integerPart != 0);

    // 如果是负数，加负号
    if (isNegative) {
        temp[idx++] = '-';
    }

    // 反转整数部分的结果
    for (int i = 0; i < idx / 2; i++) {
        char t = temp[i];
        temp[i] = temp[idx - 1 - i];
        temp[idx - 1 - i] = t;
    }
    temp[idx] = '\0';  // 整数部分结束

    strcpy(result, temp);  // 将整数部分拷贝到结果中

    // 如果有小数部分，则处理小数部分
    if (fractionalPart > 0) {
        strcat(result, ".");  // 添加小数点

        // 小数部分的转换，设定最大精度为 200 位
        for (int i = 0; i < 200 && fractionalPart > 0; i++) {
            fractionalPart *= base;
            int digit = (int)fractionalPart;
            strcat(result, (digit < 10) ? (char[]){(char)(digit + '0'), '\0'} : (char[]){(char)(digit - 10 + 'A'), '\0'});
            fractionalPart -= digit;
        }
    }
}

*/