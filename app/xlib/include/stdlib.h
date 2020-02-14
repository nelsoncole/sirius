#ifndef __STDLIB_H
#define __STDLIB_H


#if defined(__x86_64__) || defined(__aarch64__)
	#define	INT_MAX 0x7fffffffffffffff // -------	// max value of an "int"
	#define HEX_LEN 16
	
#elif defined(__i386__) || defined(__arm__)
	#define	INT_MAX	0x7fffffff // 2147483647	// max value of an "int"
	#define HEX_LEN 8

#else
#error YOUR ARCH HERE
#endif

#include "ctype.h"

extern void *malloc(size_t size);
extern void *colloc(size_t num, size_t size);
extern void free(void *ptr);
extern void *realloc(void *ptr, size_t size);


extern int strtol (const char *nptr,char **endptr,int base);
extern int atoi (const char *nptr);
extern void i2hex(unsigned int val, char* dest, int len);



#endif
