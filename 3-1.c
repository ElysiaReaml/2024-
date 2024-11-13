#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// 采用堆分配存储的方式实现
int *solution; // 每列一个皇后，solution[col]=row，表示第col列的皇后在row行
int len;       // 皇后数量，len*len的棋盘

// 函数声明
void initSolution();
void restart();
void swap(int, int);
int traverseNeighbors();
int evaluate();
void randShuffle();

int main() {
    long seed = time(NULL);  // 使用当前时间作为随机数种子
    srand(seed);  // 保证每次运行的结果不同

    len = 200;  // 棋盘大小
    solution = (int *)malloc(sizeof(int) * len);
    if (!solution) return 0;  // 分配空间失败

    initSolution();  // 初始化棋盘布局

    int flag;
    while ((flag = traverseNeighbors()) != 0)
        if (flag < 0) restart();  // 找不到更好的解时重启

    printf("冲突数为：%d\n得到的解为：", evaluate());
    for (int i = 0; i < len; ++i)
        printf("%d,", solution[i]);
    
    free(solution);
    return 0;
}

/*** 将棋盘的第i列和第j列交换 ***/
void swap(int i, int j) {
    int temp = solution[i];
    solution[i] = solution[j];
    solution[j] = temp;
}

/*** 初始化棋盘布局，随机交换2*len次 ***/
void initSolution() {
    for (int i = 0; i < len; ++i)
        solution[i] = i;
    for (int i = 0; i < 2 * len; ++i)
        swap(rand() % len, rand() % len);
}

/*** 计算当前棋盘的冲突皇后对数 ***/
int evaluate() {
    int value = 0;
    for (int i = 0; i < len - 1; ++i) {
        for (int j = i + 1; j < len; ++j) {
            // 判断是否在同一对角线上
            if (abs(solution[i] - solution[j]) == abs(i - j))
                value++;
        }
    }
    return value;
}

/*** 随机置换数组（randShuffle） ***/
void randShuffle() {
    for (int i = len - 1; i > 0; --i) {
        int j = rand() % (i + 1);
        swap(i, j);
    }
}

/*** 遍历邻居，找到冲突最小的棋盘 ***/
int traverseNeighbors() {
    int min_value = evaluate();  // 当前棋盘的冲突数
    int min_col = -1, min_row = -1;

    // 遍历所有可能的交换情况
    for (int i = 0; i < len - 1; ++i) {
        for (int j = i + 1; j < len; ++j) {
            swap(i, j);  // 交换两个皇后的位置
            int new_value = evaluate();  // 计算新的冲突数
            if (new_value < min_value) {
                min_value = new_value;
                min_col = i;
                min_row = j;
            }
            swap(i, j);  // 恢复原来的交换
        }
    }

    if (min_col == -1) {
        printf("找不到解，正在重新生成初始解并运行搜索算法中...\n");
        return -1;
    }
    swap(min_col, min_row);  // 采用更优的解
    return min_value;
}

/*** 随机重启，增加扰动 ***/
void restart() {
    int times = 20;  // 随机交换解的次数
    for (int i = 0; i < times; ++i)
        swap(rand() % len, rand() % len);
    puts("restart()被调用！");
}
