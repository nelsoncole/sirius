#ifndef __STDIO_H__
#define __STDIO_H__

#define EOF		0xFFFFFFFF
#define FOPEN_MAX	0
#define SEEK_SET	0
#define SEEK_CUR	1
#define SEEK_END	2

#include "ctype.h"
extern FILE *stdin;
extern FILE *stdout;
extern FILE *stderr;

FILE *fopen(const char *filename,const char *mode);
int fclose (FILE *fp);
int fflush(FILE *fp);
int putc (int ch, FILE *fp);
int getc (FILE *fp);

void rewind(FILE *fp);

size_t write (const void *buffer,size_t num_bytes, size_t count, FILE *fp);
size_t read (void *buffer,size_t num_bytes, size_t count, FILE *fp);


// E/S pelo console
int putchar(int c);
void puts(const char* str);


#endif
