#include "talkwithdatacenter.h"
#include <stdio.h>
#include <mysql.h>
#include <myglobal.h>
static MYSQL com_mysql;//声明一个数据库连接句柄
void dataCenterInit(void)
{

    if(mysql_init(&com_mysql)==NULL)
    {
            printf("Cannot init mysql!\n");
            return;
    }
    if(mysql_real_connect(&com_mysql,"192.168.0.1","jiabing",
                          "1234","environment",0,NULL,0)==NULL)
    {
        printf("%s\n",mysql_error(&com_mysql));
        return ;
    }
    printf("Connect successfuly!\n");
    return;
}

void  dataCenterExit(void)
{
    mysql_close(&com_mysql);
}

/*************************
 data[0]:payload类型，0x00:控制帧，0x01:数据帧
 data[1]:payload长度，字节数为单位
 payload:
        数据帧：data[2]----------传感器编号；
        　　　　data[3]----------值；
        控制帧：data[2]----------灯编号；
        　　　　data[3]----------0x0０:关，０ｘ01:开
 * ***********************/
void dataCenterStore(unsigned char *data)
{
    int  type=data[0];
    int  number=data[2];
    int value=data[3];
    char sqlcommand[100];
    int ret;
    if(type==PACKET_TYPE_CONTROL)
    {
        //以后添加对灯控制的代码
        printf("lightId=0x%x,lightcommand=0x%x\n",data[2],data[3]);
        printf("Ready to store database!\n");
        sprintf(sqlcommand,"insert into `light_control`(`number`,`state`)  values(%d,%d)",data[2],data[3]?1:0);
        printf("%s\n",sqlcommand);
        ret=mysql_query(&com_mysql,sqlcommand);
        if(ret!=0)
            printf("%s\n",mysql_error(&com_mysql));

    }
    else if(type==PACKET_TYPE_DATA)
    {
        sprintf(sqlcommand,"insert into `sensor_data`(`number`,`value`)  values(%d,%d)",number,value);
        printf("%s\n",sqlcommand);
        ret=mysql_query(&com_mysql,sqlcommand);
        if(ret!=0)
            printf("%s\n",mysql_error(&com_mysql));
    }
    return;
}
