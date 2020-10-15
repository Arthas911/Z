/************************************************************************
 * copyright (c) 2016-2020 private code
 * File:config.c
 * Author:     
 * Description: serial config function.
 * History:    2019-11-26 09:51
 ************************************************************************/
#include <stdio.h>
#include <sys/types.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include "interface.h"

/********************************************************************
 * FuncName:serial_config
 * FuncDesc:config serial properties.
 * Input   :fd, speed, databits, stopbits, parity  
 * Output  :none
 * Return  : OK:0 ERR:-1
 * Date    :2019-11-26
 * Modify  :create
 ********************************************************************/
ssize_t serial_config(int fd, int speed, int databits, int stopbits, int parity)  
{
	int flow_ctrl = 0;
	int i;
	int speed_arr[] = { B115200, B57600, B38400, B19200, B9600, B4800, B2400};  
	int name_arr[] =  { BAUDRATE_115200, BAUDRATE_57600, BAUDRATE_38400, BAUDRATE_19200, BAUDRATE_9600, BAUDRATE_4800, BAUDRATE_2400};  

	struct termios options;  

	if(tcgetattr(fd,&options)  !=  0)  
	{  
		return -1;   
	}  

	for (i= 0;  i < sizeof(speed_arr) / sizeof(int);  i++)  
	{  
		if(speed == name_arr[i])  
		{               
			cfsetispeed(&options, speed_arr[i]);   
			cfsetospeed(&options, speed_arr[i]);    
		}  
	}       

	options.c_cflag |= CLOCAL;  
	options.c_cflag |= CREAD;  

	switch(flow_ctrl)  
	{  

		case 0 :
			options.c_cflag &= ~CRTSCTS;  
			break;     

		case 1 :
			options.c_cflag |= CRTSCTS;  
			break;  
		case 2 :
			options.c_cflag |= IXON | IXOFF | IXANY;  
			break; 
		default:
			return -1;

	}  

	options.c_cflag &= ~CSIZE;  
	switch (databits)  
	{    
		case DATABIT_8 :  
			options.c_cflag |= CS8;  
			break;  
		case DATABIT_7 :      
			options.c_cflag |= CS7;  
			break;  
		case DATABIT_6:      
			options.c_cflag |= CS6;  
			break;
		default:     
			printf("databits = %d err\n", databits);
			return -1;
	}  

	switch (parity)  
	{    
		case NONE_CHECK :
			options.c_cflag &= ~PARENB;     
			options.c_iflag &= ~INPCK;      
			break;   
		case ODD_CHECK :
			options.c_cflag |= (PARODD | PARENB);   
			options.c_iflag |= INPCK;               
			break;   
		case EVEN_CHECK:
			options.c_cflag |= PARENB;         
			options.c_cflag &= ~PARODD;         
			options.c_iflag |= INPCK;        
			break;  
		default:    
			printf("parity = %d err \n", parity);      
			return -1;

	}   

	switch (stopbits)
	{    
		case STOP_BIT_1:
			options.c_cflag &= ~CSTOPB; 
			break;    
		case STOP_BIT_2:  
			options.c_cflag |= CSTOPB; 
			break;  
		default:     
			printf("stopbits = %d err \n", stopbits);
			return -1;
	}  

	options.c_oflag &= ~OPOST;  

	options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);


	options.c_cc[VTIME] = 0;
	options.c_cc[VMIN] = 0;

	options.c_iflag &= ~(ICRNL | IXON);

	tcflush(fd,TCIFLUSH);  

	if (tcsetattr(fd,TCSANOW,&options) != 0)    
	{ 
		printf("set tcsetattr failed %s----%d \n", __FUNCTION__, __LINE__); 
		return -1;   
	}  

	if(fcntl(fd, F_SETFL, 0) < 0)  
	{  
		printf("fcntl failed %s----%d \n", __FUNCTION__, __LINE__); 
		return -1;   
	}       
	else  
	{
		printf("config serial ok \n");  
	}
  
	return 0;   
} 
