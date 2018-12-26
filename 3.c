#include "header.h"

int main()
{
	/*共享内存的设置*/
	int shIDS;
	//分配共享内存块
	shIDS = shmget(shKeyS, sizeof(sharedBuf), IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
	//绑定
	sharedBuf *sBuf = (sharedBuf*)shmat(shIDS, 0, 0);
	sBuf->over=0;

	/*创建2个信号灯，*/
	int semId;
	semId = semget(semKey, 2, IPC_CREAT | 0666);

	union semun arg;
	arg.val = 10;
	semctl(semId, 0, SETVAL, arg);    // SEmpty
	arg.val = 0;
	semctl(semId, 1, SETVAL, arg);     // SFull

	int pid1, pid2; //2个子进程的ID号

	pid1=fork();
	
	if (pid1 == 0)
	{
		printf("in p1\n");
		execl("./3_read","3_read", NULL);
		exit(0);
	}
	else
	{
		pid2=fork();
		if (pid2 == 0) 
		{
			printf("in p2\n");
			execl("./3_write","3_write", NULL);
			exit(0);
		}
		else
		{
			/*等待退出*/
			waitpid(pid1, NULL, 0);
			waitpid(pid2, NULL, 0);

			/*删除信号灯&共享内存*/
			semctl(semId, 1, IPC_RMID, 0);
			shmdt(sBuf);
			shmctl(shIDS, IPC_RMID, 0);
		}
	}
	
}
