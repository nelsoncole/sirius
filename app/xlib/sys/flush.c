#include <sys/sys.h>
#include <stdlib.h>
#include <stdio.h>

#define EOF (-1)

int flush(FILE *fp) {

	unsigned char* buffer = (unsigned char*) (fp->header.buffer);
	unsigned dev_n = fp->header.dev;
	unsigned count = fp->header.count;
	unsigned int *block	= (unsigned int*)(fp->block);
	unsigned int bps = fp->header.bps;
	unsigned int lba_start;
	int block_count;
	int i;



	// error
	if(fp->header.mode[0] == '\0') return EOF;

	// error error fat
	if((FAT_BPB *)fp->header.bpb == NULL) return EOF;

	FAT_BPB *bpb = (FAT_BPB *)fp->header.bpb;


	FAT_DIRECTORY *root = FatOpenRoot(bpb);

	if(root == NULL) { return EOF; }



	// escrever

	if(fp->header.size) {

		// ler os primeiros 64KiB ou < 64KiB
		if(fp->header.size >= 65536 ) {

			block_count = 65536 /(bps*count);
					
		} else {
			block_count = fp->header.size/(bps*count);
			if(fp->header.size%(bps*count)) block_count++;

		}

		
		for(i = 0;i < block_count;i++) {

			// FIXME
			lba_start = block[i];

			if(block_write(dev_n,count,lba_start,buffer + (bps*count*i))) { 
			
				free(root); 
				return EOF;
			}

		}

	}


	if(FatUpdateFile((FAT_BPB *)fp->header.bpb,root,fp)) {

		free(root);
		return EOF;
	}


	free(root);
	return 0;


}
