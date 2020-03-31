/*
 * File Name: rmove.c
 *
 *
 * BSD 3-Clause License
 * 
 * Copyright (c) 2019, nelsoncole
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 * 
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */
 
#include <sys/sys.h>
#include <stdlib.h>


int rmove (const char *path) {

	const char *filename = path;

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
