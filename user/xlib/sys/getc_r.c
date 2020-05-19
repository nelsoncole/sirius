#include <stdio.h>
#include <sys/sys.h>

extern int std_putc_r (int c,FILE *fp);
int std_getc_r (FILE *fp)
{
	int c;

	if(fp->flags == 2/*stdin*/) {

		fp->off = fp->off2  = 0;

		while(!fp->off);

		fp->curp = (unsigned char*)(fp->buffer + fp->off2);
		c = *(unsigned char*)(fp->curp) &0xff;

		// display console	
		if(c != '\b') std_putc_r(c,stdout);
		else {
			
		
		}

	}else  { //stdout stderr

		if(!fp->off) return EOF;

		if(fp->off2 >= fp->off ) return EOF;

		fp->curp = (unsigned char*)(fp->buffer + fp->off2);
		c = *(unsigned char*)(fp->curp) &0xff;

		fp->off2++;

	}

	return (c&0xff); // successfull
}

int getc_r (FILE *fp)
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
	&& (fp->mode[1] == 't') && (fp->mode[2] == 'd')) return std_getc_r (fp);

	
	// SE FINAL DO ARQUIVO, leu todos os bytes
	if(fp->off >= fp->size ) return EOF;

	
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
	r = *(unsigned char*)(fp->curp) &0xff;

	// update
	if(fp->level >= fp->bsize) {
		fp->flags |= 0x10;
	}
	fp->off++;

	return (r);
}
