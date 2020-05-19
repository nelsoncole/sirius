#include <stdio.h>
#include <stdlib.h>
#include <sys/sys.h>

int flush_r(FILE *fp) 
{
	if(!fp) return (EOF);
	if(!(fp->flags & 0x40)) return (EOF);
	if(fp->flags & 0x10) return (EOF);


	unsigned int *block;
	unsigned sector_start;
	int block_idex;
	int table_idex;
	int r;

	r = fp->off / fp->bsize;
	table_idex = r / 1024;
	block_idex = r % 1024;


	if(fp->flags & 0x80) { // null file

		block= (unsigned int *) fp->table[0];
		sector_start = block[0] + (r * fp->count);

	} else {
		block= (unsigned int *) fp->table[table_idex];
		sector_start = block[block_idex];

	}



	if( block_write(fp->devn,fp->count, sector_start,fp->buffer) )
		return EOF;

	if(fp->flags & 0x80) return (0);


	// update file
	fp->root = (unsigned char *) FatOpenRoot((FAT_BPB *)fp->superblock);	
	if( updatefile(fp) ) {

		free(fp->root);
		return EOF;
	}


	free(fp->root);
	return (0);
}
