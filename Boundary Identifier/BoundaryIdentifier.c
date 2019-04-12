#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
typedef int WordType; //字的类型为int型 

//结点的数据结构 
typedef struct WordType *PtrlSqlList;
struct WordType {
	PtrlSqlList llink; //指向内存块的左邻结点 
	PtrlSqlList rlink; //指向内存块的右邻结点
	int startAddress; //内存块起始地址
	int endAddress; //内存块末尾地址 
	int memory; //标识内存块大小
	int tag; //标识内存块是否空闲; 1为占用,0为空闲
	int ListSize; //记录可用空间表中内存块个数 
};

//函数声明
PtrlSqlList InitialMemoryList(); //初始化可用空间表 
void printMemoryList(PtrlSqlList ptrlHead); //输出可用空间表
PtrlSqlList MallocUserMemory(PtrlSqlList ptrlHead, int userMemory); //分配用户申请的内存空间 
bool RetrieveFreeMemory(PtrlSqlList ptrlHead, PtrlSqlList userBlock); //回收空闲内存块
 
int main(int argc, char const* argv[]) 
{
	int userMemory;
	PtrlSqlList userBlock;
	bool retrieveResult = false; 
	
	PtrlSqlList ptrlHead = InitialMemoryList(); //初始化可用空间表 
	printMemoryList(ptrlHead); //输出可用空间表 
	
	printf("输入申请内存块大小:");
	scanf("%d", &userMemory);
	userBlock = MallocUserMemory(ptrlHead, userMemory);
	if(userBlock) {
		printf("分配成功.\n"); 
		printf("分配出去的内存块信息: 容量:%d  起始地址:%d  末尾地址:%d\n", userBlock->memory, userBlock->startAddress, userBlock->endAddress);
		printMemoryList(ptrlHead); //输出分配后的可用空间表 
	}
	
	//回收空闲块 
	retrieveResult = RetrieveFreeMemory(ptrlHead, userBlock);
	if(retrieveResult) {
		printf("回收成功.\n");
		printMemoryList(ptrlHead); //输出分配后的可用空间表
	}
	
	return 0;
} 
//初始化可用空间表 
PtrlSqlList InitialMemoryList() 
{
	PtrlSqlList ptrlHead, p; //表头指针 
	int i;
	 
	ptrlHead = (PtrlSqlList)malloc(sizeof(struct WordType));
	ptrlHead->rlink = NULL;
	ptrlHead->tag = -1; //表头结点tag设置为-1,作为遍历结束标志 
	ptrlHead->ListSize = 0;
	 
	//创建三个内存块
	for(i=1; i<=3; i++) {
		int memory, start;
		printf("请以此输入第%d块内存块的容量和起始地址:", i);
		scanf("%d %d", &memory, &start); 
		PtrlSqlList ptrlMemory = (PtrlSqlList)malloc(sizeof(struct WordType));
		ptrlMemory->startAddress = start; //设置起始地址 
		ptrlMemory->memory = memory; //设置内存块大小 
		ptrlMemory->endAddress = start+memory; //设置末尾地址 
		ptrlMemory->tag = 0; //设置空闲标志位 
		
		if(ptrlHead->rlink!=NULL) {
			for(p = ptrlHead; p->rlink!=ptrlHead; p=p->rlink); //找到表尾结点
			//把空闲块链接到可用表中 
			p->rlink = ptrlMemory;
			ptrlMemory->llink = p;
			ptrlMemory->rlink = ptrlHead; 
		} else if(ptrlHead->rlink==NULL) {
			ptrlHead->rlink = ptrlMemory; //第一个放进去可用表的内存块 
			ptrlMemory->llink = ptrlHead; //左邻结点指向表头结点 
			ptrlMemory->rlink = ptrlHead; //表尾结点的右邻结点指向表头指针指向的结点 
		}
		ptrlHead->ListSize++;
	}
	return ptrlHead; 
} 

//输出可用空间表
void printMemoryList(PtrlSqlList ptrlHead) 
{
	PtrlSqlList p;
	p = ptrlHead->rlink;
	int i;
	
	printf("\n当前可用表情况:\n");
	while(p->tag!=-1) {
		printf("容量块\n内存块大小:%d\n起始地址:%d\n末尾地址:%d\n\n", p->memory, p->startAddress, p->endAddress);
		p = p->rlink;
	}
//	for(i=0; i<ptrlHead->ListSize; i++) {
//		printf("容量块\n内存块大小:%d\n起始地址:%d\n末尾地址:%d\n\n", p->memory, p->startAddress, p->endAddress);
//		p = p->rlink;
//	}
	return;
} 

