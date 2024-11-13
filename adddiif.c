#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
 
#define MAXSIZE 200
 
typedef struct Node{//双向循环链表结点定义
	char data;
	struct Node *next;
	struct Node *prior;
}Node,*LinkList;
 
LinkList InitList()//初始化链表
{
	LinkList L;
	L=(Node *)malloc(sizeof(Node));
	L->data=1;
	L->next=L;
	L->prior=L;
	return L;
}

int ListLength(LinkList L)//求表长
{
    LinkList p;
    int t=0;
    p=L->next;
    while(p!=L)
    {
        t++;
        p=p->next;
    }
    return t;
}
 
void CreateList_Head(LinkList L,char num)//头插法建表
{
	LinkList p,n;
	p=L;
	n=(Node *)malloc(sizeof(Node));
	n->data=num;
	n->next=p->next;
	n->prior=p;
	p->next=n;
	n->next->prior=n;
}
 
void CreateList_Tail(LinkList L,char num)//尾插法建表
{
	LinkList p,n;
	p=L;
	n=(Node *)malloc(sizeof(Node));
	n->data=num;
	n->next=p;
	n->prior=p->prior;
	p->prior->next=n;
	p->prior=n;
}
 
Node* create_node(char num)//创建双向链表结点
{
    Node* node = (Node*)malloc(sizeof(Node));//申请内存
    node->data = num;
    node->prior = NULL;
    node->next = NULL;
    return node;
}

void DestroyList(LinkList L)//销毁表
{
	LinkList p,q;
	p=L->next;
	while(p!=L)
	{
		q=p->next;
		free(p);
		p=q;
	}
	free(L);
}
 
void PrintList(LinkList L) //输出链表
{
	LinkList p;
	if(!L->data)		//负数前的负号
		printf("-");
	p=L->next;
    while(p!=L)
    {
        if(p->data=='.')		//小数点
			printf(".");
		else
		printf("%d",p->data);
        p=p->next;
    }
	printf("\n");
}
 
void Adjust(LinkList L) //清除多余的0和0前面的负号
{
    LinkList p=L->next;
    while(p->data==0&&ListLength(L)>1)//删除高位的0
    {
        p->next->prior=L;
        L->next=p->next;
        free(p);
        p=L->next;
    }
	if(L->next->data==0&&ListLength(L)==1)//删除0前面的负号
		L->data=1;
}
 
void GetNum(LinkList L,char num[])//将输入的数字（字符串）转换成链表
{
	long long i;
	for(i=0;i<strlen(num);i++)
	{
		if((num[i]>='0'&&num[i]<='9')||num[i]=='-'||num[i]=='.')	//判断输入是否合法
		{
			if(num[i]=='-')			//操作数为负数情况，负号不读入链表
			{
				L->data=0;			//链表头结点数据域表示正负（1为正，0为负）
				continue;
			}else if(num[i]=='.')		//小数点不读入链表
				continue;
			CreateList_Tail(L,num[i]-'0'); //采用尾插法（高位在前）
		}
		else
		{
			printf("输入错误！\n");
			exit(0);
		}
	}
}

void PrintResult(LinkList add_result,LinkList min_result,LinkList mul_result)//打印结果
{
	printf("两数之和为： ");
	PrintList(add_result);
	printf("两数之差为： ");
	PrintList(min_result);
	printf("两数之积为： ");
	PrintList(mul_result);
}
 
LinkList Add(LinkList L1,LinkList L2)//加法实现
{
    LinkList Minus(LinkList,LinkList);
	LinkList p=L1->prior,q=L2->prior;//从链表尾端（低位）开始
	LinkList L3;
	int carry=0,sum;
	if((L1->data==1&&L2->data==1)||(L1->data==0&&L2->data==0))//当两数均为或均为负时直接相加
	{
		L3=InitList();
		while(p!=L1&&q!=L2)
		{
			sum=p->data+q->data+carry;
			CreateList_Head(L3,sum%10);  //采用头插法（保证高位在前）
			carry=sum/10;
			p=p->prior;
			q=q->prior;
		}
		while(p!=L1)
		{
			sum=p->data+carry;
			CreateList_Head(L3,sum%10);
			carry=sum/10;
			p=p->prior;
		}
		while(q!=L2)
		{
			sum=q->data+carry;
			CreateList_Head(L3,sum%10);
			carry=sum/10;
			q=q->prior;
		}
		if(carry)
		{
			CreateList_Head(L3,carry);
		}
	}
	else
	{
		if(L1->data==1&&L2->data==0)//正数加负数，转换为正数减正数
		{
			L2->data=1;     //先将其置为正数，防止减法运算时负号判断问题
			L3=Minus(L1,L2);
			L2->data=0;		//恢复原来的符号
		}
		else//负数加正数，转换为第二个数减第一个数
		{
			L1->data=1;
			L3=Minus(L2,L1);
			L1->data=0;
		}
	}
	if(L1->data==0&&L2->data==0)//两负数之和仍为负数
		L3->data=0;
	Adjust(L3);		//清除多余的0和0前面的负号
	return L3;
}
 
