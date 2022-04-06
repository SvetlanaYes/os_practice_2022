#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h> 


int main(int argc, char** argv)
{
    const char* server_path = "/tmp/server";
    const char* client_path = "/tmp/client";
    if (mkfifo(server_path, 0666) == -1) {
          perror("fifo make");
          return 1;
    }
    const int server_fd = open(server_path, O_RDONLY);
    if (server_fd == -1) {
          perror("fifo open");
          return 1;
    }
    const int client_fd   = open(client_path, O_WRONLY);
    if (client_fd == -1) {
        perror("fifo open");
        return 1;
    }
    while(true)
    {

       char expr[3];
       char buf;
       int res = 0;
       for (int i = 0;read(server_fd, &buf, 1) > 0; ++i)
       {
           expr[i] = buf;
       }
       int left_operand = atoi(expr[1]);
       int right_operand = atoi(expr[2]);
       if (expr[0] == '+')
       {
          res = left_operand + right_operand;
       }
       if (expr[0] == '-')
           {
               res = left_operand - right_operand;
           }
           if (expr[0] == '/')
           {
               res = left_operand / right_operand;
           }
           if (expr[0] == '%')
           {
               res = left_operand % right_operand;
           }
           if (expr[0] == '*')
           {
               res = left_operand - right_operand;
           }
       char ress = res + "0";
       write(client_fd, ress, 1);
       write(STDOUT_FILENO, "RESULT: ", 8);
       
       write(STDOUT_FILENO, "\n", 1);
    }
       close(server_fd);

    close(client_fd);

}