/************************************************************************
 * copyright (c) 2016-2020 private code
 * File:       read.c
 * Author:     
 * Description: read serial function.
 * History:    2019-11-26 09:44
 ************************************************************************/
#include <unistd.h>
#include <sys/types.h>

/********************************************************************
 * FuncName:safe_read
 * FuncDesc:read count byte from serial.
 * Input   :fd, buf, count
 * Output  :none
 * Return  : read data len
 * Date    :2019-11-26
 * Modify  :create
 ********************************************************************/
static ssize_t safe_read(int fd, void *buf, ssize_t count)
{
	ssize_t n;

	do {
		n = read(fd, buf, count);
	} while (n < 0);

	return n;
}
/********************************************************************
 * FuncName:serial_read
 * FuncDesc:read len byte from serial.
 * Input   :fd, buf, count
 * Output  :none
 * Return  : read data len
 * Date    :2019-11-26
 * Modify  :create
 ********************************************************************/
ssize_t serial_read(int fd, void *buf, ssize_t len)
{
	ssize_t cc;
	ssize_t total;
	ssize_t times;

	times = 0;
	total = 0;

	while (len) {
		cc = safe_read(fd, buf, len);

		if (cc < 0) {
			if (total) {
				/* we already have some! */
				/* user can do another read to know the error code */
				return total;
			}
			return cc; /* read() returns -1 on failure. */
		}
		if (cc == 0 && times >= 100)
		{
			break;
		}else
		{
			if(cc != 0)
			{
				times = 0;
			}else
			{
				times++;
				usleep(1000);
				continue;
			}
		}
		buf = ((char *)buf) + cc;
		total += cc;
		len -= cc;
	}

	return total;
}