int ValueCompare(LinkList L1,LinkList L2)//数值（绝对值）比较函数，返回1为L1表示的值大于L2，返回0为L2表示的值大于L1
{
	if(ListLength(L1)>ListLength(L2))
		return 1;
	else
	{
		if(ListLength(L1)<ListLength(L2))
			return 0;
		else
		{
			LinkList p,q;
			p=L1->next;
			q=L2->next;
			while(p!=L1&&q!=L2)
			{
				if(p->data>q->data)
					return 1;
				else
                {
					if(p->data<q->data)
						return 0;
					else
                    {
                        p=p->next;
                        q=q->next;
                    }
                }
			}
		}
	}
	return 1;
}
 
LinkList Minus(LinkList L1,LinkList L2)//减法实现，
{
	LinkList p,q,p_head,q_head;
	LinkList L3;
	int borrow=0,difference,Positive;
	if(L1->data==1&&L2->data==1)//正数减正数的情况，正常处理(a-b)
	{
		Positive=ValueCompare(L1,L2);//判断步骤确定结果的正负
		if(Positive)
		{
			p=L1->prior;
			q=L2->prior;
			p_head=L1;
			q_head=L2;
		}
		else
		{
			p=L2->prior;
			q=L1->prior;
			p_head=L2;
			q_head=L1;
		}
		L3=InitList();
		while(p!=p_head&&q!=q_head)
		{
			difference=p->data-borrow-q->data;
			if(difference<0)
				borrow=1;
			else
				borrow=0;
			CreateList_Head(L3,difference+10*borrow);
			p=p->prior;
			q=q->prior;
		}
		while(p!=p_head)	//已经进行了大小比较，q所指链表一定先比p所指链表结束
		{
			if(!borrow)
			{
				CreateList_Head(L3,p->data);
				p=p->prior;
			}
			else
			{
				difference=p->data-borrow;
				if(difference<0)
					borrow=1;
				else
					borrow=0;
				CreateList_Head(L3,difference+10*borrow);
				p=p->prior;
			}
		}
		if(!Positive)
			L3->data=0;
	}
	else
	{
		if(L1->data==1&&L2->data==0)//正数减负数的情况，将其转换为正数加正数(a-(-b))=a+b
		{
			L2->data=1;
			L3=Add(L1,L2);
			L2->data=0;
		}
		else
			if(L1->data==0&&L2->data==1)//负数减正数的情况，将其转换为负的正数加正数-a-b=-(a+b)
			{
				L1->data=1;
				L3=Add(L1,L2);
				L1->data=0;
				L3->data=0;
			}
			else//负数减负数的情况，将其转换为第二个数（正）减第一个数（正）-a-(-b)=b-a
			{
				L1->data=1;
				L2->data=1;
				L3=Minus(L2,L1);
				L1->data=0;
				L2->data=0;
			}
	}
    Adjust(L3);		//清除多余的0和0前面的负号
	return L3;
}
 
LinkList Multiply(LinkList L1,LinkList L2)//乘法实现，利用列竖式的思路
{
	LinkList p,q,temp_ele,temp_L3;
	LinkList L3=InitList();
	CreateList_Head(L3,0);
	int carry=0,product,i,t;
	q=L2->prior;
    t=0;
	while(q!=L2)
	{
		p=L1->prior;
		temp_ele=InitList();
		while(p!=L1)
		{
			product=p->data*q->data+carry;
			CreateList_Head(temp_ele,product%10);
			carry=product/10;
			p=p->prior;
		}
		if(carry)
		{
			CreateList_Head(temp_ele,carry);
			carry=0;
		}
		for(i=0;i<t;i++)
		{
			CreateList_Tail(temp_ele,0);//低位补0
		}
		temp_L3=L3;
		L3=Add(temp_ele,temp_L3);
		DestroyList(temp_L3);
		DestroyList(temp_ele);
		q=q->prior;
		t++;
	}
	if((L1->data==1&&L2->data==1)||(L1->data==0&&L2->data==0))//正数乘正数和负数乘负数结果为正
		L3->data=1;
	else//负数乘正数和正数乘负数结果为负
		L3->data=0;
    Adjust(L3);
	return L3;
}

