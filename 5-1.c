#include <stdio.h>
#include <stdlib.h>

// 定义存放数据的类型
typedef struct {
    int id;     // 序号
    int value;  // 值
} ElemType;

// 定义二叉树的结点
typedef struct tnode {
    ElemType data;         // 该节点的值
    struct tnode *lchild;  // 左孩子
    struct tnode *rchild;  // 右孩子
} bNode;


// 递归算法(先序遍历)：构造二叉树
// 输入先序序列pre和中序序列in，以及两个序列各自的上界pu,iu和下界pl,il
bNode *buildBTreeByPI(int *pre, int pl, int pu, int *in, int il, int iu) {
    printf("\n%d-%d,%d-%d", pl, pu, il, iu); // 打印当前处理的子序列范围，用于调试

    if (pu - pl != iu - il) exit(0); // 检查输入参数，先序和中序序列长度必须一致
    if (pu - pl < 0) return NULL;  // 若序列为空，返回空结点/空树

    // 分配内存创建新节点
    bNode *t = (bNode *)malloc(sizeof(bNode));
    if (!t) exit(0); // 内存分配失败

    // 根节点的值为先序遍历的第一个元素
    t->data.value = pre[pl];
    t->data.id = pl; // 初始id，后续会重新赋值


    // 在中序遍历中找到根节点的位置，划分左右子树
    int i;
    for (i = il; i <= iu; ++i) {
        if (in[i] == pre[pl]) {
            // 计算左右子树在中序和先序遍历中的下标范围
            int il1 = il, iu1 = i - 1;
            int il2 = i + 1, iu2 = iu;
            int pl1 = pl + 1, pu1 = pl + (iu1 - il1 + 1);
            int pl2 = pu1 + 1, pu2 = pu;

            // 递归构建左右子树
            t->lchild = buildBTreeByPI(pre, pl1, pu1, in, il1, iu1);
            t->rchild = buildBTreeByPI(pre, pl2, pu2, in, il2, iu2);
            break; // 找到根节点后跳出循环
        }
    }
    return t;
}


// 层序遍历打印二叉树，并打印每个节点的值和id，并重新赋值id
void printTree(bNode *root) {
    if (root == NULL) {
        printf("空树\n");
        return;
    }

    // 使用队列实现层序遍历
    bNode *queue[100]; // 使用数组模拟队列，假设树的节点数不超过100
    int front = 0, rear = 0;
    queue[rear++] = root;

    int level = 0;
    int idCounter = 0; // id 计数器，从 0 开始


    while (front < rear) {
        int levelSize = rear - front; // 当前层级的节点数

        printf("Level %d: ", level);
        for (int i = 0; i < levelSize; ++i) {
            bNode *current = queue[front++];
            if (current) {
                current->data.id = idCounter++; // 重新赋值 id
                printf("(%d, %d) ", current->data.value, current->data.id);
                if (current->lchild) queue[rear++] = current->lchild; 
                else queue[rear++] = NULL; // 即使子节点为空，也要入队占位，以保持层级关系

                if (current->rchild) queue[rear++] = current->rchild;
                else queue[rear++] = NULL;
            } else {
                printf("N "); // 空节点
            }
        }
        printf("\n");
        level++;
    }
}





// 递归删除所有值为特定值的节点及其子树,并释放内存
bNode* deleteNodesWithValue(bNode* root, int value) {
    if (root == NULL) return NULL;

    // 先递归处理左右子树
    root->lchild = deleteNodesWithValue(root->lchild, value);
    root->rchild = deleteNodesWithValue(root->rchild, value);

    // 如果当前节点的值等于要删除的值，释放内存并返回NULL
    if (root->data.value == value) {
        free(root);
        return NULL;
    }

    // 否则返回当前节点
    return root;
}



// 辅助函数，用于在子树中查找具有特定id的节点
bNode* findInSubtree(bNode* root, int id) {
    if (root == NULL) return NULL;
    if (root->data.id == id) return root;
    bNode* left = findInSubtree(root->lchild, id);
    if (left) return left;
    return findInSubtree(root->rchild, id);
}



