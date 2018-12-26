#ifndef HEADER_INCLUDED
#define HEADER_INCLUDED

#include <pthread.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <sys/wait.h>

#include <stdlib.h>
#include <unistd.h>


union semun {
	int val;
	struct semid_ds *buf;
	unsigned short int *array;
	struct seminfo *info;
};

/*P操作*/
void P(int semid, int index)
{
	struct sembuf sem;
	sem.sem_num = index;	//信号量下标
	sem.sem_op = -1;	//执行操作
	sem.sem_flg = 0;	//操作标志
	semop(semid, &sem, 1);	//1为执行命令个数
	return;
}

/*V操作*/
void V(int semid, int index)
{
	struct sembuf sem;
	sem.sem_num = index;
	sem.sem_op = 1;
	sem.sem_flg = 0;
	semop(semid, &sem, 1);
	return;
}


typedef struct sharedBuf
{
	char data[10][80];	//存放数据
	int length[10];		//长度
	int over;			//读取结束标志
}sharedBuf;

const key_t shKeyS = 0x2212;	//共享内存键值
key_t semKey = 1;		//信号量键值

#endif // HEADER_INCLUDED
