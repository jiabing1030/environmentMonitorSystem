#ifndef TALKWITHAPPSERVER_H
#define TALKWITHAPPSERVER_H
int talkwithAppServerInit(void (*dataCenterCB)(unsigned char *data),void (*sensorNetCB)(unsigned char *data));
void talkWithAppServerExit(void);
void talkWithAppServerStart(void);
#endif // TALKWITHAPPSERVER_H
