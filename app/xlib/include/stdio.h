#ifndef __STDIO_H
#define __STDIO_H

#define EOF (-1)

#define FOPEN_MAX	0
#define SEEK_SET	0
#define SEEK_CUR	1
#define SEEK_END	2

#include "ctype.h"

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


// string
extern char *fgets (char *str,FILE *fp);

// outras
extern size_t write (const void *buffer,size_t num_bytes, size_t count, FILE *fp);
extern size_t read (void *buffer,size_t num_bytes, size_t count, FILE *fp);

extern int remove (const char *filename);
extern void rewind(FILE *fp);


// E/S pelo console
extern int putchar(int c);
extern void puts(const char* str);
extern int printf(const char *format,...);


#endif
