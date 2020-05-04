#include <sys/sys.h>
#include <stdlib.h>
#include <stdio.h>

#define EOF (-1)

#include <stdio.h>

int flush(FILE *fp) {

	if(!fp) return EOF;

	if(fp->header.mode[0] != 'w' && fp->header.mode[1] != '+' && fp->header.mode[0] != 'a') return EOF;

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

	unsigned int var0 = 0;

	var0 = fp->header.offset;
	if(fp->header.offset)fp->header.offset--;

	// error
	if(fp->header.mode[0] == '\0') { fp->header.offset = var0; return EOF; }

	// first read
	if(!(fp->header.flag&0x10)) { fp->header.offset = var0; return EOF; }
	

	if(fp->header.flag&0x80)
	{
		// atquivo null
		offset = fp->header.offset/0x10000;
		if(fp->header.offset%0x10000)offset++;

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


		offset = fp->header.offset/0x10000;
		start = (offset*(0x10000/(count2*bps)));

		for(i = 0;i<block_count;i++) {

			lba_start = block[0] + (count2*i) + start;
			if(block_write(dev_n,count2,lba_start,buffer+(count2*bps*i))) 
			{
				fp->header.offset = var0;
				return EOF;
			}
		}

		fp->header.offset = var0;
		return 0;


	}





	// error error fat
	if((FAT_BPB *)fp->header.bpb == NULL) return EOF;

	FAT_BPB *bpb = (FAT_BPB *)fp->header.bpb;


	FAT_DIRECTORY *root = FatOpenRoot(bpb);

	if(root == NULL) { return EOF; }

	// escrever
	if(fp->header.size) {

		// FIXME trabalhar com o offset

		offset = (fp->header.offset/0x10000);

		// ler os primeiros 64KiB ou < 64KiB
		if(fp->header.size >= ((0x10000*offset)+0x10000)) {

			block_count = 0x10000 /(bps*count);
					
		} else {

			block_count = fp->header.size/(bps*count);

			if(fp->header.size%(bps*count) ) block_count++;

			block_count -= ((0x10000/(bps*count))*(offset));

		}


		offset = (fp->header.offset)/0x10000;
		start = (offset*(0x10000/(count*bps)));
		
		for(i = 0;i < block_count;i++) {

			lba_start = block[i + start];
			if(block_write(dev_n,count,lba_start,buffer + (bps*count*i))) 
			{ 
			
				free(root);
				fp->header.offset = var0; 
				return EOF;
			}

		}


	}


	if(FatUpdateFile((FAT_BPB *)fp->header.bpb,root,fp)) {

		free(root);
		fp->header.offset = var0;
		return EOF;
	}


	fp->header.offset = var0;
	free(root);
	return 0;


}
