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
/*首次适应发查找内存中第一个值为K的内存块*/
int FirstSearch_SL(SLinkList space, ElemType K, int S); 
void Binary_Sort(SLinkList space, int S); /*折半插入排序*/

int main(int argc, char const* argv[])
{
	int i, j, S, K;
	
	SLinkList space; /*创建一个链表*/
	InitSpace(space); /*初始化该链表*/
	S=Malloc_SL(space); /*分配出一个结点给S,S做为链表的头结点*/
	/*此时链表头结点S的下标等于1了,所以链表下标从2开始*/
	
	printf("首次适应法:从空闲区表的第一个表目开始查询，\n");
	printf("           直到首次找到等于或大于SIZE的空闲区。\n\n");
	
	printf("请输入内存空闲区大小:");
	scanf("%d", &volume);
	
	/*因为space[1].data是首元结点,所以i从1开始*/
	printf("\n请创建各个内存块大小:");
	for (i=0; i<volume; i++) {
		j=Malloc_SL(space); /*此时下标已从2开始了*/
		scanf("%d", &space[j].data);
	}
	
	/*输出内存块*/
	printf("\n\n内存空闲区各个块大小为:");
	PrintList(space, S);
	
	printf("\n\n请输入要申请的内存的值:");
	scanf("%d", &K);
	/*首次适应法查找内存*/
	K=FirstSearch_SL(space, K, S);

	/*对内存空闲区做排序,这里使用二分插入排序法,可自行更改*/
	Binary_Sort(space, S); 
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
/*首次适应法查找内存*/ 
int FirstSearch_SL(SLinkList space, ElemType K, int S) 
{
	int i;
	
	/*先判断内存空闲区空不空*/
	if (IsEmpty(space, S)) {
		printf("内存不足,无法申请空间.\n");
		return -1;
	}
	
	i=space[S].cur; /*i从链表的头结点开始找起*/
	while (i<=volume && space[i].data<K) {
		i=space[i].cur; /*i指向下一个结点,相当于p=p->Next*/
	}
	
	if (space[i].data>=K) {
		/*i-1是因为链表头结点下标为1,首元结点下标为2开始*/
		printf("\n找到合适的内存块,下标为:%d", i-1); 
		space[i].data=0;
		volume--; /*申请内存成功后,内存块减少*/ 
	} else {
		printf("没找到合适的内存.");
		return 0; 
	}
	
	return i;
}

/*二分插入排序*/
void Binary_Sort(SLinkList space, int S) 
{
	int front, rear;
	int i, j, Tmp, m;
	
	printf("\n\n开始对内存空闲区做排序....\n");
	printf("\n排序过程:\n");
	for (i=2; i<=volume+2; i++) {
		Tmp=space[i].data;
		front=1;
		rear=i-1; /*因为要从已排好序列中做插入排序,所以rear=i-1开始*/ 
		/*二分查找找出已排好序列中元素*/
		while (front<=rear) {
			m=(front+rear)/2;
			if (space[m].data>Tmp) {
				/*证明插入点在前半部分*/
				rear=m-1;
			} else {
				/*否则插入点在后半部分*/ 
				front=m+1;
			}
		}
		/*找到插入*/
		/*front插入位置的起点,i是已排好序列的终点*/ 
		/*把已排好序列中插入位置fonrt及之后的元素往后挪*/
		for (j=i; j>0 && j>=front; j--) {
			space[j].data=space[j-1].data; /*往后挪一位*/
		}
		space[front].data=Tmp;
		PrintList(space, S);
		printf("\n");	
	}
}
