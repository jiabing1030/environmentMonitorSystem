#include <stdio.h>
#include "talkWithSensorNet.h"
#include <pthread.h>
#include <talkwithdatacenter.h>
#include <talkWithAppServer.h>
void testFunc(unsigned char *data)
{
    int i;
    static int count=1;
    int length=data[1]+2;
    printf("Receive a new packet:count=%d\n",count++);
    for(i=0;i<length;i++)
    {
        printf("0x%02x  ",data[i]);
    }
    printf("\n\n");
}

int main(void)
{
    pthread_t tid;
    printf("Init data-center communication module:\n");
   dataCenterInit();
   printf("Init sensor-net communication  module:\n");
   tid= tWSNInit(dataCenterStore);
   printf("Init web-server communication  module:\n");
   talkwithAppServerInit(dataCenterStore,tWSNSendData);
   printf("Start to talk with app-server:\n");
   talkWithAppServerStart();
   pthread_join(tid,NULL);
    return 0;
}

