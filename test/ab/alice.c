#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include<stdlib.h>
#define STDIN 0
int
main(int argc,char *argv[])
{
    if(argc<2)
    {
        printf("Enter port no. also\nUsage\n./alice <port no. say 5000>");
        exit(0);
    }
    int sockfd,newsockfd;
    struct sockaddr_in serv_addr,cli_addr;
    sockfd=socket(AF_INET,SOCK_STREAM,0);
    if(sockfd<0)
    {
        printf("Socket creation failed!");
        exit(0);
    }
    bzero((char *)&serv_addr,sizeof(serv_addr));
    serv_addr.sin_family=AF_INET;
    serv_addr.sin_addr.s_addr=INADDR_ANY;
    int portno=atoi(argv[1]);
    serv_addr.sin_port=htons(portno);
    if(bind(sockfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr))<0)
    {
        printf("Unable to bind");
        exit(0);
    }
    char buf[200];
    bzero(buf,sizeof(buf));
    listen(sockfd,5);
    int cli_size=sizeof(cli_addr);
    newsockfd=accept(sockfd,(struct sockaddr *)&cli_addr,&cli_size);
    if(newsockfd<0)
    {
        printf("Unable to accept request");
        exit(0);
    }
    while(1)
    {
        fd_set readfds;
        FD_ZERO(&readfds);
        FD_SET(STDIN,&readfds);
        FD_SET(newsockfd,&readfds);
        select(newsockfd+1,&readfds,NULL,NULL,NULL);
        if(FD_ISSET(STDIN,&readfds))
        {
            gets(buf);
            int n=write(newsockfd,buf,strlen(buf));
            if(n<0)
            {
                printf("\nUnable to send text to client\n");
                exit(0);
            }

        }
        if(FD_ISSET(newsockfd,&readfds))
        {
            bzero(buf,sizeof(buf));
            int n=read(newsockfd,buf,199);
            if(n<0)
            {
                printf("Unable");
                exit(0);
            }
            if(strcmp(buf,"EXIT")!=0)
            {
                printf("[client]>>%s\n",buf);
            }
            else
            {
                close(newsockfd);
                exit(0);
            }
        }

    }
    return 0;
}
