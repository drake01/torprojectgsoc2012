#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include<string.h>
int
main(int argc,char *argv[])
{
    int sockfd_serv,sockfd_cli,servport,myport;
    struct sockaddr_in serv_addr,my_addr,cli_addr;
    if(argc<4)
    {
        printf("Provide servername portno. \nUsage: \n./john <serveradd \'localhost\' here> <port no. of alice 5000> <port no. of john say 4000>");
        exit(0);
    }
    servport=atoi(argv[2]);
    sockfd_serv=socket(AF_INET,SOCK_STREAM,0);
    if(sockfd_serv<0)
    {
        printf("Socket creation failed\n");
        exit(0);
    }
    struct hostent *host;
    host=gethostbyname(argv[1]);
    if(host==NULL)
    {
        printf("Unable to locate server");
        exit(0);
    }
    bzero((char *)&serv_addr,sizeof(serv_addr));
    serv_addr.sin_family=AF_INET;
    bcopy((char *)host->h_addr,(char *)&serv_addr.sin_addr.s_addr,host->h_length);
    serv_addr.sin_port=htons(servport);

    char buf[200];
    bzero(buf,sizeof(buf));
    if(connect(sockfd_serv,(struct sockaddr *)&serv_addr,sizeof(serv_addr))<0)
    {
        printf("Unable to connect\n");
        exit(0);
    }
    sockfd_cli=socket(AF_INET,SOCK_STREAM,0);
    if(sockfd_cli<0)
    {
        printf("Client socket not created\n");
        exit(0);
    }
    bzero((char *)&my_addr,sizeof(my_addr));
    my_addr.sin_family=AF_INET;
    myport=atoi(argv[3]);
    my_addr.sin_port=htons(myport);
    my_addr.sin_addr.s_addr=INADDR_ANY;
    if(bind(sockfd_cli,(struct sockaddr *)&my_addr,sizeof(my_addr))<0)
    {
        printf("Unable to bind \n");
        exit(0);
    }
    listen(sockfd_cli,5);
    int cli_len=sizeof(cli_addr);
    int newsockfd_cli=accept(sockfd_cli,(struct sockaddr *)&cli_addr,&cli_len);
    if(newsockfd_cli<0)
    {
        printf("Unable to accept\n");
        exit(0);
    }
    int n=read(newsockfd_cli,buf,199);
    if(n<0)
    {
        printf("Unable to read from client\n");;
        exit(0);
    }
    printf("%s",buf);
    //gets(buf);
     n=write(sockfd_serv,buf,strlen(buf));
    if(n<0)
    {
        printf("Unable to write in buf\n");
        exit(0);
    }
    //printf("%s",buf);

    return 0;
}
