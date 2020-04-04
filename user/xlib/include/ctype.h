#ifndef __CTYPE_H__
#define __CTYPE_H__

/*int isalnum(int c);
int isalpha(int c);
int isblank(int c);
int iscntrl(int c);
int isdigit(int c);
int isgraph(int c);
int islower(int c);
int isprint(int c);
int ispunct(int c);
int isspace(int c);
int isupper(int c);
int isxdigit(int c);*/
int tolower(int c);
int toupper(int c);


#define _U	0x01	/* upper */
#define _L	0x02	/* lower */
#define _D	0x04	/* digit */
#define _C	0x08	/* cntrl */
#define _P	0x10	/* punct */
#define _S	0x20	/* white space (space/lf/tab) */
#define _X	0x40	/* hex digit */
#define _SP	0x80	/* hard space (0x20) */

extern unsigned char _ctype[];
extern char _ctmp;

#define isalnum(c) ((_ctype+1)[c]&(_U|_L|_D))
#define isalpha(c) ((_ctype+1)[c]&(_U|_L))
#define iscntrl(c) ((_ctype+1)[c]&(_C))
#define isdigit(c) ((_ctype+1)[c]&(_D))
#define isgraph(c) ((_ctype+1)[c]&(_P|_U|_L|_D))
#define islower(c) ((_ctype+1)[c]&(_L))
#define isprint(c) ((_ctype+1)[c]&(_P|_U|_L|_D|_SP))
#define ispunct(c) ((_ctype+1)[c]&(_P))
#define isspace(c) ((_ctype+1)[c]&(_S))
#define isupper(c) ((_ctype+1)[c]&(_U))
#define isxdigit(c) ((_ctype+1)[c]&(_D|_X))

#define isascii(c) (((unsigned) c)<=0x7f)
#define toascii(c) (((unsigned) c)&0x7f)

/*#define tolower(c) (_ctmp=c,isupper(_ctmp)?_ctmp+('a'+'A'):_ctmp)
#define toupper(c) (_ctmp=c,islower(_ctmp)?_ctmp+('A'-'a'):_ctmp)*/























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
