#include "ManiFold_UART1.hpp"

bool WaitFlag = FALSE;                           //等待接收标志，信号法读取串口使用
bool STOP = 0;                                   //停止接收标志，信号法读取串口使用

int SetOpt(int fd,int nSpeed, int nBits, char nEvent, int nStop)
{
    struct termios newtio,oldtio;
    if  ( tcgetattr( fd,&oldtio)  !=  0) 
    { 
        perror("SetupSerial 1");
        return -1;
    }
    bzero( &newtio, sizeof( newtio ) );
    newtio.c_cflag  |=  CLOCAL | CREAD; 
    newtio.c_cflag &= ~CSIZE; 
	
	//数据位
    switch( nBits )
    {
    case 7:
        newtio.c_cflag |= CS7;
        break;
    case 8:
        newtio.c_cflag |= CS8;
        break;
    }
	
	//校验
    switch( nEvent )
    {
    case 'o':
    case 'O':                     //奇校验
        newtio.c_cflag |= PARENB;
        newtio.c_cflag |= PARODD;
        newtio.c_iflag |= (INPCK | ISTRIP);
        break;
    case 'e':
    case 'E':                     //偶校验
        newtio.c_iflag |= (INPCK | ISTRIP);
        newtio.c_cflag |= PARENB;
        newtio.c_cflag &= ~PARODD;
        break;
    case 'n':
    case 'N':                    //无校验
        newtio.c_cflag &= ~PARENB;
        break;
    }
	
	//波特率
	switch( nSpeed )
    {
    case 300:
        cfsetispeed(&newtio, B300);
        cfsetospeed(&newtio, B300);
        break;
    case 1200:
		cfsetispeed(&newtio, B1200);
		cfsetospeed(&newtio, B1200);
		break;
    case 2400:
        cfsetispeed(&newtio, B2400);
        cfsetospeed(&newtio, B2400);
        break;
    case 4800:
        cfsetispeed(&newtio, B4800);
        cfsetospeed(&newtio, B4800);
        break;
    case 9600:
        cfsetispeed(&newtio, B9600);
        cfsetospeed(&newtio, B9600);
        break;
    case 19200:
        cfsetispeed(&newtio, B19200);
        cfsetospeed(&newtio, B19200);
        break;
    case 38400:
        cfsetispeed(&newtio, B38400);
        cfsetospeed(&newtio, B38400);
        break;
    case 57600:
        cfsetispeed(&newtio, B57600);
        cfsetospeed(&newtio, B57600);
        break;
    case 115200:
        cfsetispeed(&newtio, B115200);
        cfsetospeed(&newtio, B115200);
        break;
    default:
        cfsetispeed(&newtio, B9600);
        cfsetospeed(&newtio, B9600);
        break;
    }
    
    //停止位
    if( nStop == 1 )
    {
        newtio.c_cflag &=  ~CSTOPB;
    }
    else if ( nStop == 2 )
    {
        newtio.c_cflag |=  CSTOPB;
    }
    newtio.c_cc[VTIME]  = 0;
    newtio.c_cc[VMIN] = 0;
    tcflush(fd,TCIFLUSH);
    if((tcsetattr(fd,TCSANOW,&newtio))!=0)
    {
        perror("Serial set error");
        return -1;
    }
    cout<<"Serial set done!"<<endl;
    return 0;
}

