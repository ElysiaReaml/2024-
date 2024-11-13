#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 100  // 定长顺序存储最大长度

// 串的定长顺序存储表示
typedef struct {
    char data[MAX_SIZE];
    int length;
} SeqString;

// 串的堆分配存储表示
typedef struct {
    char *data;
    int length;
} HeapString;

// 定长顺序存储串的基本操作
void InitSeqString(SeqString *S) {
    S->length = 0;
    S->data[0] = '\0';
}

void DestroySeqString(SeqString *S) {
    S->length = 0;
    S->data[0] = '\0';
}

int LengthSeqString(SeqString S) {
    return S.length;
}

void ClearSeqString(SeqString *S) {
    S->length = 0;
    S->data[0] = '\0';
}

// 堆分配存储串的基本操作
void InitHeapString(HeapString *S, const char *str) {
    S->length = strlen(str);
    S->data = (char *)malloc((S->length + 1) * sizeof(char));
    if (S->data) {
        strcpy(S->data, str);
    }
}

void DestroyHeapString(HeapString *S) {
    free(S->data);
    S->data = NULL;
    S->length = 0;
}

int LengthHeapString(HeapString S) {
    return S.length;
}

void ClearHeapString(HeapString *S) {
    S->length = 0;
    S->data[0] = '\0';
}

// KMP模式匹配辅助函数：构建部分匹配表
void BuildNext(const char *pattern, int *next) {
    int i = 0, j = -1;
    next[0] = -1;
    int len = strlen(pattern);
    while (i < len - 1) {
        if (j == -1 || pattern[i] == pattern[j]) {
            i++; j++;
            next[i] = j;
        } else {
            j = next[j];
        }
    }
}

// KMP模式匹配算法
int KMP(const char *text, const char *pattern) {
    int i = 0, j = 0;
    int text_len = strlen(text);
    int pattern_len = strlen(pattern);
    int *next = (int *)malloc(pattern_len * sizeof(int));
    BuildNext(pattern, next);
    
    while (i < text_len && j < pattern_len) {
        if (j == -1 || text[i] == pattern[j]) {
            i++;
            j++;
        } else {
            j = next[j];
        }
    }
    free(next);
    if (j == pattern_len) {
        return i - j;  // 返回匹配的起始位置
    } else {
        return -1;  // 未匹配
    }
}

// 求子串
void SubString(HeapString *sub, HeapString S, int pos, int len) {
    if (pos < 0 || pos + len > S.length) {
        printf("Invalid position or length.\n");
        return;
    }
    sub->data = (char *)malloc((len + 1) * sizeof(char));
    strncpy(sub->data, S.data + pos, len);
    sub->data[len] = '\0';
    sub->length = len;
}

// 替换子串
void Replace(HeapString *S, const char *T, const char *V) {
    int pos = KMP(S->data, T);
    if (pos == -1) return;

    int T_len = strlen(T);
    int V_len = strlen(V);
    int new_len = S->length - T_len + V_len;

    char *new_data = (char *)malloc((new_len + 1) * sizeof(char));
    strncpy(new_data, S->data, pos);
    strcpy(new_data + pos, V);
    strcpy(new_data + pos + V_len, S->data + pos + T_len);

    free(S->data);
    S->data = new_data;
    S->length = new_len;
}

// 拼接操作
void Concat(HeapString *S1, HeapString S2) {
    int new_len = S1->length + S2.length;
    S1->data = (char *)realloc(S1->data, (new_len + 1) * sizeof(char));
    strcat(S1->data, S2.data);
    S1->length = new_len;
}

// 遍历输出
void TraverseString(HeapString S) {
    printf("%s\n", S.data);
}

// 主函数用于测试
int main() {
    // 测试堆分配存储
    HeapString S;
    InitHeapString(&S, "Hello World");
    printf("Original String: ");
    TraverseString(S);

    // 测试求长度
    printf("Length: %d\n", LengthHeapString(S));

    // 测试模式匹配
    int pos = KMP(S.data, "World");
    if (pos != -1) printf("Pattern 'World' found at position: %d\n", pos);

    // 测试求子串
    HeapString sub;
    SubString(&sub, S, 6, 5);
    printf("SubString: ");
    TraverseString(sub);
    DestroyHeapString(&sub);

    // 测试替换
    Replace(&S, "World", "C Language");
    printf("After Replacement: ");
    TraverseString(S);

    // 测试拼接
    HeapString S2;
    InitHeapString(&S2, " is fun!");
    Concat(&S, S2);
    printf("After Concat: ");
    TraverseString(S);

    // 销毁串
    DestroyHeapString(&S);
    DestroyHeapString(&S2);

    return 0;
}
