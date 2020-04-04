#ifndef __STDLIB_H
#define __STDLIB_H

#include "size_t.h"

#define NULL ((void *)0)

typedef short unsigned int wchar_t;

typedef struct {
  int quot;
  int rem;
} div_t;


typedef struct {
  long int quot;
  long int rem;
} ldiv_t;


typedef struct {
  long long int quot;
  long long int rem;
} lldiv_t;


#define	EXIT_FAILURE 	1
#define	EXIT_SUCCESS 	0
#define	RAND_MAX	2147483647
#define	MB_CUR_MAX	1

extern void *malloc(size_t size);
extern void *calloc(size_t nmemb, size_t size);
extern void free(void *ptr);
extern void *realloc(void *ptr, size_t size);


extern div_t div(int numer, int denom);
extern ldiv_t ldiv(long numer, long denom);
extern lldiv_t lldiv(long long numer,long long denom);


extern int strtol (const char *nptr,char **endptr,int base);
extern int atoi (const char *nptr);



extern char *itoa (int val,char *str);
extern void i2hex(unsigned int val, char* dest, int len);

extern void exit(int rc);
extern char *getenv(const char *name);

// FIXME, nao funcionam
extern void qsort(void *base, size_t nmemb, size_t size,int (*compar)(const void *, const void *));
extern	unsigned long int strtoul( const char * restrict nptr, char ** restrict endptr, int base);



#endif
