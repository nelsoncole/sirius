#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/sys.h>

extern int FATReadLFN(const char *filename,void *dir_lfn, int count);
extern void FileShortName(char* shortname,const char* filename);

int updatefile(FILE *fp) 
{
	FAT_BPB *bpb = (FAT_BPB *) fp->superblock;
	FAT_DIRECTORY *dir = (FAT_DIRECTORY *) fp->root;

	char shortname[11];

	unsigned int sector_offset = 0;
	unsigned int entre_offset  = 0;

	unsigned char *buf = (unsigned char *) malloc(0x1000);

	char *lonf_file_name = (char*) malloc(0x1000);
	int i;
	unsigned char sum = 0;
	int count = 0;
	unsigned char *cur_dir;


	// DATA

	// calcular o total numero de sectores Root Directory, RootDirSectors FAT12/16
	unsigned int root_sectors = ((bpb->BPB_RootEntCnt * 32) + (bpb->BPB_BytsPerSec - 1)) / bpb->BPB_BytsPerSec;

	// Calcular o 1º sector de dados, FirstDataSector FAT12/16
	unsigned int first_data_sector = bpb->BPB_RsvdSecCnt +\
	(bpb->BPB_FATSz16 * bpb->BPB_NumFATs) + root_sectors + bpb->BPB_HiddSec;

	// Calcular o 1º sector do Root Directory, FirstRootDirSector FAT12/16
	unsigned int first_root_sector = (first_data_sector - root_sectors);






	// Comparar ate achar o SHORT NAME
	FileShortName(shortname,(const char *)fp->fname);

	for(i = 0;i < 512/*FIXME*/; i++) {

		if((dir->DIR_Name[0] ==0)){free(buf); return -1;} 
		else if((dir->DIR_Name[0] == 0xE5)) { dir++; continue; } // Avaliable
		else if((dir->DIR_Name[0] == 0x20)) { dir++; continue; } // space
		else if((dir->DIR_Attr == FAT_ATTR_LONG_NAME)) {

			cur_dir = (unsigned char*)dir;
			sum = cur_dir[13];

			count++;

			dir++; continue; 

		}
	

		// default 8.3
		
		// ChkSum (unsigned char *pFcbName)
		if(sum == ChkSum ((unsigned char*)dir->DIR_Name)) {

			// Estrair Long File Name
			FATReadLFN(lonf_file_name,--dir,count);
			++dir;

			// compara LFN
			if(!(strcmp (lonf_file_name,(const char *)fp->fname))) goto successfull;

		}
	
		// compara short name
		if( !(strncmp (dir->DIR_Name,shortname,11)) ) goto successfull;
		else { count = 0; dir++;}
	}
	
	free(lonf_file_name); 
	free(buf); return -1;

successfull:


	// add FAT

	// Aqui podemos actualizar o directory padrao
	dir->DIR_FileSize = fp->size;
	//salvar no disco

	sector_offset = (bpb->BPB_BytsPerSec * (i) / (bpb->BPB_BytsPerSec/32))/bpb->BPB_BytsPerSec + first_root_sector;
	entre_offset  = (i)%(bpb->BPB_BytsPerSec/32);

	if( block_read(bpb->specific.fat.BS_DrvNum,1,sector_offset,buf) ){free(lonf_file_name); free(buf); return -1;}

	memcpy(buf + (32 *entre_offset),dir,32);

	if( block_write(bpb->specific.fat.BS_DrvNum,1,sector_offset,buf) ){free(lonf_file_name); free(buf); return -1;}
	
	free(buf);
	free(lonf_file_name);
	return 0;
}
