#include <sys/sys.h>
#include <string.h>

#include <stdio.h>


#define EOF (-1)


int _putc (int ch, FILE *fp) 
{

	if(!fp) return EOF;

	unsigned char* buffer = (unsigned char*) (fp->header.buffer);
	unsigned dev_n = fp->header.dev;
	unsigned count = fp->header.count;
	unsigned count2 = 0;
	unsigned int *block	= (unsigned int*)(fp->block);
	unsigned int bps = fp->header.bps;
	unsigned int lba_start;
	int block_count;
	int i;

	unsigned int start = 0;
	unsigned int offset = 0;
	unsigned int offset2 = 0;
	
	// error
	if(fp->header.mode[0] == '\0') return EOF;

	// std
	if ((fp->header.mode[0] == 's') \
	&& (fp->header.mode[1] == 't') && (fp->header.mode[2] == 'd')) {
 

		if(fp->header.offset >= 65536) {

			//setmem((unsigned char*)(buffer),65536,0);
			fp->header.offset = 0;
			fp->header.offset2 = 0;
			return EOF;

		}

		// write character
		*(unsigned char*)(buffer + fp->header.offset) = ch;

		// Update offset
		fp->header.offset +=1;
		if(fp->header.offset >fp->header.size)fp->header.size +=1;
	
		return  (ch&0xff); // successfull
	}


	// Stream de arquivo

	offset = fp->header.offset/0x10000;
	offset2 = fp->header.offset%0x10000;

	if(fp->header.flag&0x80)
	{
		// Arquivo null
		if(!fp->header.blocks) return EOF;

		if ( ( (fp->header.mode[0] != 'w') && (fp->header.mode[0] != 'a') \
		&& (fp->header.mode[1] != '+') ) ) return EOF;


		if(((offset2 == 0) && (fp->header.offset != 0)) || (!(fp->header.flag&0x10))) 
		{	//FIXME cache cheio

			fp->header.flag &=~0x20;

			// chamar flush()
			// ler blocos e armazenar em cache
			if(fp->header.flag&0x10) flush(fp);

			memset(buffer,0,0x10000);

			if( (fp->header.size > ((0x10000*offset)+0x10000) ) \
			|| ( (fp->header.size != 0) && (!(fp->header.flag&0x10))) ) // ler
			{

				offset = fp->header.offset/0x10000;

				if( ( (fp->header.size >= (0x10000*offset)) && (offset != 0) ) || \
				( (!fp->header.offset) && (fp->header.size >= 0x10000) ) || \
				( (fp->header.offset != 0) && (fp->header.size >= (0x10000+fp->header.offset)) ) ) {

					count2 = 8;
					block_count = 0x10000 /(bps*count2);
					
				} else {

					count2 = 1;
					block_count = fp->header.size/(bps*count2);

					if(fp->header.size%(bps*count2) ) block_count++;

					block_count -= ((0x10000/(bps*count2))*(offset));

				}

				start = (offset*(0x10000/(count2*bps)));	

				for(i = 0;i<block_count;i++) {

					lba_start = block[0] + (count2*i) + start;
					if(block_read(dev_n,count2,lba_start,buffer+(count2*bps*i))) return EOF;
				}

			}

			// bit 0x10 primeira eitura
			// bit 0x20 permissao para leitura
			fp->header.flag |= 0x30;

		}


		//printf("write\n");
		// write character
		*(unsigned char*)(buffer + offset2) = ch;

		// Update offset
		fp->header.offset +=1;

		if(fp->header.offset > fp->header.size )fp->header.size += 1;

		return  (ch&0xff); // successfull


	}




	if ((fp->header.mode[0] == 'w') \
	|| (fp->header.mode[0] == 'a') || (fp->header.mode[1] == '+')) {

		if(((fp->header.mode[1] != 'b') || (fp->header.mode[2] != 'b')) && (ch == 0)) return ch;


		if(!fp->header.blocks) { 
			return EOF;
		}


		
		if(fp->header.size >= (fp->header.blocks*bps*count)) {

			// new block // FAT
			if(AddFAT(fp,(FAT_BPB *)fp->header.bpb,fp->block[fp->header.blocks-1])) {
				return EOF;

			}
 
		}

		if(((offset2 == 0) && (fp->header.offset != 0)) || (!(fp->header.flag&0x10))) 
		{	//FIXME cache cheio

			fp->header.flag &=~0x20;
			// chamar flush()
			// ler blocos e armazenar em cache
			if(fp->header.flag&0x10) flush(fp);

			memset(buffer,0,0x10000);

			if( (fp->header.size > ((0x10000*offset)+0x10000) ) \
			|| ( (fp->header.size != 0) && (!(fp->header.flag&0x10))) ) // ler
			{
				offset = fp->header.offset/0x10000;

				if( ( (fp->header.size >= (0x10000*offset)) && (offset != 0) ) || \
				((!fp->header.offset) && (fp->header.size >= 0x10000) ) ) {

					block_count = 0x10000 /(bps*count);
					
				} else {

					block_count = fp->header.size/(bps*count);

					if(fp->header.size%(bps*count) ) block_count++;

					block_count -= ((0x10000/(bps*count))*(offset));

				}

				start = (offset*(0x10000/(count*bps)));				
			
				for(i = 0;i<block_count;i++) {

					lba_start = block[i + start];
					if(block_read(dev_n,count,lba_start,buffer+(count*bps*i))) return EOF;

				}


			}


			// bit 0x10 primeira eitura
			// bit 0x20 permissao para leitura
			fp->header.flag |= 0x30;

		}
		
		// write character
		*(unsigned char*)(buffer + offset2) = ch;
		// Update offset
		fp->header.offset += 1;
		if(fp->header.offset > fp->header.size )fp->header.size += 1;


		return  (ch&0xff); // successfull

	}


	return EOF;
}
