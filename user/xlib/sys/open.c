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



static int open_null(SD *sdx,FILE *fd) 
{
	fd->header.count = 0xFFFFFFFF; // 4GiB por sector

	if((sdx->num_sectors*sdx->byte_of_sector/1024/1024) > 4096) return -1; 

	fd->header.bps = sdx->byte_of_sector;

	fd->header.size = sdx->num_sectors*sdx->byte_of_sector;

	fd->header.dev = sdx->devnum;

	fd->header.blocks = 1; // FIXME apenas suporta 4 GiB

	fd->block[0] =  sdx->lba_start;

	fd->header.attr = ATTR_ARCHIVE;

	return 0;
}

FILE *open(const char *path,int attr,const char *mode) {

	FILE *fd = NULL;

	SD *sdx = NULL;

	const char *filename = NULL;

	if(mode[0] == '\0') return NULL;
	if(path[0] == '\0') return NULL;

	unsigned int strsdx[2];
	unsigned int strsdn[2];
	memset(strsdx,0,8);
	memset(strsdn,0,8);

	int null = 0;
	int chil = 0;
	char *p_path;
	char *t_path;


	p_path = (char*)malloc(0x1000);

	if(path[0] == '/') 
	{

		if(!strncmp(path,"/dev/",5)) 
		{
			// null device
			null = 12345;
			chil = 0;
			

			t_path = pathneme(p_path,path+1);

			if(strncmp(p_path,"dev",3)) {
				
				free(p_path);
				return (NULL);

			}

			if(!t_path[0]) 
			{
				free(p_path);
				return (NULL);
			}


			t_path = pathneme(p_path,t_path);
			memcpy(strsdx,p_path,3);

			if(t_path[0]) 
			{
				chil = 1;
				t_path = pathneme(p_path,t_path);
				memcpy(strsdn,p_path,4);

				if(t_path[0]) 
				{
					// local
					filename = t_path;
					null = 0;
				}

			} 	

		} else {
				// Local desconhecido
	
				free(p_path);
				return (NULL);


		}


	} else {
		// pwd + path
		memcpy(strsdx,"sda",3);
		memcpy(strsdn,"sda1",4);

		filename = path;

		if(!filename[0]) 
		{
			free(p_path);
			return (NULL);
		}

	}

	free(p_path);


	// FIXME test open directory
	if((attr == ATTR_DIRECTORY) || (attr == ATTR_ARCHIVE)){
		fd = (FILE*)malloc(0x2000);
		memset(fd,0,0x2000);

		// 64 KiB
		fd->header.buffer =(unsigned int) malloc(0x10000);

	} else { 
		return (NULL);
	}


	if(null == 12345) 
	{

		fd->header.flag |= 0x80;

		sdx = read_sdx((const char*)strsdx);
		if(chil) sdx = read_sdn((const char*)strsdn,sdx);

		if(open_null(sdx,fd))
		{
			free((void*)fd->header.buffer);
			free(fd);
			return (NULL);

		}


		fd->header.bpb = (unsigned int)malloc(0x1000);

		goto successfull;

	} else {



	}


	sdx = read_sdx((const char*)strsdx);
	sdx = read_sdn((const char*)strsdn,sdx);

	VOLUME *volume	= (VOLUME*)malloc(sizeof(VOLUME));
	

	if(sdx){

		volume->lba_start = sdx->lba_start;
		volume->dev_num = sdx->devnum;

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

						if((mode[0] == 'a') && (attr == ATTR_ARCHIVE)) { 
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
			
							goto successfull;

						}else { free(bpb); free(root); /*Open DIR/FILE error*/}

					}else { free(bpb); free(root); /*Create DIR/FILE error*/}

				}else { free(bpb); free(root); /*Nenhum modo*/}

			} else free(bpb); // Open Root error

		} else;  // read FAT BPB error

	}else; // read mbr error


abort:
	
	free(volume);
	free((void*)fd->header.buffer);
	free(fd);
	return (NULL);

successfull:
	fd->header.mode[0] = mode[0];
	fd->header.mode[1] = mode[1];
	fd->header.mode[2] = mode[2];

	return fd;
}
