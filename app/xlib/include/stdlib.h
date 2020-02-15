#ifndef __STDLIB_H
#define __STDLIB_H

#include "size_t.h"

#define NULL ((void *)0)

extern void *malloc(size_t size);
extern void *colloc(size_t num, size_t size);
extern void free(void *ptr);
extern void *realloc(void *ptr, size_t size);


extern int strtol (const char *nptr,char **endptr,int base);
extern int atoi (const char *nptr);
extern void i2hex(unsigned int val, char* dest, int len);



#endif
