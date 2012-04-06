#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
int
main(int argc, char *argv[])
{
    int newsockfd,sockfd,portno;
    struct sockaddr_in my_addr,cli_addr;
    if(argc<2)
    {
        printf("Enter port no. for server alice.\nUsage:\n./alice <portno. say 5000>\n");
        exit(0);
    }
    portno=atoi(argv[1]);
    sockfd=socket(AF_INET,SOCK_STREAM,0);
    if(sockfd==-1)
    {
        printf("Socket creation failed\n");
        exit(0);
    }
    bzero((char*)&my_addr,sizeof(my_addr));
    my_addr.sin_family=AF_INET;
    my_addr.sin_port=htons(portno);
    my_addr.sin_addr.s_addr=INADDR_ANY;

    if(bind(sockfd,(struct sockaddr*)&my_addr,sizeof(my_addr))<0)
    {
        printf("Unable to bind\n");
        exit(0);
    }
    listen(sockfd,5);
    int cli_len=sizeof(cli_addr);
    newsockfd=accept(sockfd,(struct sockaddr*)&cli_addr,&(cli_len));
    if(newsockfd<0)
    {
        printf("Unable to accept request\n");
        exit(0);
    }
    char buf[200];
    bzero(buf,sizeof(buf));
    int n=read(newsockfd,buf,199);
    if(n<0)
    {
        printf("Unable to read\n");
        exit(0);
    }
    int i=0;
    for(i=0;i<strlen(buf);i++)
    {
        buf[i]=buf[i]-1;
    }
    printf("%s",buf);
    return 0;

}
