#include <stdio.h>
#include <sys/sys.h>


int std_putc_r (int c,FILE *fp)
{
	if(fp->level >= fp->bsize || fp->off >= fp->bsize) {

		fp->off = fp->off2 = 0;
		fp->level = 0;
	}

	if(fp->flags == 4/*stderr*/)  { 
			std_putc_r (c,stdout);
	}

	fp->curp = (unsigned char*)(fp->buffer + fp->off);
	*(unsigned char*)(fp->curp) = c &0xff;

	// Update offset
	fp->off++;

	if(fp->off > fp->size ) fp->size++;
	
	return (c);

}

int putc_r (int c,FILE *fp)
{
	if(!fp) return (EOF);
	if(fp->mode[0] == '\0') return EOF;

	unsigned int *block;
	unsigned sector_start;
	int block_idex;
	int table_idex;
	int r;

	// std
	if ((fp->mode[0] == 's') \
	&& (fp->mode[1] == 't') && (fp->mode[2] == 'd')) return std_putc_r (c,fp);



	if(!fp->tnb) return EOF;

	if( (fp->mode[0] != 'w') && (fp->mode[0] != 'a') && (fp->mode[1] != '+') ) 
		return EOF;

	if( fp->size >= (fp->tnb * fp->bsize) && !(fp->flags & 0x80) ) {
		// new block // FAT
		if( addfat(fp) ) return EOF;
	}

	if( fp->flags & 0x10 ) { // primeira operação, buffer cheio e ou overflow 

		fp->flags &=~0x10;
		fp->level = fp->off % fp->bsize;

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

		// read
		if( block_read(fp->devn,fp->count, sector_start,fp->buffer) ) 
			return (EOF);

		fp->flags |= 0x40; // pronto
	}


	
	fp->curp = (unsigned char*)(fp->buffer + fp->level++);
	*(unsigned char*)(fp->curp) = c &0xff;

	// update
	if(fp->level >= fp->bsize) {

		flush_r(fp);

		fp->flags |= 0x10;
	}
 
	fp->off++;

	if(fp->off > fp->size ) fp->size++;

	return (c);
}
