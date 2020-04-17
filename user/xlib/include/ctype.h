#ifndef __CTYPE_H__
#define __CTYPE_H__


/*
 **************************************
 * Credits: 
 *     Linux
 */

#define _U	0x01	/* upper */
#define _L	0x02	/* lower */
#define _D	0x04	/* digit */
#define _C	0x08	/* cntrl */
#define _P	0x10	/* punct */
#define _S	0x20	/* white space (space/lf/tab) */
#define _X	0x40	/* hex digit */
#define _SP	0x80	/* hard space (0x20) */

#define __ismask(x) (_ctype[(int)(unsigned char)(x)])

extern const unsigned char _ctype[];

#define isalnum(c) ((__ismask(c)&(_U|_L|_D)) != 0)
#define isalpha(c) ((__ismask(c)&(_U|_L)) != 0)
#define iscntrl(c) ((__ismask(c)&(_C)) != 0)
#define isdigit(c) ((__ismask(c)&(_D)) != 0)
#define isgraph(c) ((__ismask(c)&(_P|_U|_L|_D)) != 0)
#define islower(c) ((__ismask(c)&(_L)) != 0)
#define isprint(c) ((__ismask(c)&(_P|_U|_L|_D|_SP)) != 0)
#define ispunct(c) ((__ismask(c)&(_P)) != 0)
#define isspace(c) ((__ismask(c)&(_S)) != 0)
#define isupper(c) ((__ismask(c)&(_U)) != 0)
#define isxdigit(c) ((__ismask(c)&(_D|_X)) != 0)

// linux
#define isascii(c) (((unsigned char)(c))<=0x7f)
#define toascii(c) (((unsigned char)(c))&0x7f)


/*

int isblank(int c);
*/


int tolower(int c);
int toupper(int c);























// end ctype


#define TRUE 1
#define FALSE 0
#define NULL ((void *)0)


typedef unsigned size_t;

typedef struct _FILE_HEADER 
{
	// File Header 4 KiB
	unsigned char	filename[256];
	unsigned char 	attr;
	unsigned int	size;
	unsigned int 	size2;
	unsigned char	dev;
	unsigned char	p_entry;
	unsigned int	bps;
	// números de sector por bloco
	unsigned int	count;
	// número total de blocos
	unsigned int	blocks;	
	unsigned int	offset;
	unsigned int	offset2;
	unsigned int	buffer;
	// definido em libc padrão
	unsigned char	mode[4];
	unsigned char	flag;
	unsigned int    bpb; 
	struct _VFS_FILE_HEADER *current;
	struct _VFS_FILE_HEADER *next;
	unsigned char	rsvx[256 - 43];

}__attribute__ ((packed)) FILE_HEADER;

typedef struct _FILE 
{
	// File Header 512 Bytes
	FILE_HEADER header;

	// LBA block start
	unsigned int block[1/*1024-128*/];

}__attribute__ ((packed)) FILE;


#include "mbr.h"


#endif