int LinearSearch(char arr[],int length,int target)//线性查找函数，返回值为找到的元素下标，未找到则返回-1
{
	int i=0;
	
	for( i=0;i<length;i++)
	{
		if(arr[i]==target)
			return i;
	}
	return -1;
}

void Add_0(LinkList L1,LinkList L2,int add_0_L1,int add_0_L2)//在链表L1和L2的末尾添加0，使两个链表原本为小数部分长度相等
{
	int i,j;
	if(add_0_L1>add_0_L2){
		for(i=0;i<add_0_L1;i++)
		{
			CreateList_Tail(L1,0);
		}
	}else if(add_0_L1<add_0_L2){
		for(j=0;j<add_0_L2;j++)
		{
			CreateList_Tail(L2,0);
		}
	}
}

void insert(LinkList L,int index,char data)//在链表L的倒数第index个位置插入值为data的结点
{
	
/*
	int lenth = ListLength(L);

    //插入的位置超出链表的范围
    if(index <0 || index >lenth)
		printf("提供的插入位置不合法\n");
	    exit(0);
    //插入的位置如果等于0，等同于插入头结点
    if(index == lenth)
    {
        CreateList_Head(L,data);
    }
    //插入的位置如果等于链表的长度，等同于插入尾结点
    else if(index == 0)
    {
        CreateList_Tail(L,data);
    }
*/
    //从尾结点开始往前遍历到插入的前一个位置
    Node*  tmp = L->prior;
        for (int i = 0; i <  index; i++)
        {
            tmp = tmp->prior;
        }


    Node* new_node = create_node(data);

    tmp->next->prior = new_node;
    new_node->next = tmp->next;
    tmp->next = new_node;
    new_node->prior = tmp;

}

void DecimalToNBase(char* number, int base, char* result) {
	base=7;
    if (base < 2 || base > 19) {
        snprintf(result, MAXSIZE * 2 + 2, "Error: Base must be between 2 and 19."); // Use snprintf for safety
        return;
    }
	char *decimalPoint = strchr(number, '.');
    long long integerPart = 0;
    double fractionalPart = 0.0;

    if (decimalPoint != NULL) {
        *decimalPoint = '\0'; 
        integerPart = atoll(number);
        fractionalPart = atof(decimalPoint + 1) / pow(10, strlen(decimalPoint + 1));
        *decimalPoint = '.'; // Restore the decimal point
    } else {
        integerPart = atoll(number);
    }

    char integerResult[MAXSIZE + 1] = {0};
    char fractionalResult[MAXSIZE + 1] = {0};
    int intIndex = 0;
    int fracIndex = 0;

    // Convert integer part
    if (integerPart == 0) {
        integerResult[intIndex++] = '0';
    } else {
        while (integerPart > 0) {
            int remainder = integerPart % base;
            integerResult[intIndex++] = (remainder < 10) ? (remainder + '0') : (remainder - 10 + 'A');
            integerPart /= base;
        }
    }

    // Reverse integer part
    for (int i = 0, j = intIndex - 1; i < j; i++, j--) {
        char temp = integerResult[i];
        integerResult[i] = integerResult[j];
        integerResult[j] = temp;
    }

    // Convert fractional part
    if (fractionalPart > 0) {
        while (fracIndex < MAXSIZE ) {
            fractionalPart *= base;
            int digit = (int)fractionalPart;
            fractionalResult[fracIndex++] = (digit < 10) ? (digit + '0') : (digit - 10 + 'A');
            fractionalPart -= digit;
             if (fractionalPart == 0) {
                break;
            }
        }
    }

    // Ensure null termination: IMPORTANT FOR REPEAT CALLS!
    integerResult[intIndex] = '\0';
    fractionalResult[fracIndex] = '\0';


    if (fracIndex > 0) {
        snprintf(result, MAXSIZE * 2 + 2, "%s.%s", integerResult, fractionalResult); // Use snprintf for safety
    } else {
        snprintf(result,MAXSIZE * 2 + 2, "%s", integerResult); // Use snprintf for safety
    }

	memset(decimalPoint,0,sizeof(decimalPoint));
}


