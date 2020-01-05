#ifndef __VFS_H__
#define __VFS_H__


typedef struct _VFS_FILE_HEADER 
{
	// File Header 4 KiB
	CHAR8	filename[256];
	UINT8 	attr;
	UINT32	size;
	UINT8	dev;
	UINT8	p_entry;
	UINT16	bps;
	UINT8	count;	// numeros de sector por bloco
	UINT32	blocks;
	UINT8	rsv[256 - 14];

}__attribute__ ((packed)) VFS_FILE_HEADER;

typedef struct _VFS 
{
	// File Header 4 KiB
	VFS_FILE_HEADER header;

	// LBA block start
	UINT32	block[1];

}__attribute__ ((packed)) VFS;



#endif
