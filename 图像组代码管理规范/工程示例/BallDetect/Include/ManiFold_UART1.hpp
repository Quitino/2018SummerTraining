#ifndef MANIFOLD_UART1_HPP
#define MANIFOLD_UART1_HPP

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>  
#include <sys/signal.h>  
#include <string.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <iomanip>
#include <iostream>
#include <vector>


using namespace std;

#define ReceivedDataSize 512   //接收缓存大小
#define SentDataSize     512   //发送缓存大小

#define SerialRXLenth    8     //从串口读取字节数  read()
#define SerialTXLenth    72   //写入串口字节数    write()

#define FALSE  0  
#define TRUE   1  

/*
*函数名：SetOpt
*功  能：设置串口参数  
*参  数：fd 文件描述符
*		 nSpeed 波特率  默认115200
*		 nBits  数据位
*		 nEvent 校验
*		 nStop  停止位
*/
int SetOpt(int fd,int nSpeed, int nBits, char nEvent, int nStop);

/*
*函数名：OpenSerial
*功  能：打开串口 
*参  数：fd 文件描述符 
*		 comport 串口号
*/
int OpenSerial(int fd,int comport);

/*
*函数名：SerialInit
*功  能：打开串口
*参  数：无
*/
int SerialInit(void);

/*
*函数名：SerialRX
*功  能：打开串口
*参  数：fd 文件描述符 
*		 RecBuffer 接收缓存区
*/
int SerialRX(int fd,string &RecBuffer);

/*
*函数名：SerialTX
*功  能：打开串口
*参  数：fd 文件描述符 
*		 SendBuffer 要发送的字符串
*/
int SerialTX(int fd,string SendBuffer);


#endif

