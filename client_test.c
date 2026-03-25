#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include<arpa/inet.h>

int main(int argc,char *argv[])
{
    if(argc < 3){
        printf("Usage: ./client <port> <server-ip>\n");
        return 0;
    }

    int fd;
    printf("Creating socket....\n");
    fd = socket(AF_INET, SOCK_STREAM, 0);
    if(fd < 0)
    {
        perror("socket");
        return 0;
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(argv[1]));
    addr.sin_addr.s_addr = inet_addr(argv[2]);

    int len = sizeof(addr);

    printf("Connecting.....\n");
    if(connect(fd, (struct sockaddr*)&addr, len) < 0)   // FIXED
    {
        perror("connect");
        return 0;
    }
    printf("Connected...\n");

    int status;
    char buff[100];

    while(1)
    {
        printf("Enter Msg: ");
        fgets(buff,100,stdin);
        buff[strlen(buff)-1] = '\0';

        status = send(fd, buff, 100, 0);

        if(status < 0)
        {
            perror("send");
            break;
        }

        printf("Client sent: %s\n", buff);

        status = recv(fd, buff, 100, 0);
        if(status < 0)
        {
            perror("recv");
            break;
        }

        printf("Server reply: %s\n", buff);

        if(strcmp(buff, "quit") == 0)
            break;
    }

    printf("**client going down**\n");
    close(fd);
    return 0;
}
