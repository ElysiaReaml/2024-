#include <stdio.h>
#include <stdlib.h>


// 采用堆分配存储的方式实现
int *solution;  // 每列一个皇后，solution[col]=row,表示第col列的皇后在row行
int len;        // 皇后数量，len*len的棋盘

// 函数声明
void initSolution();                 // 初始化一个随机解
void swap(int , int);
int evaluate();                      // 计算当前解的冲突数
int evaluatevalue(int col, int row); // 计算移动皇后后的冲突变化量
void traverseNeighbors();          // 爬山算法实现

int main() {
    long seed = 820;  //随机数种子，如果这个数不改变，那么每次算法运行的结果都是一样的，即使用rand()函数
	srand(seed);  //这一句注释掉，等于每次的随机数种子都不一样

    printf("请输入皇后个数: ");
    scanf("%d", &len);

    // 为solution数组分配内存空间
    solution = (int *)malloc(len * sizeof(int));
    if (!solution)    return 0;// 检查内存分配是否成功

    initSolution();    // 初始化一个棋盘布局

    traverseNeighbors(); // 执行爬山算法

    // 输出最终得到的解
    printf("得到的解为: ");
    for (int i = 0; i < len; ++i) {
        printf("%d ", solution[i]);  // 输出每列皇后所在的行
    }
    printf("\n");

    free(solution);  // 释放动态分配的内存
    return 0;
}

/***  将棋盘的第i列和第j列交换  ***/
void swap(int i, int j){
	int temp = solution[i];
    solution[i] = solution[j];
    solution[j] = temp;
}

/***  初始化一个棋盘布局,将邻居数组准备好  ***/
void initSolution(){
	// 随机给初始布局或给定某个初始布局
	for(int i=0;i<len;++i)    
		solution[i]=i;
	for(int i = 0; i < 2*len; ++i)	// 对棋盘进行2*len次随机交换
		swap(rand()%len,rand()%len);
}

/***  计算棋盘的评价函数（棋盘的价值: value=可相互攻击到的皇后对数）
 ***  当皇后相互攻击不到时，目标函数的值value==0
 ***   solution[i]==solution[j], (solution[i]-solution[j])/(i-j)==1或-1时（C语言实现会有bug），两个皇后彼此能攻击到，value++
 ***  棋盘编码方式能保证不会出现 solution[i]==solution[j]，故该条件可以不用判断
 ***  思考：该函数的时间性能是否能提高？
 ***/
int evaluate() {
    int value = 0;  // 初始化冲突数

    // 创建三个数组来记录冲突情况
    int *cols = (int *)calloc(len, sizeof(int));           // 记录每行出现皇后的次数
    int *diag1 = (int *)calloc(2 * len - 1, sizeof(int));  // 主对角线冲突
    int *diag2 = (int *)calloc(2 * len - 1, sizeof(int));  // 副对角线冲突

    // 遍历每列的皇后，统计各方向的冲突
    for (int i = 0; i < len; ++i) {
        int row = solution[i];
        cols[row]++;
        diag1[row + i]++;
        diag2[row - i + (len - 1)]++;
    }

    // 计算总冲突数
    for (int i = 0; i < len; ++i) {
        if (cols[i] > 1) value += cols[i] - 1;  // 行冲突
    }
    for (int i = 0; i < 2 * len - 1; ++i) {
        if (diag1[i] > 1) value += diag1[i] - 1;  // 主对角线冲突
        if (diag2[i] > 1) value += diag2[i] - 1;  // 副对角线冲突
    }

    // 释放动态分配的内存
    free(cols);
    free(diag1);
    free(diag2);

    return value;  // 返回当前解的冲突数
}

/*** 
 * 计算移动某列皇后后冲突数的变化
 * @param col: 需要移动的列
 * @param row: 新的行位置
 * @return 冲突变化量（负值表示冲突减少）
 ***/
int evaluatevalue(int col, int newRow) {
    int oldRow = solution[col];
    int value = 0;

    int oldDiag1 = oldRow + col;
    int oldDiag2 = oldRow - col + (len - 1);

    int newDiag1 = newRow + col;
    int newDiag2 = newRow - col + (len - 1);

    // 计算旧冲突
    for (int i = 0; i < len; ++i) {
        if (i != col) {
            if (solution[i] == oldRow) 
                value--; // 旧行冲突减少
            if (solution[i] + i == oldDiag1) 
                value--; // 旧主对角线冲突减少
            if (solution[i] - i + (len - 1) == oldDiag2) 
                value--; // 旧副对角线冲突减少
        }
    }

    // 计算新冲突
    for (int i = 0; i < len; ++i) {
        if (i != col) {
            if (solution[i] == newRow) 
                value++; // 新行冲突增加
            if (solution[i] + i == newDiag1) 
                value++; // 新主对角线冲突增加
            if (solution[i] - i + (len - 1) == newDiag2) 
                value++; // 新副对角线冲突增加
        }
    }

    //printf("冲突数为：%d\n",value);
    return value;
}

/*** 
 * 爬山算法实现
 * 如果找到更优解，则进行状态更新；否则停止搜索。
 ***/
void traverseNeighbors() {
    int value = evaluate();  // 当前解的冲突数
    int found = 1;  // 标记是否找到更优解

    // 当还有冲突且找到更优解时，继续爬山
    while (value > 0 && found) {
        found = 0;  // 初始化为未找到更优解

        // 遍历所有列，尝试移动每列的皇后
        for (int col = 0; col < len; ++col) {
            int originalRow = solution[col];  // 记录当前行

            // 尝试将皇后移动到不同的行
            for (int row = 0; row < len; ++row) {
                if (row != originalRow) {  // 避免与当前行重复
                    int value = evaluatevalue(col, row);  // 计算增量
                    // 如果找到更优解（冲突减少）
                    if (value <0) {
                        solution[col] = row;  // 移动皇后
                        value += value;  // 更新当前冲突数
                        found = 1;  // 标记为找到更优解
                        break;  // 停止当前列的尝试
                    }
                }
            }
        }
    }
}

/***  当遍历完所有邻居，也没有找到更好的邻居
 ***  那么随机重置solution，然后再用爬山法搜索
 ***  随机重启：邻居是邻域内的“微小”的扰动，增加扰动范围，比如随机交换times=20次
 ***/
void restart(){
	int times = 20;  // 随机交换解的次数，扰动大小,设置times>1即可
	for(int i = 0; i < times; ++i)
		swap(rand()%len,rand()%len);
	puts("restart()被调用！");
}