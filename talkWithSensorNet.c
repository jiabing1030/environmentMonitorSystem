#include "talkWithSensorNet.h"
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/ioctl.h>
static int fd;
static   unsigned char receive_buffer[100];//用来保存接收的完整数据包
static void *receiveThread(void *arg);
/*模块的初始化：
(1)串口的初始化；
(2)开启串口接收线程
*/
pthread_t tWSNInit(void (*handlerDataFunc)(unsigned char *data))
{
    pthread_t tid;
    struct termios uart_cfg;
    fd=open("/dev/ttyUSB0",O_RDWR);
    if(fd<0)
    {
        perror("Failed to open serial:");
        return -1;
    }
   cfmakeraw(&uart_cfg);
    cfsetspeed(&uart_cfg,B115200);
    uart_cfg.c_cc[VTIME]=0;
    uart_cfg.c_cc[VMIN]=1;
    tcflush(fd,TCIOFLUSH);
    tcsetattr(fd,TCSANOW,&uart_cfg);
    pthread_create(&tid,NULL,receiveThread,handlerDataFunc);
    return tid;
}

/*
 模块退出时做些善后工作
 */
void tWSNExit(void)
{
    close(fd);
}


/*
 把buf地址开始的length个字节通过串口发送给协调器
*/
void tWSNSendData(unsigned char *buf,int length)
{
    write(fd,buf,length);//不要书写成write(fd,buf,strlen(buf));
}

/*
 接收线程入口函数
*/
void *receiveThread(void *arg)
{
    void (*pFunc)(unsigned char *) = ( void (*)(unsigned char *))arg;
    while(1)
    {
        unsigned char tmp;
        int i=0;
        int length;
        int ret;
        ret=read(fd,&tmp,1);
        printf("tmp=0x%02x\n",tmp);
        if(ret<0)
        {
            perror("error:");
            continue;
        }
        if(tmp==0xfc)
        {
            read(fd,&tmp,1);
                    printf("tmp=0x%02x\n",tmp);
            receive_buffer[i++]=tmp;
            read(fd,&tmp,1);
                    printf("tmp=0x%02x\n",tmp);
            receive_buffer[i++]=tmp;
            length=tmp;
            for(;length>0;length--)
            {
                read(fd,&tmp,1);
                        printf("tmp=0x%02x\n",tmp);
                receive_buffer[i++]=tmp;
            }
            pFunc(receive_buffer);
        }
    }
}
