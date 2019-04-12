#include <stdio.h>
#include <stdbool.h>

#define MaxSize 100 /*链表的最大长度*/
#define ElemType int

int volume=0; /*全局变量内存空闲区的块的长度*/
/*内存块的存储结构*/
typedef struct Node {
	ElemType data;
	int cur; /*游标代替指针指示结点在数组中的相对位置*/ 
}component, SLinkList[MaxSize];

/*函数声明*/
void InitSpace(SLinkList space); /*初始化"内存"*/
int GetLength(SLinkList space, int S); /*获得内存块长度*/
bool IsEmpty(SLinkList space, int S); /*判断内存空闲区空不空*/
int Malloc_SL(SLinkList space); /*malloc申请内存函数*/
void free_SL(SLinkList space, int K); /*free函数*/
void PrintList(SLinkList space, int S); /*输出链表*/
/*最坏适应法查找内存中第一个值为K的内存块*/
int WorstSearch_SL(SLinkList space, ElemType K, int S); 
void Insertion_Sort(SLinkList space, int S); /*直接插入排序*/

int main(int argc, char const* argv[])
{
	int i, j, S, K;
	
	SLinkList space; /*创建一个链表*/
	InitSpace(space); /*初始化该链表*/
	S=Malloc_SL(space); /*分配出一个结点给S,S做为链表的头结点*/
	/*此时链表头结点S的下标等于1了,所以链表下标从2开始*/
	
	printf("最佳适应法:进程申请一个大小为SIZE的存储区时，总是检查空闲区表的\n");
	printf("           第一个空闲区的大小是否大于或等于SIZE。\n");
	printf("           若空闲区小于SIZE，则分配失败；否则从空闲区中\n");
	printf("           分配SIZE的存储区给用户。\n\n");

	printf("请输入内存空闲区大小:");
	scanf("%d", &volume);
	
	/*因为space[1].data是首元结点,所以i从1开始*/
	printf("\n请创建各个内存块大小:");
	for (i=0; i<volume; i++) {
		j=Malloc_SL(space); /*此时下标已从2开始了*/
		scanf("%d", &space[j].data);
	}
	
	/*把内存空闲区调整成降序*/
	printf("把创建的内存空闲区调整成降序:\n");
	printf("调整降序过程:\n"); 
	Insertion_Sort(space, S);

	
	printf("\n\n请输入要申请的内存的值:");
	scanf("%d", &K);
	/*最坏适应法查找内存*/
	K=WorstSearch_SL(space, K, S);

	/*对内存空闲区做排序,这里使用二分插入排序法,可自行更改*/
	Insertion_Sort(space, S); 
	/*分出内存后内存空闲区状况*/
	printf("\n排序后后,系统空闲区状况:");
	PrintList(space, S);
	
	return 0; 
}

/*初始化内存空闲区*/
void InitSpace(SLinkList space) 
{
	/*将一维数组中各个分量间建立关系链成一个链表,space[0].cur为头指针*/
	int i; 
	
	for (i=0; i<MaxSize-1; i++) {
		space[i].cur=i+1;
		space[i].data=-1;
	}
	space[MaxSize-1].cur=0; /*链表中最后一个结点的"Next"指向NULL*/
}

/*获得内存块长度*/
int GetLength(SLinkList space, int S) 
{
	int length=0;
	while (space[space[S].cur].data!=-1) {
		length++;
	}
	
	return length;
}	

/*malloc函数*/
int Malloc_SL(SLinkList space) 
{
	/*模拟malloc函数,分配一个结点的空间*/
	/*若链表非空,就返回分配的结点的下标,否则返回0*/
	int i;
	
	i=space[0].cur;
	if (space[0].cur) {
		space[0].cur=space[i].cur;/*指向下一个待分配的结点的下标*/
	}
	
	return i;
}

/*free函数*/
void free_SL(SLinkList space, int K) 
{
	/*将下标为K的空闲结点回收到备用链表*/
	
	/*下标为K的空闲结点回收到space[0].cur*/
	/*下一次调用malloc是就会先分配 K */
	space[K].cur=space[0].cur;
	space[0].cur=K;
}

/*输出链表*/
void PrintList(SLinkList space, int S) 
{
	while (space[space[S].cur].data!=-1) {
		printf("%d\t", space[space[S].cur].data);
		S++;
	}
}

/*判断内存空闲区空不空*/
bool IsEmpty(SLinkList space, int S) 
{
	if (space[space[S].cur].data==-1) {
		return true;
	} else {
		return false;
	}
}
/*最坏适应法查找内存*/ 
int WorstSearch_SL(SLinkList space, ElemType K, int S) 
{
	int i, M;
	int Min=MaxSize; /*Min变量用来存申请内存和需要内存的差值最小*/
	/*先判断内存空闲区空不空*/
	if (IsEmpty(space, S)) {
		printf("内存不足,无法申请空间.\n");
		return -1;
	}
	
	/*i从链表的头结点开始找起,即下标为2开始找起*/
	if (space[space[S].cur].data>=K) {
		/*因为链表下标从2开始,但实际对应显示给用户看的链表是1开始*/
		printf("\n找到合适的内存块,下标为:%d\n", space[S].cur-1); 
		space[space[S].cur].data=0;
		volume--; /*申请内存成功后,内存块减少*/
	}
	else {
		printf("没找到合适的内存.");
		return 0; 
	}
	
	return i;
}

/*直接插入排序*/
void Insertion_Sort(SLinkList space, int S) 
{
	int i, Tmp, j;
	for (i=space[S].cur+1; i<=volume+3; i++) {
		Tmp=space[i].data; /*从序列中逐个拿出元素*/
		for (j=i; j>2 && space[j-1].data<Tmp; j--) {
			space[j].data=space[j-1].data; /*往后挪一位*/
		}
		space[j].data=Tmp; /*最后找到比Tmp大的数都往后挪了,留下正确的位置就插入*/
		PrintList(space, S); /*此为输出序列函数,此处每次输出一次排序后的序列*/ 
		printf("\n");
	}
	printf("\n内存空闲区调整成降序后:");
	PrintList(space, S);
} 
