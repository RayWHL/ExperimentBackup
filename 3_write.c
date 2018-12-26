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

	printf("write.\n");

	

	FILE *fpw;
	if ((fpw = fopen("get2", "wb")) == NULL)
	{
		printf("open ERROR");
	}


	int writeNum = 0;

	while (1)
	{
		if(sBuf->length[writeNum]==0 && sBuf->over==1)
		{
			break;
		}

		P(semId, 1);
		//printf("write a block to the file  num:%d size:%d\n",writeNum,sBuf->length[writeNum]);

		fwrite(sBuf->data[writeNum], sizeof(char), sBuf->length[writeNum], fpw);
		writeNum = (writeNum + 1) % 10;

		V(semId, 0);
	}

	
	printf("write successfully!\n");
	fclose(fpw);

	shmdt(sBuf);
	return 0;
}



