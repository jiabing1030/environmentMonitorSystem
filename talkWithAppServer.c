#include <talkWithAppServer.h>
#include <myglobal.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <strings.h>
#include <stdio.h>
void payloadHandler(int packetType,unsigned char *data,int length);
static int listen_fd;
static void (*storeControlData)(unsigned char *data);
static void (*sendControlData)(unsigned char *data);
int talkwithAppServerInit(void (*dataCenterCB)(unsigned char *data),void (*sensorNetCB)(unsigned char *data))
{
    struct sockaddr_in listenAddr;
    int socksize,ret;
    int i=1;
    socksize=sizeof(struct sockaddr);
    bzero(&listenAddr,sizeof(listenAddr));
    listenAddr.sin_family=AF_INET;
    listenAddr.sin_port=htons(SERVER_PORT);
    listenAddr.sin_addr.s_addr=INADDR_ANY;
    listen_fd=socket(AF_INET,SOCK_STREAM,0);
    setsockopt( listen_fd,SOL_SOCKET,SO_REUSEADDR,&i,sizeof(i));
    ret=bind( listen_fd,(struct sockaddr*)&listenAddr,socksize);
    if(ret==0)
        printf("Bind successfully:%d\n",SERVER_PORT);
    ret=listen( listen_fd,8);
    if(ret==0)
        printf("Listen successfully!\n");
    storeControlData=dataCenterCB;
    sendControlData=sensorNetCB;
    return 0;
}

void talkWithAppServerStart(void)
{
    int count;
   socklen_t socksize=sizeof(struct sockaddr);
    while(1)
    {
        int comm_fd;
        struct sockaddr_in client_addr;
        if((comm_fd=accept(listen_fd,(struct sockaddr*)&client_addr,&socksize))>=0)
        {
            char ipstr[16];
            unsigned char buf[100];
            int length;
            int   i;
            inet_ntop(AF_INET,&client_addr.sin_addr.s_addr,ipstr,16);
            printf("A new connection come on:%s\n",ipstr);
            //获取数据包起始字节0xfc
            while(1)
            {
                count=read(comm_fd,buf,1);
                if(count<=0)
                {
                    perror("Net error:");
                    break;
                }
                if(buf[0]==0xfc)
                 {
                    read(comm_fd,buf,1);
                    read(comm_fd,buf+1,1);
                    length=buf[1];
                    i=2;
                    for(;length>0;length--)
                    {
                        count=read(comm_fd,buf+i,length);
                        i+=count;
                        length-=count;
                    }
                    storeControlData(buf);
                    sendControlData(buf);
                }
            }
        }
    }
}

void talkWithAppServerExit(void)
{
    close( listen_fd);
}
