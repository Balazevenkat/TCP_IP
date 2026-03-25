
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<unistd.h>
#include<error.h>
#include<arpa/inet.h>
#include<netdb.h>

int main(int argc,char *argv[])
{
    int sfd;
    sfd=socket(AF_INET,SOCK_STREAM,0);
    printf("creating Socket..\n");
    if(sfd<0)
    {
        perror("socket");
        return 0;
    }

    printf("***Created socket server end***\n");
    struct sockaddr_in saddr;

    saddr.sin_family=AF_INET;
    saddr.sin_port=htons(atoi(argv[1]));
    saddr.sin_addr.s_addr=INADDR_ANY;

    int slen=sizeof(saddr);

    printf("binding..\n");
    if(bind(sfd,(const struct sockaddr*)&saddr,slen)<0)
    {
        perror("bind");
        return 0;
    }

    if(listen(sfd,0)<0)
    {
        perror("listen");
        return 0;
    }
    printf("listening...\n");

    int cfd;

    struct sockaddr_in caddr;
    int clen=sizeof(caddr);

    cfd=accept(sfd,(struct sockaddr*)&saddr,&clen);

    if(cfd<0)
    {
        perror("accpet");
        return 0;
    }

    int status,i;
    char buff[100];

    while(1)
    {
        printf("echo msg form clinet....\n");
        status=recv(cfd,buff,100,0);
        if(status<0)
        {
            perror("recv..");
            break;
        }
        else if(status==0)
        {
            printf("aborted without 3 H&S...\n");
            break;
        }
        else
        printf("msg: %s\n",buff);

        //char *ptr=buff,c;
//        int base=1,res=0;
//      int n=strlen(buff)-1;
 //       char *ptr=buff+n,c;


        printf("msg: %s\n", buff);
/*********
int res = 0;
char *ptr = buff;

// binary → integer
while (*ptr)
{
    if (*ptr == '0' || *ptr == '1')
        res = res * 2 + (*ptr - '0');
    ptr++;
}

// integer → string
sprintf(buff, "%d", res);

status = send(cfd, buff, 100, 0);

********/



        int base = 1, res = 0;
int n = strlen(buff) - 1;
char *ptr = buff + n;

// convert binary → integer
while (n >= 0)
{
    if (*ptr == '0' || *ptr == '1')
    {
        res += base * (*ptr - '0');
        base *= 2;
    }
    n--;
    ptr--;
}

// convert integer → string
int i = 0;
char temp[20];

if (res == 0)
{
    strcpy(buff, "0");
}
else
{
    while (res)
    {
        temp[i++] = (res % 10) + '0';
        res /= 10;
    }
    temp[i] = '\0';

    // reverse temp into buff
    int k = 0;
    while (i)
        buff[k++] = temp[--i];

    buff[k] = '\0';
}



        status=send(cfd,buff,100,0);
        if(status<0)
        {
            perror("send");
            break;
        }

        if(strcmp(buff,"quit")==0)
        break;
        }


    printf("***server going down***\n");
    close(sfd);
    close(cfd);

}
