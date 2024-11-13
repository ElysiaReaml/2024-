#include <stdio.h>
#include <stdlib.h>


// ���öѷ���洢�ķ�ʽʵ��
int *solution;  // ÿ��һ���ʺ�solution[col]=row,��ʾ��col�еĻʺ���row��
int len;        // �ʺ�������len*len������

// ��������
void initSolution();                 // ��ʼ��һ�������
void swap(int , int);
int evaluate();                      // ���㵱ǰ��ĳ�ͻ��
int evaluatevalue(int col, int row); // �����ƶ��ʺ��ĳ�ͻ�仯��
void traverseNeighbors();          // ��ɽ�㷨ʵ��

int main() {
    long seed = 820;  //��������ӣ������������ı䣬��ôÿ���㷨���еĽ������һ���ģ���ʹ��rand()����
	srand(seed);  //��һ��ע�͵�������ÿ�ε���������Ӷ���һ��

    printf("������ʺ����: ");
    scanf("%d", &len);

    // Ϊsolution��������ڴ�ռ�
    solution = (int *)malloc(len * sizeof(int));
    if (!solution)    return 0;// ����ڴ�����Ƿ�ɹ�

    initSolution();    // ��ʼ��һ�����̲���

    traverseNeighbors(); // ִ����ɽ�㷨

    // ������յõ��Ľ�
    printf("�õ��Ľ�Ϊ: ");
    for (int i = 0; i < len; ++i) {
        printf("%d ", solution[i]);  // ���ÿ�лʺ����ڵ���
    }
    printf("\n");

    free(solution);  // �ͷŶ�̬������ڴ�
    return 0;
}

/***  �����̵ĵ�i�к͵�j�н���  ***/
void swap(int i, int j){
	int temp = solution[i];
    solution[i] = solution[j];
    solution[j] = temp;
}

/***  ��ʼ��һ�����̲���,���ھ�����׼����  ***/
void initSolution(){
	// �������ʼ���ֻ����ĳ����ʼ����
	for(int i=0;i<len;++i)    
		solution[i]=i;
	for(int i = 0; i < 2*len; ++i)	// �����̽���2*len���������
		swap(rand()%len,rand()%len);
}

/***  �������̵����ۺ��������̵ļ�ֵ: value=���໥�������Ļʺ������
 ***  ���ʺ��໥��������ʱ��Ŀ�꺯����ֵvalue==0
 ***   solution[i]==solution[j], (solution[i]-solution[j])/(i-j)==1��-1ʱ��C����ʵ�ֻ���bug���������ʺ�˴��ܹ�������value++
 ***  ���̱��뷽ʽ�ܱ�֤������� solution[i]==solution[j]���ʸ��������Բ����ж�
 ***  ˼�����ú�����ʱ�������Ƿ�����ߣ�
 ***/
int evaluate() {
    int value = 0;  // ��ʼ����ͻ��

    // ����������������¼��ͻ���
    int *cols = (int *)calloc(len, sizeof(int));           // ��¼ÿ�г��ֻʺ�Ĵ���
    int *diag1 = (int *)calloc(2 * len - 1, sizeof(int));  // ���Խ��߳�ͻ
    int *diag2 = (int *)calloc(2 * len - 1, sizeof(int));  // ���Խ��߳�ͻ

    // ����ÿ�еĻʺ�ͳ�Ƹ�����ĳ�ͻ
    for (int i = 0; i < len; ++i) {
        int row = solution[i];
        cols[row]++;
        diag1[row + i]++;
        diag2[row - i + (len - 1)]++;
    }

    // �����ܳ�ͻ��
    for (int i = 0; i < len; ++i) {
        if (cols[i] > 1) value += cols[i] - 1;  // �г�ͻ
    }
    for (int i = 0; i < 2 * len - 1; ++i) {
        if (diag1[i] > 1) value += diag1[i] - 1;  // ���Խ��߳�ͻ
        if (diag2[i] > 1) value += diag2[i] - 1;  // ���Խ��߳�ͻ
    }

    // �ͷŶ�̬������ڴ�
    free(cols);
    free(diag1);
    free(diag2);

    return value;  // ���ص�ǰ��ĳ�ͻ��
}

/*** 
 * �����ƶ�ĳ�лʺ���ͻ���ı仯
 * @param col: ��Ҫ�ƶ�����
 * @param row: �µ���λ��
 * @return ��ͻ�仯������ֵ��ʾ��ͻ���٣�
 ***/
int evaluatevalue(int col, int newRow) {
    int oldRow = solution[col];
    int value = 0;

    int oldDiag1 = oldRow + col;
    int oldDiag2 = oldRow - col + (len - 1);

    int newDiag1 = newRow + col;
    int newDiag2 = newRow - col + (len - 1);

    // ����ɳ�ͻ
    for (int i = 0; i < len; ++i) {
        if (i != col) {
            if (solution[i] == oldRow) 
                value--; // ���г�ͻ����
            if (solution[i] + i == oldDiag1) 
                value--; // �����Խ��߳�ͻ����
            if (solution[i] - i + (len - 1) == oldDiag2) 
                value--; // �ɸ��Խ��߳�ͻ����
        }
    }

    // �����³�ͻ
    for (int i = 0; i < len; ++i) {
        if (i != col) {
            if (solution[i] == newRow) 
                value++; // ���г�ͻ����
            if (solution[i] + i == newDiag1) 
                value++; // �����Խ��߳�ͻ����
            if (solution[i] - i + (len - 1) == newDiag2) 
                value++; // �¸��Խ��߳�ͻ����
        }
    }

    //printf("��ͻ��Ϊ��%d\n",value);
    return value;
}

/*** 
 * ��ɽ�㷨ʵ��
 * ����ҵ����Ž⣬�����״̬���£�����ֹͣ������
 ***/
void traverseNeighbors() {
    int value = evaluate();  // ��ǰ��ĳ�ͻ��
    int found = 1;  // ����Ƿ��ҵ����Ž�

    // �����г�ͻ���ҵ����Ž�ʱ��������ɽ
    while (value > 0 && found) {
        found = 0;  // ��ʼ��Ϊδ�ҵ����Ž�

        // ���������У������ƶ�ÿ�еĻʺ�
        for (int col = 0; col < len; ++col) {
            int originalRow = solution[col];  // ��¼��ǰ��

            // ���Խ��ʺ��ƶ�����ͬ����
            for (int row = 0; row < len; ++row) {
                if (row != originalRow) {  // �����뵱ǰ���ظ�
                    int value = evaluatevalue(col, row);  // ��������
                    // ����ҵ����Ž⣨��ͻ���٣�
                    if (value <0) {
                        solution[col] = row;  // �ƶ��ʺ�
                        value += value;  // ���µ�ǰ��ͻ��
                        found = 1;  // ���Ϊ�ҵ����Ž�
                        break;  // ֹͣ��ǰ�еĳ���
                    }
                }
            }
        }
    }
}

/***  �������������ھӣ�Ҳû���ҵ����õ��ھ�
 ***  ��ô�������solution��Ȼ��������ɽ������
 ***  ����������ھ��������ڵġ�΢С�����Ŷ��������Ŷ���Χ�������������times=20��
 ***/
void restart(){
	int times = 20;  // ���������Ĵ������Ŷ���С,����times>1����
	for(int i = 0; i < times; ++i)
		swap(rand()%len,rand()%len);
	puts("restart()�����ã�");
}