/*
 * File Name: open.c
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
#include <string.h>

#define NULL ((void *)0)


FILE *open(const char *path,int attr,const char *mode) {

	FILE *fd = NULL;

	const char *filename = path;

	if(mode[0] == '\0') return NULL;

	// FIXME test open directory
	if((attr == ATTR_DIRECTORY) || (attr == ATTR_ARCHIVE)){
		fd = (FILE*)malloc(0x2000);
		memset(fd,0,0x2000);

		// 64 KiB
		fd->header.buffer =(unsigned int) malloc(0x10000);

	} else { 
		return (NULL);
	}


	MBR *mbr	= (MBR*)malloc(0x200);
	VOLUME *volume	= (VOLUME*)malloc(sizeof(VOLUME));
	

	if(!block_read(0,1,0,mbr) ){

		volume->lba_start = mbr->part[0].lba_start;
		volume->dev_num = 0;

		//FAT TYPE
		FAT_BPB  *bpb  	= FatReadBPB(volume);

		if(bpb != 0){

			// salve bpb
			fd->header.bpb = (unsigned int)bpb;

			// aqui podemos lidar com os subdirectorios



			FAT_DIRECTORY *root =FatOpenRoot(bpb);
			if(root) {

				if ( (mode[0] == 'r') || (mode[0] == 'a') ) {

					if(!FatOpenFile(bpb,root,filename,attr,fd)) {

						free(root);
						free(volume);
						free(mbr);

						if(mode[0] == 'a' && attr == ATTR_ARCHIVE) { 
							fd->header.offset2 = fd->header.size;
							fd->header.offset = fd->header.size;
						}

						goto successfull;
					
				
					} else { free(bpb); free(root); /*Open DIR error*/}



				} else if ( (mode[0] == 'w') || (mode[0] == 'a') )  {


					// se o arquivo exite, devemos zerar
					// se nao existe devemos criar um vazio
		
					// verificar se existe
					if(!FatCheckFile(bpb,root,filename) ) {
						// remover arquivo
						if(FatRemveFile(bpb,root,filename)) {
							free(bpb); free(root);

							goto abort;

						}else {

				
							if(attr != ATTR_ARCHIVE) {
								// FIXME, nao remover directorio
								free(bpb); free(root);
								goto abort;
							}

						}
					}

 					// criar arquivo
					if(!FatCreateFile(bpb,root,filename,attr) ) {

						
						//update root
						if(FatUpdateRoot(bpb,root)) {

							free(bpb); free(root);
							goto abort;
						}
	
			
						// abrir arquivo
						if(!FatOpenFile(bpb,root,filename,attr,fd) ) {

							free(root);
							free(volume);
							free(mbr);

							goto successfull;

						}else { free(bpb); free(root); /*Open DIR/FILE error*/}

					}else { free(bpb); free(root); /*Create DIR/FILE error*/}

				}else { free(bpb); free(root); /*Nenhum modo*/}

			} else free(bpb); // Open Root error

		} else;  // read FAT BPB error

	}else; // read mbr error


abort:
	
	free(volume);
	free(mbr);
	free((void*)fd->header.buffer);
	free(fd);
	return (NULL);

successfull:
	fd->header.mode[0] = mode[0];
	fd->header.mode[1] = mode[1];
	fd->header.mode[2] = mode[2];

	return fd;
}