long double BinaryToDecimal(char* binary)//二进制转十进制
{
    int length = strlen(binary);
    int dotIndex = -1;
    for (int i = 0; i < length; i++)
    {
        if (binary[i] == '.')
        {
            dotIndex = i;
            break;
        }
    }

    long double integerPart = 0;
    long double fractionalPart = 0;

    // 处理整数部分
    for (int i = 0; i < dotIndex; i++){
        integerPart = integerPart * 2 + (binary[i] - '0');
    }

    // 处理小数部分
    for (int i = dotIndex + 1; i < length; i++){
        fractionalPart = fractionalPart * 2 + (binary[i] - '0');
    }

    fractionalPart /= pow(2, length - dotIndex - 1);

    return integerPart + fractionalPart;
}

void ConvertToLinkedListArray(LinkList L, char* array) 
{
	LinkList p;
	int index=0;

	p=L->next;
    while(p!=L){
        if(p->data=='.'){		//小数点
			array[index]='.';
		}
		else{
			array[index]=p->data+'0';
		}
		p=p->next;		
		index++;
    }
	array[index]='\0'; // 添加字符串结束符
	printf("\n");
}

LinkList CopyList(LinkList L)//复制链表
{
    LinkList newL = InitList();
    LinkList p = L->next;
    while (p != L)
    {
        CreateList_Tail(newL, p->data);
        p = p->next;
    }
    return newL;
}

LinkList Power_Operations(char arr[],int n)//幂运算
{
	int len_fraction=strlen(arr)-LinearSearch(arr,strlen(arr),'.')-1;//得到小数部分长度
	int add_0_fraction=n*len_fraction;

	LinkList L_power,L_result,L_temp;
	L_power=InitList();
	L_result=InitList();
	L_temp=InitList();

	GetNum(L_power,arr); 
	L_result=CopyList(L_power);
	if(n>1){
	for(int i=2;i<=n;i++){
		L_temp=CopyList(L_result);
		L_result=Multiply(L_power,L_temp);
		}
	}
	insert(L_result,add_0_fraction,'.');
	
	return L_result;
}

typedef struct Fraction {
    int numerator;
    int denominator;
} Fraction;//分数结构体

Fraction parseFraction(char* fractionStr) {
    Fraction fraction;
    char* numeratorStr = strtok(fractionStr, "/");//分子
    char* denominatorStr = strtok(NULL, "/");//分母
    fraction.numerator = atoi(numeratorStr);//将字符串转换为整数
    fraction.denominator = atoi(denominatorStr);
    return fraction;
}

long double fractionToDouble(Fraction fraction) {
    return (long double)fraction.numerator / fraction.denominator;
}

long double parseAndConvertToDouble(char* fractionStr) {
    Fraction fraction = parseFraction(fractionStr);
    return fractionToDouble(fraction);
}//将分数转换为浮点数
/*
LinkList doubleToLinkList(double num) {
    LinkList L = InitList();
    if (num < 0) {
        L->data = 0; // 负数标记
        num = -num; // 取绝对值
    }
    long long intPart = (long long)num;
    double decimalPart = num - intPart;

    // 处理整数部分
    if (intPart == 0) {
        CreateList_Tail(L, 0);
    } else {
        while (intPart > 0) {
            CreateList_Tail(L, intPart % 10);
            intPart /= 10;
        }
    }
    ReverseList(L); // 反转整数部分

    // 添加小数点
    CreateList_Tail(L, '.');

    // 处理小数部分
    int count = 0;
    while (decimalPart > 0 && count < 10) {
        decimalPart *= 10;
        int digit = (int)decimalPart;
        CreateList_Tail(L, digit);
        decimalPart -= digit;
        count++;
    }

    return L;
}
*/


long double LinkListToDouble(LinkList L) {
    long double result = 0.0;
    int decimalPlace = 0;
    int isNegative = (L->data == 0) ? 1 : 0; // 检查链表头部的正负标志
    long double decimalFactor = 1.0;

    LinkList p = L->next;
    while (p != L) {
        if (p->data >= 0 && p->data <= 9) { // 确保是数字
            if (decimalPlace) { // 如果已经开始处理小数部分
                decimalFactor *= 0.1;
                result += (p->data * decimalFactor);
            } else {
                result = result * 10 + p->data;
            }
        } else if (p->data == '.') { // 遇到小数点
            decimalPlace = 1;
        }
        p = p->next;
    }

    if (isNegative) {
        result = -result;
    }

    return result;
}

