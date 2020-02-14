#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <diskblock.h>



/*
 *  Modo 	Significado
 *
 * r		Abre um arquivo-texto para leitura
 * w		Cria um arquivo-texto para escrita
 * a		Anexa a um arquivo-texto
 * rb		Abre um arquivo binário para leitura
 * wb		Cria um arquivo binário para escrita
 * ab		Anexa a um arquivo binário
 * r+		Abre um arquivo-texto para leitura/escrita
 * w+		Cria um arquivo-texto para leitura/escrita
 * a+		Anexa ou cria um arquivo-texto para leitura/escrita
 * r+b		Abre um arquivo binário para leitura/escrita
 * w+b		Cria um arquivo binário para escrita/escrita
 * a+b		Anexa ou cria um arquivo binário para escrita/escrita
 *
 *	
 */


/*
 * TODO, apenas suport a FAT
 *
 */


FILE *fopen (const char *filename,const char *mode) 
{
	if(mode[0] == '\0') return NULL;


	FAT_BPB  *bpb  		= NULL;
	FAT_DIRECTORY *root 	= NULL;
	FILE *fp = (FILE*) malloc(0x1000);
	memset(fp,0,0x1000);

	// 64 KiB
	fp->header.buffer =(unsigned int) malloc(0x10000);

	MBR *mbr	= (MBR*) malloc(0x200);
	VOLUME *volume	= (VOLUME*)malloc(sizeof(VOLUME));

/*	FIXME
 *
 *	No futuro, o MBR deve ser lindo fora destas rotinas
 *
 */
	if(block_read(0,1,0,mbr) ) goto error;

	volume->lba_start = mbr->part[0].lba_start;
	volume->dev_num = 0;


	switch(0) {

	case 0: //FAT File System

		bpb  	= FatReadBPB(volume);
		if(bpb == NULL) goto error;

		// salve bpb
		fp->header.bpb =  (unsigned int)bpb;

		root =FatOpenRoot(bpb);
		if(root == NULL) { 
			
			free(bpb);  
			goto error; 
		}


		if ( (mode[0] == 'r') || (mode[0] == 'a') ) {

			if( FatOpenFile(bpb,root,filename,ATTR_ARCHIVE,fp) ) {

				// configure arquivo
				

			}else { free(root); free(bpb); goto error;}


		}else if ( (mode[0] == 'w') || (mode[0] == 'a') )  {


			// se o arquivo exite, devemos zerar
			// se nao existe devemos criar um vazio
		
			// verificar se existe
			if(!FatCheckFile(bpb,root,filename) ) {
				// deve criar uma funcao FATCheckFile()
				// remover arquivo
				if(FatRemveFile(bpb,root,filename)) {
					free(root); free(bpb); goto error;

				}
			} 

			// criar arquivo
			if(!FatCreateFile(bpb,root,filename,ATTR_ARCHIVE) ) {
			
				// abrir arquivo
				if( FatOpenFile(bpb,root,filename,ATTR_ARCHIVE,fp) ) {

					// configure arquivo
				

				}else { free(root); free(bpb); goto error;}
			}else { free(root); free(bpb); goto error;}

		}else { free(root); free(bpb); goto error;}

			free(root); 
			free(bpb);
			// successfull
			// configure arquivo
			fp->header.mode[0] = mode[0];
			fp->header.mode[1] = mode[1];
			fp->header.mode[2] = mode[2];

			if(mode[0] == 'a') { 
				fp->header.offset2 = fp->header.offset = fp->header.size;
			}
			break;


	default: // Outro File System
			goto error;
			break;


	}

	
	
	return (FILE*) fp;
	

error:
	free(volume);
	free(mbr);
	free((void*)fp->header.buffer);
	free(fp);
	return NULL;

	
}
