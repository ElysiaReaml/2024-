#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAXSIZE 300

int LinearSearch(char arr[],int length,int target)//线性查找函数，返回值为找到的元素下标，未找到则返回-1
{
	int i=0;
    
    for(i=0;i<length;i++)
	{
		if(arr[i]==target)
			return i;
	}
	return -1;
}

int main() {
	char number1[MAXSIZE]="11114.51411";
	char number2[MAXSIZE]="1919.810";

	int len_fraction1=strlen(number1)-LinearSearch(number1,strlen(number1),'.')-1;
	int len_fraction2=strlen(number2)-LinearSearch(number2,strlen(number2),'.')-1;
    int max_len_fraction=(len_fraction1<len_fraction2)?len_fraction2:len_fraction1;
	int add_0_fraction1=max_len_fraction-len_fraction1;//得到需要补0的个数
	int add_0_fraction2=max_len_fraction-len_fraction2;//得到需要补0的个数
		int addresult_len_fraction=max_len_fraction;
	int minusresult_len_fraction=max_len_fraction;
	int multiplyresult_len_fraction=len_fraction1+len_fraction2;//获得输出计算结果时小数点插入位置


    printf("len1=%d\n",strlen(number1));
    printf("len2=%d\n",strlen(number2));
    printf("max_len_fraction=%d\n",max_len_fraction);
    printf("len_fraction1=%d\n",len_fraction1);
    printf("len_fraction2=%d\n",len_fraction2);
    printf("add_0_fraction1=%d\n",add_0_fraction1);
    printf("add_0_fraction2=%d\n",add_0_fraction2);
	printf("addresult_len_fraction=%d\n",addresult_len_fraction);
	printf("minusresult_len_fraction=%d\n",minusresult_len_fraction);
	printf("multiplyresult_len_fraction=%d\n",multiplyresult_len_fraction);
    return 0;
}