int OpenSerial(int fd,int comport)
{
    /*
    * DJI ManiFold UART1 /dev/ttyTHS0  RXD 13 TXD 15 GND 16
    * DJI ManiFold UART2 /dev/ttyTHS1  RXD 6  TXD 5  GND 4
    * DJI ManiFold UART3 /dev/ttyTHS2  RXD 2  TXD 3  GND 4
    * DJI ManiFold UART4 /dev/ttyS0     内核控制台
    */
    if (comport==1)
    {    
    	//程序刚刚启动的时候，0是标准输入，1是标准输出，2是标准错误。如果此时去打开一个新的文件，它的文件描述符会是3
    	//执行完open()后，fd = 3;
    	//不控制 不睡眠 可读可写 
    	fd = open( "/dev/ttyTHS0", O_RDWR|O_NOCTTY|O_NDELAY); 
        if (fd == -1)
        {
            perror("Can't Open Serial Port");
            return(-1);
        }
        else 
        {
        	cout<<"open UART1 ......"<<endl;
        }
    }
    else if(comport==2)
    {    
    	fd = open( "/dev/ttyTHS1", O_RDWR|O_NOCTTY|O_NDELAY);
        if (fd == -1)
        {
            perror("Can't Open Serial Port");
            return(-1);
        }
        else 
        {
            cout<<"open UART2 ......"<<endl;
        }    
    }
    else if (comport==3)
    {
        fd = open( "/dev/ttyTHS2", O_RDWR|O_NOCTTY|O_NDELAY);
        if (fd == -1)
        {
            perror("Can't Open Serial Port");
            return(-1);
        }
        else 
        {
            cout<<"open UART3 ......"<<endl;
        }
    }
    else if (comport==4)
    {
        fd = open( "/dev/ttyS0", O_RDWR|O_NOCTTY|O_NDELAY);
        if (fd == -1)
        {
            perror("Can't Open Serial Port");
            return(-1);
        }
        else 
        {
            cout<<"open UART4 ......"<<endl;
            cout<<"UART4 is system console,please be careful"<<endl;
        }
    }
    
    if(isatty(STDIN_FILENO)==0)
    {
    	cout<<"standard input is not a terminal device"<<endl;
    }
    else
    {
    	//cout<<"isatty success!"<<endl;
    }
    //cout<<"fd_open = "<<fd<<endl;
    return fd;
}


void  signal_handler_IO (int status)  
{  
	cout<<"received SIGIO signale."<<endl;
	WaitFlag = FALSE;  
} 

int SerialInit(void)
{
	int fd = 0;
	struct sigaction saio; 
	
    if((fd=OpenSerial(fd,1))<0)
    {
        perror("open_serial error");
        return -1;
    }
    if((SetOpt(fd,115200,8,'N',1))<0)
    {
        perror("set_opt error");
        return -1;
    }
    
	saio.sa_handler = signal_handler_IO;  
	sigemptyset (&saio.sa_mask);  
	saio.sa_flags = SA_SIGINFO;  
	saio.sa_restorer = NULL;  
	sigaction (SIGIO, &saio, NULL);  
	fcntl(fd, F_SETOWN, getpid ());  
	fcntl(fd, F_SETFL, FASYNC);
	
	return fd;
}

int SerialRX(int fd,string &RecBuffer)
{
	int nread = 0;
	char ReceiveBuff[ReceivedDataSize]=""; 
	
	nread = read (fd, ReceiveBuff, SerialRXLenth);
	if(nread < 0)
	{
		return -1;   //error
	}
	cout<<"nread  ="<<setw(4)<<nread<<", The date received is : "<<ReceiveBuff<<endl;
	/*   信号方式读写串口，会造成写等操作阻塞
	while (!STOP)  
    {  
		// after receving SIGIO ,WaitFlag = FALSE,input is availabe and can be read,then put data to buffer
		if (!WaitFlag)   
		{  
			memset (ReceiveBuff, 0, sizeof(ReceiveBuff));  
			nread = read (fd, ReceiveBuff, 255);  
			cout<<"nread  ="<<setw(4)<<nread<<", The date received is : "<<ReceiveBuff<<endl;
			WaitFlag = TRUE;                    //wait for new input 
		}
    } 
    */
    
    //for(int i = 0;i < SerialRXLenth;i++)
    //	RecBuffer += ReceiveBuff[i];
    RecBuffer = ReceiveBuff;
    return 0;
}

int SerialTX(int fd,string SendBuffer)
{
	int nwrite = 0;
	char WriteBuff[SentDataSize] = "\0";
	for(int i = 0;i < SendBuffer.size();i++) 
		WriteBuff[i] = SendBuffer[i];
	nwrite = write(fd,WriteBuff,SerialTXLenth);
	if(nwrite < 0)
	{
		return -1; //error
	}
    cout<<"nwrite ="<<setw(4)<<nwrite<<", The date sent is : "<<WriteBuff<<endl;
    
    return 0;
}

