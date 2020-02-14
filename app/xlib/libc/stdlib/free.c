
#include <ctype.h>
#include <stdlib.h>

void free(void *ptr) {

	__asm__ __volatile__("int $0x72"::"a"(3),"d"(ptr));


}
