#include <stdlib.h>

#undef        free

void free(void *ptr) {

	if(!ptr) return;
	__asm__ __volatile__("int $0x72"::"a"(3),"d"(ptr));


}
