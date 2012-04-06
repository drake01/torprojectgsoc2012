#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include<string.h>
#include<stdlib.h>
#define STDIN 0
int main(int argc,char *argv[])
{
    if(argc<3)
    {
        printf("Enter servername and portno.\nUsage:\n./bob <serveraddr here localhost> <portno. say 5000>");
    }
    int portno=atoi(argv[2]);
    struct sockaddr_in serv_addr;
    struct hostent *server;
    server=gethostbyname(argv[1]);
    if(server==NULL)
    {
        printf("Unable to locate server");
        exit(0);
    }
    int sockfd=socket(AF_INET,SOCK_STREAM,0);
    if(sockfd<0)
    {
        printf("Socket not created");
        exit(0);
    }
    bzero((char *)&serv_addr,sizeof(serv_addr));
    serv_addr.sin_family=AF_INET;
    serv_addr.sin_port=htons(portno);
    bcopy((char *)server->h_addr,(char *)&serv_addr.sin_addr.s_addr,server->h_length);
    char buf[200];
    if(connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr))<0)
    {
        printf("Unable to connect");
        exit(0);
    }
    while(1)
    {
        fd_set readfds;
        FD_ZERO(&readfds);
        FD_SET(STDIN,&readfds);
        FD_SET(sockfd,&readfds);
        select(sockfd+1,&readfds,NULL,NULL,NULL);
        if(FD_ISSET(STDIN,&readfds))
        {
            gets(buf);
            int n=write(sockfd,buf,strlen(buf));
            if(n<0)
            {
                printf("\nUnable to send text to client\n");
                exit(0);
            }
            if(strcmp(buf,"EXIT")==0)
            {
                exit(0);
            }

        }
        if(FD_ISSET(sockfd,&readfds))
        {
            bzero(buf,sizeof(buf));
            int n=read(sockfd,buf,199);
            if(n<0)
            {
                printf("Unable to read");
                exit(0);
            }
            printf("[server]>>%s\n",buf);

        }

    }
            return 0;
}
