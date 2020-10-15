/************************************************************************
 * copyright (c) 2016-2020 private code
 * File:       interface.h
 * Author:     
 * Description: uart function file.
 * History:    2019-11-26 10:58
 ************************************************************************/
#ifndef __INTERFACE_H__
#define __INTERFACE_H__

#include <sys/types.h>

typedef enum _baudrate
{
    BAUDRATE_115200  = 0x01,		/* baudrate 115200 */
    BAUDRATE_57600   = 0x02,		/* baudrate 57600 */
    BAUDRATE_38400   = 0x03,		/* baudrate 38400 */
    BAUDRATE_19200   = 0x04,		/* baudrate 19200 */
    BAUDRATE_9600    = 0x05,		/* baudrate 9600 */
    BAUDRATE_4800    = 0x06,		/* baudrate 4800 */
    BAUDRATE_2400    = 0x07,		/* baudrate 2400 */
    BAUDRATE_DEFAULT,			/* baudrate fault */
}baudrate_e;

typedef enum _databits
{
    DATABIT_6       = 0x01,		/* data bit is 6 */
    DATABIT_7       = 0x02,		/* data bit is 7 */
    DATABIT_8       = 0x03,		/* data bit is 8 */
    DATABIT_DEFAULT,			/* data bit fault */
}databits_e;

typedef enum _parity
{
    NONE_CHECK       = 0x01,		/* do not use check */
    ODD_CHECK        = 0x02,		/* odd check */
    EVEN_CHECK       = 0x03,		/* even check */
    PARITY_DEFAULT,			/* serial parity type fault */
}parity_e;

typedef enum _stop
{
    STOP_BIT_1     = 0x01,		/* stop bit is 1 */
    STOP_BIT_2     = 0x02,		/* stop bit is 2 */
    STOP_BIT_DEFAULT,			/* stop bit fault */
}stop_e;



/********************************************************************
 * FuncName:full_init
 * FuncDesc:init ttyUSBx.
 * Input   :example ttyUSB0, ttyUSB1, ttyUSB2  
 * Output  :none
 * Return  : OK:return serial Descriptor ERR: -1
 * Date    :2019-11-26
 * Modify  :create
 ********************************************************************/
ssize_t serial_init(const char* tty_name);



/********************************************************************
 * FuncName:serial_exit
 * FuncDesc:serial safe exit when code end.
 * Input   :a valid serial Descriptor.  
 * Output  :none
 * Return  : OK: 0 
 * Date    :2019-11-26
 * Modify  :create
 ********************************************************************/
ssize_t serial_exit(int fd);



/********************************************************************
 * FuncName:serial_config
 * FuncDesc:config serial properties.
 * Input   :fd, speed, databits, stopbits, parity  
 * Output  :none
 * Return  : OK:0 ERR:-1
 * Date    :2019-11-26
 * Modify  :create
 ********************************************************************/
ssize_t serial_config(int fd, int speed, int databits, int stopbits, int parity);



/********************************************************************
 * FuncName:serial_read
 * FuncDesc:read len byte from serial.
 * Input   :fd, buf, count
 * Output  :none
 * Return  : read data len
 * Date    :2019-11-26
 * Modify  :create
 ********************************************************************/
ssize_t serial_read(int fd, void *buf, ssize_t len);



/********************************************************************
 * FuncName:serial_write
 * FuncDesc:send data to serial len byte.
 * Input   :fd, buf, len
 * Output  :none
 * Return  :write data len
 * Date    :2019-11-26
 * Modify  :create
 ********************************************************************/
ssize_t serial_write(int fd, const void *buf, size_t len);


#endif
