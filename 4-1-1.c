#include <stdio.h>
#include <string.h>  
#include <stdlib.h>  

#define MAX_SIZE 100  // 定义字符串的最大长度

// 初始化字符串，将 initValue 的内容复制到 str 中
void initString(char* str, const char* initValue) {
    if (!str) {  // 检查 str 是否为空
        fprintf(stderr, "错误：初始值无效。\n");
        return;
    }
    strncpy(str, initValue, MAX_SIZE - 1);  // 复制 initValue 内容到 str
    str[MAX_SIZE - 1] = '\0';  // 确保字符串以空字符结尾
}

// 清空字符串，将第一个字符设为 '\0'
void clearString(char* str) {
    if (!str) {  // 检查 str 是否为空
        fprintf(stderr, "错误：内存分配失败\n");
        return;
    }
    str[0] = '\0';  // 清空字符串
}

// 获取字符串长度的函数
int getStringLength(const char* str) {
    if (!str) {  // 检查 str 是否为空
        fprintf(stderr, "错误：字符串指针无效。\n");
        return -1;
    }
    return strlen(str);  
}

// 构建 next 数组，用于 KMP 算法的模式匹配
void buildNextArray(const char* pattern, int* next) {
    int patternLength = getStringLength(pattern);
    next[0] = -1;  // 初始化第一个元素为 -1
    int i = 0, j = -1;

    while (i < patternLength) {
        if (j == -1 || pattern[i] == pattern[j]) {
            i++;
            j++;
            next[i] = j;  // 更新 next 数组的值
        } else {
            j = next[j];  // 回溯 j 值
        }
    }
}

// KMP 算法查找所有匹配位置
// 返回所有匹配的位置并通过 `count` 传出匹配的个数
int* kmpSearchAll(const char* text, const char* pattern, int* matchCount) {
    int textLength = getStringLength(text);
    int patternLength = getStringLength(pattern);
    int* positions = (int*)malloc(sizeof(int) * MAX_SIZE);  // 动态分配位置数组
    *matchCount = 0;  // 匹配次数初始化为 0

    if (patternLength == 0) return positions;  // 若模式为空，直接返回

    int* next = (int*)malloc(sizeof(int) * patternLength);  // 为 next 数组分配空间
    buildNextArray(pattern, next);

    int i = 0, j = 0;
    while (i < textLength) {
        if (j == -1 || text[i] == pattern[j]) {
            i++;
            j++;
        } else {
            j = next[j];
        }
        if (j == patternLength) {  // 匹配成功
            positions[(*matchCount)++] = i - j;  // 记录匹配的位置
            j = next[j];  // 继续查找下一个匹配
        }
    }

    free(next);  // 释放 next 数组内存
    return positions;
}

// 获取字符串的子串，从 start 位置开始，长度为 length
int getSubstring(const char* str, int start, int length, char* subStr) {
    int strLength = getStringLength(str);
    if (start < 0 || start >= strLength || length <= 0 || start + length > strLength) {
        fprintf(stderr, "Error: Invalid start position or length.\n");
        subStr[0] = '\0';
        return 0;  // 返回失败
    }
    strncpy(subStr, str + start, length);  // 复制子串内容
    subStr[length] = '\0';  // 确保子串以空字符结尾
    return 1;  // 返回成功
}

// 字符串替换函数，使用 KMP 查找并替换所有匹配的子串
int replaceAll(char* S, const char* T, const char* V) {
    int SLength = getStringLength(S);
    int TLength = getStringLength(T);
    int VLength = getStringLength(V);
    int matchCount;
    int* positions = kmpSearchAll(S, T, &matchCount);  // 获取所有匹配位置

    if (matchCount == 0) {  // 若没有匹配，直接返回
        printf("Pattern '%s' not found in the string '%s'.\n", T, S);
        free(positions);
        return 0;
    }

    int newLength = SLength + matchCount * (VLength - TLength);  // 计算新字符串的长度
    if (newLength >= MAX_SIZE) {
        fprintf(stderr, "Error: Resulting string length exceeds maximum allowed size.\n");
        free(positions);
        return 0;
    }

    char* newString = (char*)malloc(newLength + 1);  // 为新字符串分配内存
    int newIndex = 0;
    int oldIndex = 0;

    for (int i = 0; i < matchCount; ++i) {
        while (oldIndex < positions[i]) {
            newString[newIndex++] = S[oldIndex++];
        }
        for (int j = 0; j < VLength; ++j) {
            newString[newIndex++] = V[j];
        }
        oldIndex += TLength;  // 跳过 T 的匹配部分
    }
    while (oldIndex < SLength) {
        newString[newIndex++] = S[oldIndex++];
    }
    newString[newIndex] = '\0';  // 确保新字符串以空字符结尾
    strncpy(S, newString, newLength + 1);  // 复制新字符串回 S

    free(newString);
    free(positions);
    return 1;
}

// 拼接两个字符串
int concatenateStrings(char* S1, const char* S2) {
    int S1Length = getStringLength(S1);
    int S2Length = getStringLength(S2);

    if (S1Length + S2Length >= MAX_SIZE) {
        fprintf(stderr, "Error: Resulting string length exceeds maximum allowed size.\n");
        return 0;
    }
    strncpy(S1 + S1Length, S2, MAX_SIZE - S1Length - 1);
    S1[S1Length + S2Length] = '\0';
    return 1;
}

// 遍历并输出字符串中的所有字符
void traverseString(const char* str) {
    if (!str) {
        fprintf(stderr, "Error: Invalid string pointer.\n");
        return;
    }
    printf("Traversing characters in the string:\n");
    for (int i = 0; str[i] != '\0'; ++i) {
        printf("%c\n", str[i]);
    }
}

int main() {
    char myString[MAX_SIZE];
    char string1[MAX_SIZE];
    char string2[MAX_SIZE];

    initString(string1, "Hello, ");
    initString(string2, "HeFei!");
    initString(myString, "Hello, World!");

    printf("Initialized String: %s\n", myString);

    // 获取字符串长度
    int length = getStringLength(myString);
    printf("String Length: %d\n", length);


    // KMP 查找子串匹配
    const char* pattern = "World";
    int matchCount;
    int* positions = kmpSearchAll(myString, pattern, &matchCount);
    if (matchCount > 0) {
        printf("Pattern '%s' found at positions: ", pattern);
        for (int i = 0; i < matchCount; ++i) {
            printf("%d ", positions[i]);
        }
        printf("\n");
    } else {
        printf("Pattern '%s' not found.\n", pattern);
    }
    free(positions);

    // 获取子串
    char subString[MAX_SIZE];
    if (getSubstring(myString, 7, 6, subString)) {
        printf("Substring: %s\n", subString);
    }

    // 替换示例
    const char* oldSubstring = "World";
    const char* newSubstring = "USTC";
    if (replaceAll(myString, oldSubstring, newSubstring)) {
        printf("String after replacement: %s\n", myString);
    }

    // 清空字符串和获取字符串长度
    clearString(myString);
    printf("Cleared String: %s\n", myString);

    length = getStringLength(myString);
    printf("Cleared String Length: %d\n", length);
    
    // 拼接示例
    if (concatenateStrings(string1, string2)) {
        printf("Concatenated String: %s\n", string1);
    }

    traverseString(string1);

    return 0;
}