int main()
{
	
	char number1[MAXSIZE]="13.56";
	char number2[MAXSIZE]="1323.31";
	printf("输入第一个十进制小数\n");
	scanf("%s",number1);
	printf("输入第二个十进制小数\n");
	scanf("%s",number2);

	int len_fraction1=strlen(number1)-LinearSearch(number1,strlen(number1),'.')-1;
	int len_fraction2=strlen(number2)-LinearSearch(number2,strlen(number2),'.')-1;//得到小数部分长度
	
	int max_len_fraction=(len_fraction1<len_fraction2)?len_fraction2:len_fraction1;
	int add_0_fraction1=max_len_fraction-len_fraction1;//得到需要补0的个数
	int add_0_fraction2=max_len_fraction-len_fraction2;//得到需要补0的个数



    LinkList L1,L2,AddResult,MinusResult,MultiplyResult;
	L1=InitList();
	L2=InitList();
	GetNum(L1,number1);
    GetNum(L2,number2);

	//在L1和L2的末尾补0，将小数化为整数方便运算
	Add_0(L1,L2,add_0_fraction1,add_0_fraction2);

    AddResult=Add(L1,L2);
    MinusResult=Minus(L1,L2);
	MultiplyResult=Multiply(L1,L2);

	int addresult_len_fraction=max_len_fraction;
	int minusresult_len_fraction=max_len_fraction;
	int multiplyresult_len_fraction=len_fraction1+len_fraction2+1;//获得输出计算结果时小数点插入位置
	
	insert(AddResult,addresult_len_fraction,'.');
	insert(MinusResult,minusresult_len_fraction,'.');
	insert(MultiplyResult,multiplyresult_len_fraction,'.');
    

	PrintResult(AddResult,MinusResult,MultiplyResult);   


	char add_result[MAXSIZE];
	char minus_result[MAXSIZE];
	char multiply_result[MAXSIZE];
	ConvertToLinkedListArray(AddResult, add_result);
	ConvertToLinkedListArray(MinusResult, minus_result);
	ConvertToLinkedListArray(MultiplyResult, multiply_result);

	int base = 7;
	printf("您所需转换的进制为:");
	scanf("%d",&base);

    char result_number_1[MAXSIZE];
    DecimalToNBase(number1, base, result_number_1);
	printf("number_1转换进制结果: %s\n", result_number_1);

	char result_number_2[MAXSIZE];
	DecimalToNBase(number2, base, result_number_2);
	printf("number_2转换进制结果: %s\n", result_number_2);

	char result_add[MAXSIZE];
	DecimalToNBase(add_result, base, result_add);
	printf("加法转换进制结果: %s\n", result_add);

	char result_minus[MAXSIZE];
	DecimalToNBase(minus_result, base, result_minus);
	printf("减法转换进制结果: %s\n", result_minus);

	char result_multiply[MAXSIZE];
	DecimalToNBase(multiply_result, base, result_multiply);
	printf("乘法转换进制结果: %s\n", result_multiply);


	char binary[MAXSIZE];
    printf("输入二进制小数: ");
    scanf("%s", binary);

    long double decimal = BinaryToDecimal(binary);

    __mingw_printf("转换为十进制结果: %Lf\n", decimal);


	int b=3;
	int d=2;
	char x[10]="1.4";
	char a[10]="3/7";
	char c[10]="-1/3";
	char e[10]="4";
	
	printf("输入a*x^b+c*x^d+e中的a,b,c,d,e,x(例如3/7*x^3+-1/3*x^2+4)\n");
	printf("输入a:");
	scanf("%s",a);
	printf("输入b:");
	scanf("%d",&b);
	printf("输入c:");
	scanf("%s",c);
	printf("输入d:");
	scanf("%d",&d);
	printf("输入e:");
	scanf("%s",e);
	printf("输入x:");
	scanf("%s",x);
	
	LinkList PowerResult_1,PowerResult_2;
	PowerResult_1=InitList();
	PowerResult_2=InitList();
	PowerResult_1=Power_Operations(x,b);
	PowerResult_2=Power_Operations(x,d);
	printf("x^%d的结果为:",b);
	PrintList(PowerResult_1);
	printf("x^%d的结果为:",d);
	PrintList(PowerResult_2);

	long double a_result=parseAndConvertToDouble(a);
	long double b_result=LinkListToDouble(PowerResult_1);
	long double c_result=parseAndConvertToDouble(c);
	long double d_result=LinkListToDouble(PowerResult_2);
	long double e_result=parseAndConvertToDouble(e);

    long double result=(a_result*b_result)+(c_result*d_result)+e_result;


	__mingw_printf("结果为: %Lf\n", result);


	DestroyList(L1);
    DestroyList(L2);
    DestroyList(AddResult);
	DestroyList(MinusResult);
	DestroyList(MultiplyResult);

	return 0;

}
