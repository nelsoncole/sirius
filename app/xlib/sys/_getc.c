#include <sys/sys.h>

#define EOF (-1)

// Standard streams.
extern FILE *stdout;

int _getc (FILE *fp)
{
	unsigned char* buffer = (unsigned char*) (fp->header.buffer);
	unsigned dev_n = fp->header.dev;
	unsigned count = fp->header.count;
	unsigned count2 = 0;
	unsigned int *block	= (unsigned int*)(fp->block);
	unsigned int bps = fp->header.bps;
	unsigned int lba_start;
	int block_count;
	int i;
	int ch;
	
	// error
	if(fp->header.mode[0] == '\0') return EOF;

	if(fp->header.flag&0x80)
	{
		// Arquivo null
		if(!fp->header.blocks) return EOF;

		// first operation
		if(!(fp->header.flag&0x10)) {

			// ler primeiros 64 KiB e manter em cache


			
			if(fp->header.size >= 65536 ) {

				count2 = 8;
 
				block_count = 0x10000/(bps*count2);

			}
			else {

				count2 = 1;
				block_count = fp->header.size/(bps*count2);
			}

			
			for(i = 0;i< block_count;i++) { // 64 KiB

				lba_start = block[0] + (fp->header.offset/bps);
				if(block_read(dev_n,count2,lba_start,buffer+(count2*bps*i))) return EOF;

			}
				
					
			// bit 0x10 primeira eitura
			// bit 0x20 perminsao de leitura
			fp->header.flag |= 0x30;


		}

	

		goto _default;


	}

	
	// std
	if ((fp->header.mode[0] == 's') \
	&& (fp->header.mode[1] == 't') && (fp->header.mode[2] == 'd')) {

		if(fp->header.attr == 2/*stdin*/) {
			fp->header.offset = 0;
			fp->header.offset2 = 0;

			while(!fp->header.offset);

			ch = *(unsigned char*)(buffer + fp->header.offset2);

			// display console	
			if(ch != '\b')_putc(ch,stdout);
			else {
			
		
			}
		}else  { //stdout stderr

			if(!fp->header.offset)return (EOF);
			if((fp->header.offset2 >= fp->header.offset ))return (EOF);

			ch = *(unsigned char*)(buffer + fp->header.offset2);

			fp->header.offset2++;


		}

		return ch; // successfull
	}



	if(!fp->header.blocks) { 
		return EOF;
	}

	// primeira operacao
	if(!(fp->header.flag&0x10)){
		if(fp->header.size) {

			// ler os primeiros 64KiB ou < 64KiB
			if(fp->header.size >= 65536 ) {
				block_count = 65536 /(bps*count);
					
			} else {
				block_count = fp->header.size/(bps*count);
				if(fp->header.size%(bps*count)) block_count++;

			}

		
			for(i = 0;i<block_count;i++) {

				lba_start = block[(fp->header.offset/bps/count) + i] + (fp->header.offset/bps%count);
				if(block_read(dev_n,count,lba_start,buffer + (count*bps*i))) return EOF;

			}

		}

		// bit 0x10 primeira eitura
		// bit 0x20 perminsao de leitura
		fp->header.flag |= 0x30;

	}


_default:
	// checar perminsao de leitura
	if(!(fp->header.flag&0x20))return EOF; 

	if(fp->header.offset >= fp->header.size && (!fp->header.flag)) {
		// FINAL DO ARQUIVO, leu todos os bytes
		return EOF;

	}

	ch = *(unsigned char*)(buffer + fp->header.offset2);
	// Update offset
	fp->header.offset +=1;
	fp->header.offset2 +=1;

	return (ch&0xff); // successfull
}

