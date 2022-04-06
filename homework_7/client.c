#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdbool.h>

int main(int argc, char** argv)
{
       printf("F");

    const char* server_path = "/tmp/server";
    const char* client_path = "/tmp/client";
     if (mkfifo(client_path, 0666) == -1) {
        perror("fifo make");
        return 1;
    }
    const int client_fd   = open(client_path, O_RDONLY);
    if (client_fd == -1) {
        perror("fifo open");
        return 1;
    }

    const int server_fd = open(server_path, O_WRONLY);
    if (server_fd == -1) {
          perror("fifo open");
          return 1;
    }
   
   printf("F");
    
    while(true)
    {

       char expr;
       printf("%s", "Operation - ");
       scanf("%c",&expr);
       write(server_fd, expr, 1);
       printf("%s", "left operand - ");
       scanf("%c",&expr);
       write(server_fd, expr, 1);
       printf("%s", "right operand - ");
       scanf("%c",&expr);
       write(server_fd, expr, 1);
       
       write(STDOUT_FILENO, "RESULT: ", 8);
       char buf;
       while (read(client_fd, &buf, 1) > 0)
           write(STDOUT_FILENO, &buf, 1);
       write(STDOUT_FILENO, "\n", 1);
    }
       close(server_fd);

    close(client_fd);

}