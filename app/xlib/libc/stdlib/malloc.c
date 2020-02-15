

#include <stdlib.h>

void *malloc(size_t size) {

	unsigned int rc;

	__asm__ __volatile__("int $0x72":"=a"(rc):"a"(2),"d"(size));

	return (void*)(rc);


}
