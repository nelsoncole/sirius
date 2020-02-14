#ifndef __CTYPE_H__
#define __CTYPE_H__


#define TRUE 1
#define FALSE 0
#define NULL ((void *)0)

typedef unsigned char uint8_t;
typedef	unsigned short uint16_t;
typedef	unsigned int uint32_t;
typedef	unsigned long long uint64_t;
typedef unsigned size_t;

typedef struct _FILE_HEADER 
{
	// File Header 4 KiB
	unsigned char	filename[256];
	unsigned char 	attr;
	unsigned int	size;
	unsigned char	dev;
	unsigned char	p_entry;
	unsigned short	bps;
	// números de sector por bloco
	unsigned char	count;
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
	unsigned int block[1024-128];

}__attribute__ ((packed)) FILE;


#include "mbr.h"


#endif
