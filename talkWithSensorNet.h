#ifndef TALKWITHSENSORNET_H
#define TALKWITHSENSORNET_H
#include <pthread.h>
pthread_t tWSNInit(void (*handlerDataFunc)(unsigned char *data));
void tWSNExit(void);
void tWSNSendData(unsigned char *buf);
#endif // TALKWITHSENSORNET_H
