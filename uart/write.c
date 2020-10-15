/************************************************************************
 * copyright (c) 2016-2020 private code
   File:       write.c
 * Author:     
 * Description: serial send function.
 * History:    2019-11-26 09:51
 ************************************************************************/
#include <unistd.h>
#include <sys/types.h>

/********************************************************************
 * FuncName:safe_write
 * FuncDesc:send data to serial count byte.
 * Input   :fd, buf, count
 * Output  :none
 * Return  :write data count
 * Date    :2019-11-26
 * Modify  :create
 ********************************************************************/
static ssize_t safe_write(int fd, const void *buf, size_t count)
{
	ssize_t n;

	for (;;) {
		n = write(fd, buf, count);
		if (n >= 0)
			break;
	}

	return n;
}

/********************************************************************
 * FuncName:serial_write
 * FuncDesc:send data to serial len byte.
 * Input   :fd, buf, len
 * Output  :none
 * Return  :write data len
 * Date    :2019-11-26
 * Modify  :create
 ********************************************************************/
ssize_t serial_write(int fd, const void *buf, size_t len)
{
	ssize_t cc;
	ssize_t total;

	total = 0;

	while (len) {
		cc = safe_write(fd, buf, len);

		if (cc < 0) {
			if (total) {
				/* we already wrote some! */
				/* user can do another write to know the error code */
				return total;
			}
			return cc;  /* write() returns -1 on failure. */
		}

		total += cc;
		buf = ((const char *)buf) + cc;
		len -= cc;
	}

	return total;
}

