#include <sys/sys.h>
#include <string.h>

#include <stdio.h>


#define EOF (-1)

int _putc (int ch, FILE *fp) 
{
	unsigned char* buffer = (unsigned char*) (fp->header.buffer);
	unsigned dev_n = fp->header.dev;
	unsigned count = fp->header.count;
	unsigned int offset = fp->header.offset;
	unsigned int *block	= (unsigned int*)(fp->block);
	unsigned int bps = fp->header.bps;
	unsigned int lba_start;
	int block_count;
	int i;
	
	// error
	if(fp->header.mode[0] == '\0') return EOF;

	// std
	if ((fp->header.mode[0] == 's') \
	&& (fp->header.mode[1] == 't') && (fp->header.mode[2] == 'd')) {
 

		if(offset >= 65536) {

			//setmem((unsigned char*)(buffer),65536,0);
			fp->header.offset = 0;
			fp->header.offset2 = 0;
			return EOF;

		}

		// write character
		*(unsigned char*)(buffer + offset) = ch;

		// Update offset
		offset = fp->header.offset +=1;
		if(fp->header.offset >fp->header.size)fp->header.size +=1;
	
		return  (ch&0xff); // successfull
	}


	// Stream de arquivo

	if ((fp->header.mode[0] == 'w') \
	|| (fp->header.mode[0] == 'a') || (fp->header.mode[1] == '+')) {

		if(((fp->header.mode[1] != 'b') || (fp->header.mode[2] != 'b')) && (ch == 0)) return ch;


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
					if(block_read(dev_n,count,lba_start,buffer)) return EOF;

				}

			}

			// bit 0x10 primeira eitura
			// bit 0x20 perminsao de leitura
			fp->header.flag |= 0x30;

		}



		if(fp->header.size >= (fp->header.blocks*bps*count)) {

			// new block // FAT
			if(AddFAT(fp,(FAT_BPB *)fp->header.bpb,fp->block[fp->header.blocks-1]))
				return EOF;

 
		}

		if(fp->header.offset2 >= 65536) {

			// flush()
			fp->header.offset2 = 0;			
			memset(buffer,0,0x10000);

			fp->header.flag &= 0x0f;

		}
		
		// write character
		*(unsigned char*)(buffer + fp->header.offset2) = ch;
		// Update offset
		fp->header.offset +=1;
		fp->header.offset2 +=1;

		if(fp->header.offset > fp->header.size )fp->header.size += 1;

		return  (ch&0xff); // successfull

	}


	return EOF;
}