// 查找最近公共祖先 (LCA)
bNode* findLCA(bNode* root, int id1, int id2) {
    if (root == NULL) return NULL;


    // 如果id1和id2都在左子树或右子树中，递归查找
    if (findInSubtree(root->lchild, id1) && findInSubtree(root->lchild, id2)) {
        return findLCA(root->lchild, id1, id2);
    }
    if (findInSubtree(root->rchild, id1) && findInSubtree(root->rchild, id2)) {
        return findLCA(root->rchild, id1, id2);
    }


    // 如果一个在左子树，一个在右子树，或者当前节点就是其中一个，则当前节点为LCA
    if ((findInSubtree(root, id1) || root->data.id == id1) && (findInSubtree(root,id2) || root->data.id == id2 ))
        return root;


    return NULL; // 未找到
}



// 辅助函数，递归打印路径
bool printPathToNodeRecursive(bNode* root, int id, char* path, int pathLen) {
    if (root == NULL) return false;

    if (root->data.id == id) {
        path[pathLen] = '\0'; // 添加字符串终止符
        printf("%s\n", path);
        return true;
    }

    if (root->lchild) {
        path[pathLen] = '左';
        if (printPathToNodeRecursive(root->lchild, id, path, pathLen + 1)) return true;
    }

    if (root->rchild) {
        path[pathLen] = '右';
        if (printPathToNodeRecursive(root->rchild, id, path, pathLen + 1)) return true;
    }

    return false;
}

// 打印从根节点到特定id节点的路径
void printPathToNode(bNode* root, int id) {
     if (root == NULL) {
        printf("空树\n");
        return;
    }

    if (!findInSubtree(root, id)) {
        printf("id为%d未找到\n", id);
        return;
    }


    char path[100]; // 假设路径长度不超过100
    printPathToNodeRecursive(root, id, path, 0);

}





// 辅助函数，用于递归查找二叉树中的最大值和最小值
void findMinMaxValue(bNode* root, int* minValue, int* maxValue) {
    if (root == NULL) return;

    // 更新最大值和最小值. 使用指针以便修改原始值
    if (root->data.value < *minValue) *minValue = root->data.value;
    if (root->data.value > *maxValue) *maxValue = root->data.value;

    // 递归查找左右子树
    findMinMaxValue(root->lchild, minValue, maxValue);
    findMinMaxValue(root->rchild, minValue, maxValue);
}



// 计算二叉树节点中最大的value和最小的value之差
int calculateValueDifference(bNode* root) {
    if (root == NULL) return 0;

    int minValue = root->data.value;
    int maxValue = root->data.value;


    findMinMaxValue(root, &minValue, &maxValue); // 使用指针传递

    return maxValue - minValue;
}



int main() {
    // 先序遍历序列
    int pre[] = {23, 7, 13, 10, 8, 14, 17, 18, 33, 76, 42, 89};
    // 中序遍历序列
    int in[] = {7, 8, 10, 13, 14, 17, 18, 23, 33, 42, 76, 89};

    // 构建二叉树
    bNode *root = buildBTreeByPI(pre, 0, 11, in, 0, 11);
    printf("\n");


    printTree(root);

      // 查找id1和id2的最近共同祖先
    int id1 = 2;
    int id2 = 3;
    bNode* lca = findLCA(root, id1, id2);
    if (lca) {
        printf("%d 和 %d的最近共同祖先是 %d\n", id1, id2, lca->data.value);
    } else {
        printf("未找到最近共同祖先.\n");
    }
    
    
    int targetId = 0;
    printf("路径%d:\n", targetId);
    printPathToNode(root, targetId);
    

     // 计算二叉树节点中最大的value和最小的value之差
    int valueDifference = calculateValueDifference(root);
    printf("最大值最小值之差为: %d\n", valueDifference);


    // 删除值为17的节点及其子树
    int deleteValue = 17;
    root = deleteNodesWithValue(root, deleteValue);
    printf("删除节点%d后的二叉树:\n", deleteValue);
    printTree(root);



    return 0;
}