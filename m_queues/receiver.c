#include <stdio.h>
#include <stdlib.h>
#include <mqueue.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <sys/select.h>

#define MAX_MESSAGES 10
#define MAX_MSG_SIZE 256
#define MSG_BUFFER_SIZE (MAX_MSG_SIZE + 10)
#define QUEUE_PERMISSIONS 0660

int main(int argc, char *argv[])
{
	fd_set readfds;
	char buffer[MSG_BUFFER_SIZE];

	int msgq_fd = 0;
	int ret = 0;
	if(argc <= 1)
	{
		printf("provide a recipient msg() name : format </msgq-name>\n");
		return 0;
	}

	struct mq_attr attr;
	attr.mq_flags = 0;
	attr.mq_maxmsg = MAX_MESSAGES;
	attr.mq_msgsize = MAX_MSG_SIZE;
	attr.mq_flags = 0;

	msgq_fd = mq_open(argv[1], O_RDONLY | O_CREAT, QUEUE_PERMISSIONS, &attr);

	if(msgq_fd == -1)
	{
		printf("Client : mq_open failed, errno = %d\n", errno);
		exit(1);
	}

	for(;;)
	{
		FD_ZERO(&readfds);
		FD_SET(msgq_fd, &readfds);

		printf("Reciever blocked on select().....\n");
		select(msgq_fd + 1, &readfds, NULL, NULL, NULL);

		if(FD_ISSET(msgq_fd, &readfds))
		{
			printf("Msg recvd msg() %s \n", argv[1]);

			memset(buffer, 0, MSG_BUFFER_SIZE);

			ret = mq_receive(msgq_fd, buffer, MSG_BUFFER_SIZE, NULL);

			if(ret == -1)
			{
				printf("mq_receive error, errno = %d\n", errno);
				exit(1);
			}

			printf("Msg recvd from Queue = %s\n", buffer);
		}
	}
	
}
