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
    int sockfd,newsockfd;
    struct sockaddr_in serv_addr;
    if(argc<3)
    {
        printf("Provide servername portno. \nUsage: \n./bob <serveradd \'localhost\' here> <port no. of john>");
        exit(1);
    }
    int portno=atoi(argv[2]);
    sockfd=socket(AF_INET,SOCK_STREAM,0);
    if(sockfd<0)
    {
        printf("Socket creation failed\n");
        exit(1);
    }
    struct hostent *host;
    host=gethostbyname(argv[1]);
    if(host==NULL)
    {
        printf("Server not found\n");
        exit(0);
    }
    bzero((char*)&serv_addr,sizeof(serv_addr));
    serv_addr.sin_family=AF_INET;
    bcopy((char *)host->h_addr,(char *)&serv_addr.sin_addr.s_addr,host->h_length);
    serv_addr.sin_port=htons(portno);
    char buf[200];
    bzero(buf,sizeof(buf));
    if(connect(sockfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr))<0)
    {
        printf("Unable to connect \n");
        exit(0);
    }
    gets(buf);
    int i;
    for(i=0;i<strlen(buf);i++)
    {
        buf[i]=buf[i]+1;
    }
    int n=write(sockfd,buf,strlen(buf));
    if(n<0)
    {
        printf("Unable to send data\n");
        exit(0);
    }
    return 0;
}
