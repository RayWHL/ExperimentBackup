#include "header.h"

int main()
{
	//获取信号灯
	int semId;
	semId = semget(semKey, 2, IPC_CREAT | 0666);

	//获取共享S的内存

	int shIDS;
	shIDS = shmget(shKeyS, sizeof(sharedBuf), IPC_CREAT | 0666);
	sharedBuf *sBuf = (sharedBuf*)shmat(shIDS, NULL, 0);

	printf("read\n");

	//file control
	FILE *fp;
	if ((fp = fopen("3", "rb")) == NULL)
	{
		printf("open ERROR");
	}

	int readNum = 0;
	int size;
	while(1)
	{
		P(semId, 0);
		size = fread(sBuf->data[readNum], sizeof(char), 80, fp);
		sBuf->length[readNum] = size;

		readNum = (readNum + 1) % 10;
		if(size==0 )
		{
			sBuf->over=1;
			printf("over!\n");
			break;
		}

		V(semId, 1);

		//printf("read a bolck size : %d\n",size );
	}
	printf("read successfully!\n");
	fclose(fp);
	shmdt(sBuf);
}
