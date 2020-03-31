#ifndef __STRING_H
#define __STRING_H

#include "size_t.h"
#define NULL ((void *)0)

extern size_t strlen(const char *s);

extern void *memset(void *s, char val, size_t count);

extern char *strcpy(char* s1, const char* s2);

extern int strncpy(char *s1,const char *s2,size_t count);

extern int strcmp (const char* s1, const char* s2);

extern int strncmp (const char* s1,const char* s2,size_t count);

extern void *memcpy(void *dest,void *src, size_t n);

extern char *strcat (char *dest, const char *src);



#endif