//分配用户申请的内存空间 
PtrlSqlList MallocUserMemory(PtrlSqlList ptrlHead, int userMemory)
{
	PtrlSqlList p;
	int cutoff = 2000; //阈值
	
	//遍历可用空间表查找,这里选择首次适应法
	for(p = ptrlHead->rlink; p->memory<userMemory || p->tag!=-1; p = p->rlink) {
		if(p->tag == -1 || !p) {
			return NULL; //找不到内存适合的结点,返回NULL 
		} else if(p->memory>=userMemory) {
			//空闲块大小和用户申请大小差小于阈值,则空闲块整块分配 
			if(p->memory-userMemory<cutoff) {
				if(p == ptrlHead) {
					ptrlHead = NULL;
				} else {
					p->llink->rlink = p->rlink;
					p->rlink->llink = p->llink;
					p->llink = p->rlink = p;
					p->tag = 1; //已占用 
					return p;
				}
			} else {
				//差值大于阈值,则把空闲块的后userMemory部分内存分配
				PtrlSqlList newBlock = (PtrlSqlList)malloc(sizeof(struct WordType));
				newBlock->memory = userMemory;
				newBlock->startAddress = p->endAddress-userMemory+1; //设置分配块的头部
				newBlock->endAddress = p->endAddress;
				newBlock->tag = 1;
				 
				p->memory-=userMemory; //设置剩余空闲块大小
				//设置分配后剩余空闲块的末尾地址,模拟分配后userMemory部分内存 
				p->endAddress-=userMemory;  
				return newBlock;
			}
		}
	}  
} 

//回收空闲内存块
bool RetrieveFreeMemory(PtrlSqlList ptrlHead, PtrlSqlList userBlock) 
{
	/*回收回来的内存块有四种情况:
	 *1、内存块的左右邻区均为占用块
	 *2、左邻区为空闲块
	 *3、右邻区为空闲块
	 *4、左右邻区均为空闲块 
	 */
    userBlock->tag = 0;
    PtrlSqlList leftBlock = ptrlHead->rlink; 
    PtrlSqlList rightBlock = ptrlHead->rlink;
	  
	//在可用空间表中寻找左右邻区结点 
	for(leftBlock; leftBlock->endAddress+1!=userBlock->startAddress && leftBlock->tag!=-1; leftBlock = leftBlock->rlink) {
		if(leftBlock->endAddress+1 == userBlock->startAddress) {
	  		break; //找到左邻结点就停止 
		} 
	}
	for(rightBlock; rightBlock->startAddress-1!=userBlock->endAddress && rightBlock->tag!=-1; rightBlock = rightBlock->rlink) {
		if(rightBlock->startAddress-1 == userBlock->endAddress) {
	  		break; //找到右邻结点就停止 
		} 
	} 
	//判断左右邻区结点是否正确
	if(leftBlock->endAddress+1 == userBlock->startAddress) {
		leftBlock->tag = 0; 
		printf("左邻区查找成功,起始地址:%d 末尾地址:%d\n", leftBlock->startAddress, leftBlock->endAddress);
	} else {
		leftBlock->tag = -1;
		printf("左邻区查找失败,无左邻区结点\n"); 
	}
	if(rightBlock->startAddress-1 == userBlock->endAddress) {
		rightBlock->tag = 0;
		printf("右邻区查找成功,起始地址:%d 末尾地址:%d\n", rightBlock->startAddress, rightBlock->endAddress);
	} else {
		rightBlock->tag = -1;
		printf("右邻区查找失败,无右邻区结点\n");
	}
	  
	//第一种情况:如果左右邻区均为占用块,就把回收的内存块放到表头
	if(leftBlock->tag == -1 && rightBlock->tag == -1) {
	  	userBlock->rlink = ptrlHead->rlink;
	  	userBlock->llink = ptrlHead;
	  	ptrlHead->rlink->rlink = userBlock;
	  	ptrlHead->rlink = userBlock;
	} 
	  
	//第二种情况:如果只有左邻区是空闲块,则把回收块和它的左邻区空闲块合并
	if(leftBlock->tag == 0 && rightBlock->tag == -1) {
	  	leftBlock->memory+=userBlock->memory; //两个内存块大小合并
		leftBlock->endAddress = userBlock->endAddress;
		free(userBlock);
	} 
	
	//第三种情况:如果只有右邻区是空闲块,则把回收块和它的右邻区合空闲块并 
	if(leftBlock->tag == -1 && rightBlock->tag == 0) {
		rightBlock->memory+=userBlock->memory;
		rightBlock->startAddress = userBlock->startAddress;
		free(userBlock);
	}
	
	//第四种情况:如果左右邻区均为空闲块,则三个空闲块一起合并
	if(leftBlock->tag == 0 && rightBlock->tag == 0) {
		leftBlock->memory += (userBlock->memory+rightBlock->memory); //三块内存大小合并
		leftBlock->rlink = rightBlock->rlink;
		rightBlock->rlink->llink = leftBlock;
		leftBlock->endAddress = rightBlock->endAddress;
		free(rightBlock);
		free(userBlock);
	}
	
	return true;	
} 
