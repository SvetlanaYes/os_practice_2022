#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>

typedef struct msg_buffer {
	long msg_type;
	char msg_text[5];
} message_t;

void  INThandler(int sig)
{
     char  c;

     signal(sig, SIG_IGN);
	// msgctl(msgid1, IPC_RMID, NULL);
	// msgctl(msgid2, IPC_RMID, NULL);
     signal(SIGINT, INThandler);
}

int main()
{
	key_t key1 = 55;
	int msgid1 = msgget(key1, 0666 | IPC_CREAT);
    if (msgid1 == -1)
    {
        perror("Unable to get message queue\n");
        return 1;
    }
	key_t key2 = 77;

	int msgid2 = msgget(key2, 0666 | IPC_CREAT);
    if (msgid2 == -1)
    {
        perror("Unable to get message queue\n");
        return 1;
    }
	
    message_t msg1;
	message_t msg2;

    signal(SIGINT, INThandler);

	while(1)
	{
      msg1.msg_type = 1;
    
	  strcpy(msg1.msg_text, "PING\0");

	  if (msgsnd(msgid1, &msg1, sizeof(msg1), 0) == -1)
      {
         perror("Unable to send a message\n");
         return 1;
      }
      printf("Data send is : %s \n", msg1.msg_text);
      int bytes_count = msgrcv(msgid2, &msg2, sizeof(message_t), 1, 0);
      if (bytes_count == -1)
      {
        perror("Unable to receive a message.\n");
        return 1;
      }
	   printf("Data Received is : %s \n", msg2.msg_text);
    }
	return 0;
}