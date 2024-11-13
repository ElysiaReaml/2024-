#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 初始化字符串，将传入的初始值复制到动态分配的内存中
char* initString(const char* initValue) {
    if (initValue == NULL) {  // 检查初始值是否为空
        fprintf(stderr, "错误：初始值无效。\n");
        return NULL;
    }

    int length = strlen(initValue);  // 计算初始值长度
    char* str = (char*)malloc((length + 1) * sizeof(char));  // 动态分配内存
    if (str == NULL) {
        fprintf(stderr, "错误：内存分配失败\n");
        return NULL;
    }

    strcpy(str, initValue);  // 复制初始值到新分配内存中
    return str;
}

// 销毁字符串，释放动态分配的内存
void destroyString(char* str) {
    if (str != NULL) {
        free(str);
    }
}

// 清空字符串，将第一个字符设为 '\0' 表示空串
void clearString(char* str) {
    if (str != NULL) {
        str[0] = '\0';
    }
}

// 获取字符串长度
int getStringLength(const char* str) {
    if (str == NULL) {
        fprintf(stderr, "错误：字符串指针无效。\n");
        return -1;
    }
    return strlen(str);
}

// 构建 KMP 算法的部分匹配表（前缀表）
// 用于字符串匹配中记录每个字符的最长前后缀长度
void buildPrefixTable(const char* pattern, int* prefixTable, int patternLength) {
    int len = 0;  // 前缀的长度
    prefixTable[0] = 0;  // 第一个字符的前缀长度为0

    int i = 1;
    while (i < patternLength) {
        if (pattern[i] == pattern[len]) {
            len++;
            prefixTable[i] = len;
            i++;
        } else {
            if (len != 0) {
                len = prefixTable[len - 1];
            } else {
                prefixTable[i] = 0;
                i++;
            }
        }
    }
}

// KMP 算法查找所有匹配位置
// 返回所有匹配的位置并通过 `count` 传出匹配的个数
int* kmpSearch(const char* text, const char* pattern, int* count) {
    int textLength = getStringLength(text);
    int patternLength = getStringLength(pattern);

    if (textLength == -1 || patternLength == -1) {
        *count = 0;
        return NULL;
    }

    int* positions = (int*)malloc(textLength * sizeof(int));  // 动态数组存储匹配位置
    int* prefixTable = (int*)malloc(patternLength * sizeof(int));
    int posIndex = 0;  // 位置索引

    buildPrefixTable(pattern, prefixTable, patternLength);

    int i = 0, j = 0;
    while (i < textLength) {
        if (pattern[j] == text[i]) {  // 匹配字符
            i++;
            j++;
        }

        if (j == patternLength) {  // 完成匹配
            positions[posIndex++] = i - j;  // 记录匹配位置
            j = prefixTable[j - 1];
        } else if (i < textLength && pattern[j] != text[i]) {
            if (j != 0) {
                j = prefixTable[j - 1];
            } else {
                i++;
            }
        }
    }

    *count = posIndex;  // 返回匹配次数
    free(prefixTable);
    return positions;
}

// 获取字符串的子串，从 start 位置开始，长度为 length
char* getSubstring(const char* str, int start, int length) {
    int strLength = getStringLength(str);
    if (start < 0 || start >= strLength || length <= 0 || start + length > strLength) {
        fprintf(stderr, "Error: Invalid start position or length.\n");
        return NULL;
    }

    char* substring = (char*)malloc((length + 1) * sizeof(char));
    if (substring == NULL) {
        fprintf(stderr, "Error: Memory allocation failed.\n");
        return NULL;
    }

    strncpy(substring, str + start, length);  // 复制子串内容
    substring[length] = '\0';  // 添加终止符
    return substring;
}

// 字符串替换函数，使用 KMP 查找并替换所有匹配的子串
char* Replace(const char* S, const char* T, const char* V) {
    int SLength = getStringLength(S);
    int TLength = getStringLength(T);
    int VLength = getStringLength(V);

    if (SLength == -1 || TLength == -1 || VLength == -1) {
        return NULL;
    }

    int count;
    int* positions = kmpSearch(S, T, &count);  // 查找所有匹配位置
    int newLength = SLength + count * (VLength - TLength);  // 计算新字符串长度

    char* newStr = (char*)malloc((newLength + 1) * sizeof(char));
    if (newStr == NULL) {
        fprintf(stderr, "Error: Memory allocation failed.\n");
        free(positions);
        return NULL;
    }

    int i = 0, j = 0;
    for (int k = 0; k < count; k++) {
        int pos = positions[k];
        strncpy(newStr + j, S + i, pos - i);  // 复制当前匹配前的内容
        j += pos - i;
        i = pos;

        strncpy(newStr + j, V, VLength);  // 复制替换字符串
        j += VLength;
        i += TLength;
    }

    strcpy(newStr + j, S + i);  // 复制剩余字符
    free(positions);
    return newStr;
}

// 拼接两个字符串
char* concatStrings(const char* str1, const char* str2) {
    int len1 = getStringLength(str1);
    int len2 = getStringLength(str2);

    if (len1 == -1 || len2 == -1) {
        return NULL;
    }

    char* result = (char*)malloc((len1 + len2 + 1) * sizeof(char));  // 动态分配新内存
    if (result == NULL) {
        fprintf(stderr, "Error: Memory allocation failed.\n");
        return NULL;
    }

    strcpy(result, str1);  // 复制第一个字符串
    strcpy(result + len1, str2);  // 复制第二个字符串
    return result;
}

// 遍历字符串中的所有字符并输出
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
    // 初始化和遍历字符串
    char* myString = initString("Hello, World!");
    if (myString != NULL) {
        printf("Initialized string: %s\n", myString);
        traverseString(myString);
    }

    // 获取字符串长度和清空字符串
    printf("String length: %d\n", getStringLength(myString));
    clearString(myString);
    printf("Cleared string: %s\n", myString);
    printf("Cleared string length: %d\n", getStringLength(myString));

    destroyString(myString);  // 销毁字符串
    myString = NULL;

    // KMP 查找子串匹配
    char* text = initString("Hello,World!");
    char* pattern = initString("World");
    int count;
    int* positions = kmpSearch(text, pattern, &count);

    for (int i = 0; i < count; i++) {
        printf("Match position: %d\n", positions[i] + 1);
    }
    free(positions);

    // 获取子串
    char* substring = getSubstring(text, 3, 6);
    if (substring != NULL) {
        printf("Substring: %s\n", substring);
        destroyString(substring);
    }

    // 替换示例
    char* newText = Replace(text, "World", "USTC");
    if (newText != NULL) {
        printf("New text: %s\n", newText);
        destroyString(newText);
    }

    // 拼接示例
    char* str1 = initString("Hello, ");
    char* str2 = initString("HeFei!");
    char* concatenatedStr = concatStrings(str1, str2);
    if (concatenatedStr != NULL) {
        printf("Concatenated string: %s\n", concatenatedStr);
        destroyString(concatenatedStr);
    }

    destroyString(text);
    destroyString(pattern);
    destroyString(str1);
    destroyString(str2);

    return 0;
}
