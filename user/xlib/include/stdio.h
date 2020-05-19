#ifndef __STDIO_H
#define __STDIO_H

#include "ctype.h"
#include "size_t.h"
#include <stdarg.h>


#define NULL ((void *)0)

#define EOF (-1)
#define _IOFBF		0
#define _IOLBF		1
#define _IONBF		2

#define BUFSIZ		65536 //8192

#define FOPEN_MAX	16
#define FILENAME_MAX	256

#define SEEK_SET	0
#define SEEK_CUR	1
#define SEEK_END	2


/*
 *
 * Flags
 * 0x2  =stdin
 * 0x3  =stdout
 * 0x4  =stderr 
 * 0x10 = Leitura
 * 0x20 = Arquivo
 * 0x40 = Pronto
 * 0x80 = File null
 * 0x88 = Spin lock
 * 	
 */

typedef struct FILE {
	short level;		// Nível do buffer cheio/vazio
	unsigned flags;		// Sinalizadores de status
	char fd;		// Descritor de arquivo
	unsigned char hold;	// Caractere ungetc se não existir um buffer
	unsigned bsize;		// Tamanho do buffer
	unsigned char *buffer;	// Buffer de transferência
	unsigned char *curp;	// Ponteiro activo atual
	unsigned istemp;	// Indicador de arquivo temporário
	short token;		// Usado para verificação de validade
	//
	unsigned char mode[4];
	//
	char fname[256];	// Nome do arquivo aberto
	unsigned size;		// Tamanho do arquivo
	unsigned int off;	// Deslocamento
	unsigned int off2;
	//
	unsigned bps;		// Bytes por sector
	unsigned tnb;		// Total numero de blocos 
	unsigned count;		// Numero de sectores por bloco
	unsigned devn;		// Numero do dispositivo
	unsigned char *superblock;
	unsigned char *root;	//
	unsigned d_entry;	// Entrada no directorio  
	unsigned fs_type;	// Tipo de sistema de arquivo  

	unsigned int *table;	// 1024x1024
	
}__attribute__ ((packed)) FILE;



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

extern long int ftell(FILE *fp);


extern int vfprintf(FILE * fp,const char * fmt, va_list ap);
extern int fprintf(FILE *fp, const char *fmt, ...);


// E/S de string
extern int vsprintf(char * str,const char * fmt, va_list ap);
extern int sprintf(char * str, const char * fmt, ...);

extern int snprintf(char *s, size_t n,const char * fmt, ...);
extern int vsnprintf(char *s, size_t n,const char *fmt, va_list ap);


// E/S pelo console
extern int putchar(int c);
extern void puts(const char* str);
extern int printf(const char *format,...);



// FIXME sem suporte
extern int sscanf(const char * restrict s,const char * restrict format, ...);
extern void perror(const char *s);



#endif
