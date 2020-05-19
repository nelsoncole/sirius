#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/sys.h>

extern unsigned int fat_flag;
extern unsigned int fat_ready;

extern int SalveFat(FAT_BPB *bpb,void *_fat_table,int FATOffset,unsigned int val);

int addfat(FILE *fp) 
{
	FAT_BPB *bpb = (FAT_BPB *) fp->superblock;


	int r = fp->tnb - 1;

	int table_idex = r / 1024;
	int block_idex = r % 1024;

	unsigned int *block= (unsigned int *) fp->table[table_idex];

	unsigned int first_sector_of_cluster = block[block_idex];

	unsigned short *fat_table =(unsigned short*) malloc(0x2000);

	unsigned int FATOffset = 0;
	unsigned int TableEntry = 0;
	unsigned int current_n =0;
	unsigned int new_n =0;
	int i;

	// calcular o total numero de sectores Root Directory, RootDirSectors FAT12/16
	unsigned int root_sectors = ((bpb->BPB_RootEntCnt * 32) + (bpb->BPB_BytsPerSec - 1)) / bpb->BPB_BytsPerSec;

	// Calcular o 1º sector de dados, FirstDataSector FAT12/16
	unsigned int first_data_sector = bpb->BPB_RsvdSecCnt +\
	(bpb->BPB_FATSz16 * bpb->BPB_NumFATs) + root_sectors + bpb->BPB_HiddSec;

	// FirstSectorofCluster = ((N – 2) * BPB_SecPerClus) + FirstDataSector;
	unsigned int n = (((first_sector_of_cluster - first_data_sector)/bpb->BPB_SecPerClus) + 2 );


	// ThisFATSecNum = BPB_ResvdSecCnt + bpb->BPB_HiddSec
	unsigned int 	ThisFATSecNum = bpb->BPB_RsvdSecCnt + bpb->BPB_HiddSec;
	int count = 8;
	// Ler a FAT, achar uma entrada NULL

	FATOffset = n *2; // For FAT16
	TableEntry = n - ((bpb->BPB_BytsPerSec/ 2) * (FATOffset / bpb->BPB_BytsPerSec));

	if(ReadFat(bpb,fat_table,FATOffset)) { free(fat_table); return EOF; } //ERROR

	// For FAT16
	if(fat_table[TableEntry] != (EOF&0xffff)) { free(fat_table); return EOF; } //ERROR

	// salve 
	current_n = n;
	n = 0;

	// active turbo
	fat_flag = 0;
	fat_ready = 1;

	int j = 0; // FAT16
	int t = (bpb->BPB_BytsPerSec*count)/2;
	int fl = 1;

	// sondar por uma entrada NULL
	for(i =0;i < 65536/*FIXME FATsize*/;i++) { //For FAT16

		if(j >= t || fl != 0 ) {
			
			if(block_read(bpb->specific.fat.BS_DrvNum,count,ThisFATSecNum,fat_table)) { 
				free(fat_table); 
				return EOF;
			}

			ThisFATSecNum += count;
			j = 0;
			fl = 0;
		}

		if( !fat_table[j++] ) break;

		n++;

	}

	// dactive turbo
	fat_flag = 0;
	fat_ready = 0;

	//salve
	new_n += n;

	// escreve o fat
	FATOffset = current_n *2; // For FAT16
	if(SalveFat(bpb,fat_table,FATOffset,new_n)) { free(fat_table); return EOF; } //ERROR

	FATOffset = new_n *2; // For FAT16
	if(SalveFat(bpb,fat_table,FATOffset,EOF)) { free(fat_table); return EOF; } //ERROR


	if( fp->tnb >= (1024 * 1024) ) {

		//FIXME aqui devemos adicionar um novo bloco de 4KiB no *vfs 
		fputs("FIXME AddFAT(), limite de bloco no vfs\n",stdout); 
		free(fat_table);
		return EOF;
	} else {
		// FirstSectorofCluster = ((N – 2) * BPB_SecPerClus) + FirstDataSector;

		if(block_idex >= 1023) {
			table_idex++;
			block_idex = 0;

			fp->table[table_idex] = (unsigned int ) malloc(0x1000);

		}else block_idex++;

		
		block= (unsigned int *) fp->table[table_idex];

		block[block_idex] = first_sector_of_cluster = \
		((new_n - 2) * bpb->BPB_SecPerClus) + first_data_sector;

		fp->tnb++;

	}

	free(fat_table);

	return 0;

}

