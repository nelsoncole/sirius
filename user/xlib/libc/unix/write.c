 
#include <sys/sys.h>
#include <unistd.h>

extern unsigned int stream[3];

/*
	0 = stdin
	1 = staout
	2 = stderr


*/

ssize_t write(int fd, const void *buf, size_t count)
{
	if(fd > 2) return 0;

	return (c_write(buf,1,count,(FILE*)stream[fd]));

}
