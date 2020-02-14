#include <stdio.h>
#include <stdlib.h>
#include <diskblock.h>


/*
 * TODO, apenas suport a FAT
 *
 */


int remove (const char *filename)
{
	FAT_BPB  *bpb  		= NULL;
	FAT_DIRECTORY *root 	= NULL;
	MBR *mbr	= (MBR*) malloc(0x200);
	VOLUME *volume	= (VOLUME*)malloc(sizeof(VOLUME));



	if(block_read(0,1,0,mbr) ) goto error;

	volume->lba_start = mbr->part[0].lba_start;
	volume->dev_num = 0;


	bpb  	= FatReadBPB(volume);
	if(bpb == NULL) goto error;

	root =FatOpenRoot(bpb);
	if(root == NULL) goto error; 

	// deve criar uma funcao FATCheckFile()
	if(!FatCheckFile(bpb,root,filename) ) {

		// remover arquivo
		if(FatRemveFile(bpb,root,filename)) {
			goto error;

		}
	} 


	free(volume);
	free(mbr);
	free(root); 
	free(bpb);

	return 0;
error:
	free(volume);
	free(mbr);
	free(root); 
	free(bpb); 
	return -1;
}
