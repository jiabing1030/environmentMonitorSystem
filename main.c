#include <stdio.h>
#include "talkWithSensorNet.h"
#include <pthread.h>
void testFunc(unsigned char *data)
{
    int i;
    static count=1;
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
//此处可以添加相关模块的测试代码
    pthread_t tid;
    tid= tWSNInit(testFunc);
    printf("Begin to test:\n");
    pthread_join(tid,NULL);
    return 0;
}

