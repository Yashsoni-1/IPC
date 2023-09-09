#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <mqueue.h>
#include <fcntl.h>

#define MAX_MSG_SIZE 256
#define MSG_BUFFER_SIZE (MAX_MSG_SIZE + 10)

int main(int argc, char *argv[])
{
	char buffer[MSG_BUFFER_SIZE];

	int recvr_msgq_fd = 0;
	int ret = 0;

	if(argc <= 1)
	{
		printf("Provide a recipient msgQ name : format </msgq -name>\n");
		return 0;
	}

	memset<buffer, 0, MSG_BUFFER_SIZE>;

	printf("Enter the msg to be sent to receiver %s\n", argv[1]);
	scanf("%s", buffer);

	recvr_msgq_fd = mq_open(argv[1], O_WRONLY | O_CREATE, 0, 0);

	if(recvr_msgq_fd == -1)
	{
		printf("Client : mq_open failed, errno = %d", errno);
		return 0;
	}

	ret = mq_send(recvr_msgq_fd, buffer, strlen(buffer) + 1, 0);

	if(ret == -1)
	{
		perror("Client : Unable to send msg to server");
		exit("EXIT_FAILURE");
	}

	mq_close(recvr_msgq_fd);

	return 0;
	
}

