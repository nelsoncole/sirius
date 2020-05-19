#include <sys/sys.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define NULL ((void *)0)


void free_file_table(FILE *fp)
{
	int i;
	if(!fp->table)return;

	for(i = 0; i <1024;i++) {

		if(!fp->table[i]) break;

		free((void*)fp->table[i]);
	}

	free(fp->table);
}

static int open_null_r(SD *sdx,FILE *fp) 
{
	fp->count = 8; // 0xFFFFFFFF; // 4GiB por sector

	if((sdx->num_sectors*sdx->byte_of_sector/1024/1024) > 4096) return -1; 

	fp->bps = sdx->byte_of_sector;

	fp->size = sdx->num_sectors*sdx->byte_of_sector;

	fp->devn = sdx->devnum;

	fp->tnb = 1; // FIXME apenas suporta 4 GiB
	
	fp->bsize	= fp->bps*fp->count;

	fp->superblock  = (unsigned char *) malloc(0x1000);
	fp->flags |= 0x80;


	unsigned int *block  = (unsigned int *) malloc(0x1000);
	memset(block,0,0x1000);

	fp->table[0] =  (unsigned int) block;

	block[0] =  sdx->lba_start;

	return 0;
}

FILE *open_r(const char *path,int attr,const char *mode) {

	FILE *fp = NULL;

	SD *sdx = NULL;

	const char *filename = NULL;

	if(mode[0] == '\0') return NULL;
	if( (path[0] == '\0') || (!path)) return NULL;

	unsigned int strsdx[2];
	unsigned int strsdn[2];
	memset(strsdx,0,8);
	memset(strsdn,0,8);

	int null = 0;
	int chil = 0;
	char *p_path;
	char *t_path;


	p_path = (char*) malloc(0x1000);

	if(path[0] == '/') {

		if(!strncmp(path,"/dev/",5)) {
			
			// null device
			null = 12345;
			chil = 0;

			t_path = pathneme(p_path,path+1);

			if(strncmp(p_path,"dev",3)) {
				
				free(p_path);
				return (NULL);

			}

			if(!t_path[0]) {
				free(p_path);
				return (NULL);
			}


			t_path = pathneme(p_path,t_path);
			memcpy(strsdx,p_path,3);

			if(t_path[0]) {
				chil = 1;
				t_path = pathneme(p_path,t_path);
				memcpy(strsdn,p_path,4);

				if(t_path[0]) {
					// local
					filename = t_path;
					null = 0;
				}

			} 


		} else {
				// Local desconhecido

				fputs("Debug: /dev/...",stdout);
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
		fp = (FILE*)malloc(sizeof(FILE));
		memset(fp,0,sizeof(FILE));

		fp->table = (unsigned int *)malloc(0x1000); //4KB
		memset(fp->table,0,0x1000);

		fp->buffer = (unsigned char *) malloc(BUFSIZ);

		if(null != 12345) strcpy(fp->fname,filename);

		int len = strlen(mode);
		if(len > 4) len = 4;
		memcpy(fp->mode,mode,len);

	} else { 
		return (NULL);
	}

	if(null == 12345) 
	{

		sdx = read_sdx((const char*)strsdx);
		if(chil) sdx = read_sdn((const char*)strsdn,sdx);

		if(open_null_r(sdx,fp))
		{
			free(fp->buffer);
			free_file_table(fp);
			free(fp);
			return (NULL);

		}

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
			fp->superblock  = (unsigned char *) bpb;

			// aqui podemos lidar com os subdirectorios
			fp->root = (unsigned char *) FatOpenRoot((FAT_BPB *)fp->superblock);


			if ( (fp->mode[0] == 'r') || (fp->mode[0] == 'a') ) {

				if( fileopen(fp) ) goto abort;

				if((fp->mode[0] == 'a') && (attr == ATTR_ARCHIVE)) { 
						fp->off = fp->size;
				}

			}else if (fp->mode[0] == 'w') {

				// se o arquivo exite, devemos zerar
				// se nao existe devemos criar um vazio
		
				// verificar se existe
				if(!FatCheckFile(bpb,(FAT_DIRECTORY *)fp->root,filename) ) {
					// remover arquivo
					if(FatRemveFile(bpb,(FAT_DIRECTORY *)fp->root,filename)) {
						goto abort;

					}else {
						if(attr != ATTR_ARCHIVE) {
							// FIXME, nao remover directorio
							goto abort;
						}

					}
				}


				// criar arquivo
				if(!FatCreateFile(bpb,(FAT_DIRECTORY *)fp->root,filename,attr) ) {
					//update root
					if(FatUpdateRoot(bpb,(FAT_DIRECTORY *)fp->root)) goto abort;
			
					// abrir arquivo
					if( fileopen(fp) ) goto abort;

				} else goto abort;



			}else goto abort;

				// successfull
				free(volume);
				free(fp->root);

		}else {

			free(volume);
			free(fp->buffer);
			free_file_table(fp);
			free(fp);
			return (NULL);

		} ////

	}else {

		free(volume);
		return (NULL);
	}

successfull:
	fp->flags |= 0x10;
	if(attr == ATTR_ARCHIVE) fp->flags |= 0x20;
	return fp;

abort:
	free(fp->superblock);
	free(fp->root);
	free(volume);
	free(fp->buffer);
	free_file_table(fp);
	free(fp);
	return (NULL);
}
