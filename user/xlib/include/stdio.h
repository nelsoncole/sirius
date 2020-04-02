#ifndef __STDIO_H
#define __STDIO_H


#define NULL ((void *)0)

#define EOF (-1)

#define FOPEN_MAX	0
#define SEEK_SET	0
#define SEEK_CUR	1
#define SEEK_END	2

#include "ctype.h"
#include "size_t.h"



// Standard streams.
extern FILE *stdin;
extern FILE *stdout;
extern FILE *stderr;

// C89/C99 say they're macros.  Make them happy.
#define stdin stdin
#define stdout stdout
#define stderr stderr


// E/S de arquivo
extern FILE *fopen(const char *filename,const char *mode);
extern int fclose (FILE *fp);
extern int fflush(FILE *fp);

// caracters
extern int putc (int ch, FILE *fp);
extern int getc (FILE *fp);
extern int fputc (int ch, FILE *fp);
extern int fgetc (FILE *fp);


// string
extern char *fgets (char *str,int length,FILE *fp);
extern int fputs (const char *str,FILE *fp);

// outras
extern size_t fread (void *buffer, size_t size, size_t count, FILE *fp);
extern size_t fwrite (const void *buffer, size_t size, size_t count, FILE *fp);

extern int remove (const char *path);
extern void rewind(FILE *fp);

extern int fseek (FILE *fp, long num_bytes, int origin );

long int ftell(FILE *fp);


// E/S pelo console
extern int putchar(int c);
extern void puts(const char* str);
extern int printf(const char *format,...);


#endif
