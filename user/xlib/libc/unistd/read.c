
#include <sys/sys.h>
#include <unistd.h>
#include <stdio.h>


ssize_t read(int fd, const void *buf, size_t count)
{

	printf("read( ..., ..., ...)");
	for(;;);
	return (0);

}



