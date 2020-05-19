#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/sys.h>

extern unsigned int fat_flag;
extern unsigned int fat_ready;

extern int FATReadLFN(const char *filename,void *dir_lfn, int count);
extern void FileShortName(char* shortname,const char* filename);

int fileopen(FILE *fp) 
{

	FAT_BPB *bpb = (FAT_BPB *) fp->superblock;
	FAT_DIRECTORY *dir = (FAT_DIRECTORY *) fp->root;

	char shortname[11];
	unsigned int lba = 0;
	unsigned int N = 0;
	int FATOffset = 0;
	int TableEntry = 0;
	unsigned short *fat_table =(unsigned short*) malloc(0x1000);

	char *lonf_file_name = (char*)malloc(256);
	int i;
	unsigned char sum = 0;
	int count = 0;
	unsigned char *cur_dir = 0;


	unsigned int *block = 0;
	int block_idex	= 0;
	char *filename = (char *)fp->fname;

	// DATA
	unsigned int first_sector_of_cluster = 0;

	// calcular o total numero de sectores Root Directory, RootDirSectors FAT12/16
	unsigned int root_sectors = ((bpb->BPB_RootEntCnt * 32) + (bpb->BPB_BytsPerSec - 1)) / bpb->BPB_BytsPerSec;

	// Calcular o 1º sector de dados, FirstDataSector FAT12/16
	unsigned int first_data_sector = bpb->BPB_RsvdSecCnt +\
	(bpb->BPB_FATSz16 * bpb->BPB_NumFATs) + root_sectors + bpb->BPB_HiddSec;

	// Comparar ate achar o SHORT NAME
	FileShortName(shortname,(char*)filename);

	count = 0;
	for(i = 0;i < 512/*FIXME*/; i++) {

		if((dir->DIR_Name[0] ==0)){goto error;} // FIXME Errro
		else if((dir->DIR_Name[0] == 0xE5)) { dir++; continue; } // Avaliable
		else if((dir->DIR_Name[0] == 0x20)) { dir++; continue; } // space
		else if((dir->DIR_Attr == FAT_ATTR_LONG_NAME)) {

			cur_dir = (unsigned char*)(dir);

			sum = cur_dir[13];

			count++;

			dir++; continue; 

		}
		else if((dir->DIR_Attr != FAT_ATTR_DIRECTORY) &&\
	 	(dir->DIR_Attr != FAT_ATTR_ARCHIVE)) { dir++; continue; }
	

		// default 8.3
		
		// ChkSum (unsigned char *pFcbName)
		if(sum == ChkSum ((unsigned char*)dir->DIR_Name)) {

			// Estrair Long File Name
			FATReadLFN(lonf_file_name,--dir,count);
			++dir;

			// compara LFN
			if(!(strcmp (lonf_file_name,filename))) goto successfull;

		}
	
		// compara short name
		if( !(strncmp (dir->DIR_Name,shortname,11)) ) goto successfull;
		else { count = 0; dir++;}
	}
	
	goto error;

successfull:
	// Aqui podemos carregar o VFS
	//fp->flags 	= ATTR_ARCHIVE;
	fp->size 	= dir->DIR_FileSize;
	fp->devn	= bpb->specific.fat.BS_DrvNum;
	fp->bps 	= bpb->BPB_BytsPerSec;
	fp->count	= bpb->BPB_SecPerClus;
	fp->tnb		= 0;
	fp->bsize	= fp->bps*fp->count;

	fat_flag = 0;
	fat_ready = 0;


	block  = (unsigned int *) malloc(0x1000);
	memset(block,0,0x1000);
	fp->table[block_idex++] =  (unsigned int) block;
	

	N = dir->DIR_FstClusLO | dir->DIR_FstClusHI << 16;
	
	i = 0;
	unsigned char flag = 0;
	while(1/*TRUE*/) {

		// FirstSectorofCluster = ((N – 2) * BPB_SecPerClus) + FirstDataSector;
		lba = first_sector_of_cluster = ((N - 2) * bpb->BPB_SecPerClus) + first_data_sector;
			
		if(i == 1024) {

			if(block_idex >= 1024) goto error; // error

			i = 0;
			block  = (unsigned int *) malloc(0x1000);
			memset(block,0,0x1000);
			fp->table[block_idex++] =  (unsigned int) block;

		}

		block[i++] = lba; 


		FATOffset = N *2; // For FAT16

		if(!(flag)) {
			if(ReadFat(bpb,fat_table,FATOffset))
				goto error;

			flag = 1;

		}
		// Para FAT16 divide por 2
		// TableEntry = N - ((BPB_BytsPerSec/ 2) * (FATOffset / BPB_BytsPerSec));
		TableEntry = N - ((bpb->BPB_BytsPerSec/ 2) * (FATOffset / bpb->BPB_BytsPerSec));		

		N = fat_table[TableEntry];

		fp->tnb += 1;

		if( (N == 0xFFFF) || (N == 0xFFF8) ) break;
		//For FAT16
		if(TableEntry >= ((bpb->BPB_BytsPerSec/2)-1)) flag = 0;
	
	}
		

	goto done;
error:
	free(lonf_file_name);
	free(fat_table);
	return -1;
done:
	free(lonf_file_name);
	free(fat_table);	
	return 0;
}


