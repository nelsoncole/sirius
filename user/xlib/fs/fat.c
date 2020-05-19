/*
 * File Name: fat.c
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





#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/sys.h>


unsigned int fat_flag = 0;
unsigned int fat_ready = 0;


//FIXME Nelson nao deixar de olhar o fixme em AddFat(); 


// 1 Read BPB
// 2 Read Root
// 3 Read FAT
// 4 Funcao FatOpenFile(in dev_num, in filename,out vfs)

//-----------------------------------------------------------------------------
// ChkSum()
// Returns an unsigned byte checksum computed on an unsigned byte
// array. The array must be 11 bytes long and is assumed to contain
// a name stored in the format of a MS-DOS directory entry.
// Passed: pFcbName   Pointer to an unsigned byte array assumed to be
// 11 bytes long.
// Returns: Sum    An 8-bit unsigned checksum of the array pointed
// to by pFcbName.
//------------------------------------------------------------------------------
unsigned char ChkSum (unsigned char *pFcbName)
{
	short FcbNameLen;
	unsigned char Sum;
	Sum = 0;

	for (FcbNameLen=11; FcbNameLen!=0; FcbNameLen--) {
		// NOTE: The operation is an unsigned char rotate right
		Sum = ((Sum & 1) ? 0x80 : 0) + (Sum >> 1) + *pFcbName++;
	}

	return (Sum);
}
int FATReadLFN(const char *filename,void *dir_lfn, int count)
{
	char *name = (char*)filename;
	unsigned char ord = 0;
	char *tmp;
	int i,t;
	int _count = count;

	for(i=0;i < _count || (ord != 0x40);i++) {
		tmp = (char*)( (char*)dir_lfn - (i*32) );
		// LDIR_Name1
		ord = *tmp++;
		for(t=0;t<5;t++) {

			*name++ = *tmp;
			if(!(*tmp)) goto end;
			
			tmp++; tmp++;
		}
		// LDIR_Name2
		tmp++; tmp++; tmp++;
		for(t=0;t<6;t++) {

			*name++ = *tmp;
			if(!(*tmp)) goto end;
			
			tmp++; tmp++;
		}
		// LDIR_Name3
		tmp++; tmp++;
		for(t=0;t<2;t++) {

			*name++ = *tmp;
			if(!(*tmp)) goto end;
			
			tmp++; tmp++;
		}
	

	}

	return -1;

end:
	return 0;

}


void FileShortName(char* shortname,const char* filename)
{
	char* _filename = (char*)filename;
	char* _shortname = shortname;
	int i;

	for(i =0;(*_filename != '.') && (*_filename != 0);i++) {
	 	if(i < 11)*_shortname++ = *_filename++;
		else return;
	}

	for(;i < 8;i++)*_shortname++ = ' ';

	// saltar o .
	_filename++;
	
	for(i =0; i < 3; i++) *_shortname++ = *_filename++;

	// Convert para Mausculos
	_shortname = shortname;

	for(i =0; i < 11; i++) {

		if((_shortname[i] > 0x60) && (_shortname[i] < 0x7B ) )
		_shortname[i] = (_shortname[i] - 0x20);

	}
	
}

void FileMountShortName(char* shortname,const char* filename)
{
	char* _filename = (char*)filename;
	char* _shortname = shortname;
	int i;
	int exe = 0;

	for(i=0; i < 11;i++) {

		if((i == 8) && (!exe) && ((_filename[8] != ' ') || (_filename[9] != ' ') || (_filename[10] != ' ')) ) {
			
			_shortname[i + exe] = '.';

			exe = 1;
			i--;

		} else _shortname[i + exe] = _filename[i]; 
	}

	// retirar o 0x20
	for(i=7;i >= 0;i--) {
		if(_shortname[i] == ' ') {

			_shortname[i] = _shortname[i+1];
			_shortname[i+1] = _shortname[i+2];
			_shortname[i+2] = _shortname[i+3];
			_shortname[i+3] = _shortname[i+4];
			_shortname[i+4] = 0;

		} 
	}
	

}

FAT_BPB *FatReadBPB(VOLUME *volume) 
{
	VOLUME *v = volume;
	
	FAT_BPB *bpb = (FAT_BPB*)malloc(0x1000);


	if(block_read(v->dev_num,1,v->lba_start,bpb))
		return 0;

	// configure device number
	bpb->specific.fat.BS_DrvNum = v->dev_num;

	return ((FAT_BPB*) bpb);

}


FAT_DIRECTORY *FatOpenRoot(FAT_BPB *bpb)
{
	FAT_DIRECTORY *root = (FAT_DIRECTORY *)malloc(0x10000); //64 KiB = 2048 entradas

	memset(root,0,0x10000);

	// calcular o total numero de sectores Root Directory, RootDirSectors FAT12/16
	unsigned int root_sectors = ((bpb->BPB_RootEntCnt * 32) + (bpb->BPB_BytsPerSec - 1)) / bpb->BPB_BytsPerSec;
	

	// Calcular o 1º sector de dados, FirstDataSector FAT12/16
	unsigned int first_data_sector = bpb->BPB_RsvdSecCnt +\
	(bpb->BPB_FATSz16 * bpb->BPB_NumFATs) + root_sectors + bpb->BPB_HiddSec;

	
	// Calcular o 1º sector do Root Directory, FirstRootDirSector FAT12/16
	unsigned int first_root_sector = (first_data_sector - root_sectors);


	// Carregar o Root Directory na memorio, FAT12/16
	if(block_read(bpb->specific.fat.BS_DrvNum,root_sectors,first_root_sector,root))
		return 0;


	return ((FAT_DIRECTORY*) root);

}



int FatUpdateRoot(FAT_BPB *bpb,FAT_DIRECTORY *root)
{
	// calcular o total numero de sectores Root Directory, RootDirSectors FAT12/16
	unsigned int root_sectors = ((bpb->BPB_RootEntCnt * 32) + (bpb->BPB_BytsPerSec - 1)) / bpb->BPB_BytsPerSec;
	

	// Calcular o 1º sector de dados, FirstDataSector FAT12/16
	unsigned int first_data_sector = bpb->BPB_RsvdSecCnt +\
	(bpb->BPB_FATSz16 * bpb->BPB_NumFATs) + root_sectors + bpb->BPB_HiddSec;

	

	// Calcular o 1º sector do Root Directory, FirstRootDirSector FAT12/16
	unsigned int first_root_sector = (first_data_sector - root_sectors);


	// Carregar o Root Directory na memorio, FAT12/16
	if(block_read(bpb->specific.fat.BS_DrvNum,root_sectors,first_root_sector,root))
		return -1;

	return 0;

}

int ReadFat(FAT_BPB *bpb,void *fat_table,int FATOffset)
{

	// ThisFATSecNum = BPB_ResvdSecCnt + (FATOffset / BPB_BytsPerSec);
	unsigned int 	ThisFATSecNum = bpb->BPB_RsvdSecCnt + (FATOffset / bpb->BPB_BytsPerSec) + bpb->BPB_HiddSec;


	if( (fat_ready == 1) && (ThisFATSecNum == fat_flag)) return 0;
	else fat_flag = ThisFATSecNum;

	if(block_read(bpb->specific.fat.BS_DrvNum,1,ThisFATSecNum,fat_table))
		return -1;

	return 0;

}

int SalveFat(FAT_BPB *bpb,void *_fat_table,int FATOffset,unsigned int val)
{
	unsigned short *fat_table = (unsigned short*)_fat_table;
	int TableEntry;

	// ThisFATSecNum = BPB_ResvdSecCnt + (FATOffset / BPB_BytsPerSec);
	unsigned int 	ThisFATSecNum = bpb->BPB_RsvdSecCnt + (FATOffset / bpb->BPB_BytsPerSec) + bpb->BPB_HiddSec;


	if(block_read(bpb->specific.fat.BS_DrvNum,1,ThisFATSecNum,fat_table))
		return -1;

	// Para FAT16 divide por 2
	// TableEntry = N - ((BPB_BytsPerSec/ 2) * (FATOffset / BPB_BytsPerSec));
	TableEntry = (FATOffset /2) - ((bpb->BPB_BytsPerSec/ 2) * (FATOffset / bpb->BPB_BytsPerSec));
	fat_table[TableEntry] = val &0xFFFF;
	

	if(block_write(bpb->specific.fat.BS_DrvNum,1,ThisFATSecNum,fat_table))
		return -1;


	return 0;

}

int AddFAT(VFS *fd, FAT_BPB *bpb, unsigned int first_sector_of_cluster) 
{
	unsigned short *fat_table =(unsigned short*) malloc(0x1000);

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

	// Ler a FAT, achar uma entrada NULL

	FATOffset = n *2; // For FAT16
	TableEntry = n - ((bpb->BPB_BytsPerSec/ 2) * (FATOffset / bpb->BPB_BytsPerSec));

	if(ReadFat(bpb,fat_table,FATOffset)) { free(fat_table); return EOF; } //ERROR

	// For FAT16
	if(fat_table[TableEntry] != (EOF&0xffff)) { free(fat_table); return EOF; } //ERROR

	// salve 
	current_n = n;
	n = 3;

	// active turbo
	fat_flag = 0;
	fat_ready = 1;

	// sondar por uma entrada NULL
	for(i =3;i < 65536/*FIXME FATsize*/;i++) { //For FAT16

		FATOffset = i *2; // For FAT16
		TableEntry = i - ((bpb->BPB_BytsPerSec/ 2) * (FATOffset / bpb->BPB_BytsPerSec));

		if(ReadFat(bpb,fat_table,FATOffset) ) { free(fat_table); return EOF; } //ERROR

	 	if(!fat_table[TableEntry])break;

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


	if(fd->header.blocks >= 896) {
		
		//FIXME aqui devemos adicionar um novo bloco de 4KiB no *vfs 
		fputs("FIXME AddFAT(), limite de bloco no vfs\n",stdout); 
		free(fat_table);
		return EOF;
	}
	else {
		// FirstSectorofCluster = ((N – 2) * BPB_SecPerClus) + FirstDataSector;
		fd->block[fd->header.blocks] = first_sector_of_cluster = \
		((new_n - 2) * bpb->BPB_SecPerClus) + first_data_sector;

		fd->header.blocks++;

	}


	free(fat_table);

	return 0;

}


int RemoveFAT(FAT_BPB *bpb, unsigned int sector_of_cluster) 
{
	unsigned short *fat_table =(unsigned short*) malloc(0x1000);

	// calcular o total numero de sectores Root Directory, RootDirSectors FAT12/16
	unsigned int root_sectors = ((bpb->BPB_RootEntCnt * 32) + (bpb->BPB_BytsPerSec - 1)) / bpb->BPB_BytsPerSec;

	// Calcular o 1º sector de dados, FirstDataSector FAT12/16
	unsigned int first_data_sector = bpb->BPB_RsvdSecCnt +\
	(bpb->BPB_FATSz16 * bpb->BPB_NumFATs) + root_sectors + bpb->BPB_HiddSec;

	// FirstSectorofCluster = ((N – 2) * BPB_SecPerClus) + FirstDataSector;
	unsigned int n = (((sector_of_cluster - first_data_sector)/bpb->BPB_SecPerClus) + 2 );



	// n*2 for FAT16, n*4 for FAT32

	
	if(SalveFat(bpb,fat_table,n*2/*FATOffset em bytes*/,0 /*remove fat*/)) {

		free(fat_table);
		return -1;
	}


	free(fat_table);
	return 0;

}

int EditFAT(FAT_BPB *bpb, unsigned int sector_of_cluster,unsigned int val) 
{
	unsigned short *fat_table =(unsigned short*) malloc(0x1000);

	// calcular o total numero de sectores Root Directory, RootDirSectors FAT12/16
	unsigned int root_sectors = ((bpb->BPB_RootEntCnt * 32) + (bpb->BPB_BytsPerSec - 1)) / bpb->BPB_BytsPerSec;

	// Calcular o 1º sector de dados, FirstDataSector FAT12/16
	unsigned int first_data_sector = bpb->BPB_RsvdSecCnt +\
	(bpb->BPB_FATSz16 * bpb->BPB_NumFATs) + root_sectors + bpb->BPB_HiddSec;

	// FirstSectorofCluster = ((N – 2) * BPB_SecPerClus) + FirstDataSector;
	unsigned int n = (((sector_of_cluster - first_data_sector)/bpb->BPB_SecPerClus) + 2 );



	// n*2 for FAT16, n*4 for FAT32

	
	if(SalveFat(bpb,fat_table,n*2/*FATOffset em bytes*/,val /*edit fat*/)) {

		free(fat_table);
		return -1;
	}


	free(fat_table);
	return 0;

}


int FatCreateFile(FAT_BPB *bpb,FAT_DIRECTORY *_dir,const char *filename,unsigned char attr)
{
	char short_name[11];
	char *tmp = (char*)malloc(0x1000);
	char *buf = (char*)malloc(0x1000);
	FAT_DIRECTORY *first_dir = (FAT_DIRECTORY *)malloc(0x1000);

	unsigned short *fat_table =(unsigned short*) malloc(0x1000);

	FAT_DIRECTORY *dir = _dir;
	char 	*_tmp;
	char 	*name = (char*)filename;
	unsigned name_size =0;
	unsigned dir_size =0;
	unsigned int sector_offset = 0;
	unsigned int entre_offset  = 0;

	int  FATOffset = 0;
	int  TableEntry = 0;
	unsigned int FstClus = 0;

	int rc = 0;	
	char ch = 0;

	int i,n =0;
	int LDIR_Ord = 0;
	unsigned char flag = 0;





	// DATA

	unsigned int first_sector_of_cluster = 0;

	// calcular o total numero de sectores Root Directory, RootDirSectors FAT12/16
	unsigned int root_sectors = ((bpb->BPB_RootEntCnt * 32) + (bpb->BPB_BytsPerSec - 1)) / bpb->BPB_BytsPerSec;

	// Calcular o 1º sector de dados, FirstDataSector FAT12/16
	unsigned int first_data_sector = bpb->BPB_RsvdSecCnt +\
	(bpb->BPB_FATSz16 * bpb->BPB_NumFATs) + root_sectors + bpb->BPB_HiddSec;

	// Calcular o 1º sector do Root Directory, FirstRootDirSector FAT12/16
	unsigned int first_root_sector = (first_data_sector - root_sectors);




	FstClus = 3;
	FATOffset = 0;

	rc = ReadFat(bpb,fat_table,FATOffset);
	if(rc) goto end; // error



	// active turbo
	fat_flag = 0;
	fat_ready = 1;

	for(i =3;i < 65536/*FIXME FATsize*/;i++) { //For FAT16

		FATOffset = i *2; // For FAT16
		TableEntry = i - ((bpb->BPB_BytsPerSec/ 2) * (FATOffset / bpb->BPB_BytsPerSec));

		rc = ReadFat(bpb,fat_table,FATOffset);
		if(rc) goto end; // error

	 	if(!fat_table[TableEntry])break;

		FstClus++;


	}

	// dactive turbo
	fat_flag = 0;
	fat_ready = 0;
	

	name_size = strlen(filename) /*\0*/+ 1;

	dir_size = (name_size/13);
	if((name_size%13))dir_size++;

	// Procurando entrada NULL
	for(i = 0;i < 2048/*FIXME*/; i++) {

		if((dir->DIR_Name[0] == 0)){break;} // FIXME NULL
		dir++;
		n++;
	}

	if((i + dir_size) >= 2048){  
		rc = -1;
		goto end; // Sem espaco no directory raiz
	}


	n += dir_size;

	// Short name 8.3
	name = (char*)filename;
	_tmp = short_name; 
	for(i=0;i<8;i++) {
	
		if(*name == '.') *_tmp++ = ' ';
		else if(*name == '\0') *_tmp++ = ' ';
		else {
			if(*name >= 0x61) ch = *name++ - 0x20;
			else ch = *name++;
			*_tmp++ = ch;
		}
	}

	if(*name != '.' && (attr != 1) ) {

		_tmp--;
		_tmp--;
		*_tmp++ = '~';
		*_tmp++ = '1';

	}

	while(*name) {
		if(*name == '.')break;
	 	name++;
	}

	if(*name == '.') {

		name++;
		for(i=0;i<3;i++) {

			if(*name >= 0x61) ch = *name++ - 0x20;
			else ch = *name++;
			*_tmp++ = ch;

		}
	
	}else {

		for(i=0;i<3;i++) *_tmp++ = ' ';

	}

	// Escreve entrada Padrao
	_tmp = tmp;
	memset(_tmp,0,32);

	// Short name 8.3
	memcpy(_tmp,short_name,11);


	if(attr == 0)*(unsigned char*)(_tmp + 11) = FAT_ATTR_ARCHIVE;
	else if(attr == 1) *(unsigned char*)(_tmp + 11) = FAT_ATTR_DIRECTORY;

	*(unsigned char*)(_tmp + 12) = 0; // DIR_NTRes

	*(unsigned char*)(_tmp + 13) = 0; // DIR_CrtTimeTenth

	*(unsigned short*)(_tmp + 14) = 0; // DIR_CrtTime

	*(unsigned short*)(_tmp + 16) = 0; // DIR_CrtDate

	*(unsigned short*)(_tmp + 18) = 0; // DIR_LstAccDate

	*(unsigned short*)(_tmp + 20) = FstClus << 16; // DIR_FstClusHI

	*(unsigned short*)(_tmp + 22) = 0; // DIR_WrtTime

	*(unsigned short*)(_tmp + 24) = 0; // DIR_WrtDate

	*(unsigned short*)(_tmp + 26) = FstClus; // DIR_FstClusLO

	*(unsigned int*)(_tmp + 28) = 0; // DIR_FileSize

	
	sector_offset = (bpb->BPB_BytsPerSec * (n) / (bpb->BPB_BytsPerSec/32))/bpb->BPB_BytsPerSec + first_root_sector;
	entre_offset  = (n)%(bpb->BPB_BytsPerSec/32);

	rc = block_read(bpb->specific.fat.BS_DrvNum,1,sector_offset,buf);
	if(rc) goto end;

	memcpy(buf + (32 *entre_offset),tmp,32);

	rc = block_write(bpb->specific.fat.BS_DrvNum,1,sector_offset,buf);
	if(rc) goto end;

	// salvando a FAT
	rc = SalveFat(bpb,fat_table,FstClus*2,0xFFFFFFFF);
	if(rc)goto end;
	
	// if for sub-directory
	if(attr == 1) {
	// escreve o first directory '.'

		memset(first_dir,0,512);
		first_dir->DIR_Name[0] = '.';
		first_dir->DIR_Attr = FAT_ATTR_DIRECTORY;
		first_dir->DIR_FstClusHI = FstClus << 16;
		first_dir->DIR_FstClusLO = FstClus;

		first_dir++;
		first_dir->DIR_Name[0] = '.';
		first_dir->DIR_Name[1] = '.';
		first_dir->DIR_Attr = FAT_ATTR_DIRECTORY;

		if(1/*root directory*/) {
			first_dir->DIR_FstClusHI = 0;
			first_dir->DIR_FstClusLO = 0;
		} else {

			first_dir->DIR_FstClusHI = dir->DIR_FstClusHI;
			first_dir->DIR_FstClusLO = dir->DIR_FstClusLO;
		}


		first_dir--;

		// salve no disco
		// FirstSectorofCluster = ((N – 2) * BPB_SecPerClus) + FirstDataSector;
		unsigned int lba = first_sector_of_cluster = ((FstClus - 2) * bpb->BPB_SecPerClus) + first_data_sector;

		for(i=0;i< bpb->BPB_SecPerClus;i++) {
			rc = block_write(bpb->specific.fat.BS_DrvNum,1,lba+i,first_dir);
			if(rc) goto end;

			memset(first_dir,0,512);

		}

	

	}

	// Escreve entrada LFN

	name = (char*)filename;
	flag = 1;
	LDIR_Ord = 1;
	while(flag) {

	n--;
	
	_tmp = tmp;
	memset(_tmp,0,32);
	
	// LDIR_Name1
	_tmp = tmp + 1;
	for(i=0;i<5;i++) {

		if(flag != 0) {
			if(*name) { 
				*_tmp++ = *name++;
				*_tmp++ = 0;
			} else { 
				*_tmp++ = *name;
				*_tmp++ = 0; 
				flag = 0;
			}

		} else {	
			*_tmp++ = 0xFF;
			*_tmp++ = 0xFF;
			
		}
	}

	// LDIR_Name2
	_tmp = tmp + 14;
	for(i=0;i<6;i++) {
		if(flag != 0) {
			if(*name) { 
				*_tmp++ = *name++;
				*_tmp++ = 0;
			} else { 
				*_tmp++ = *name;
				*_tmp++ = 0; 
				flag = 0;
			}

		} else {	
			*_tmp++ = 0xFF;
			*_tmp++ = 0xFF;
			
		}
	}

	// LDIR_Name3
	_tmp = tmp + 28;
	for(i=0;i<2;i++) {
		if(flag != 0) {
			if(*name) { 
				*_tmp++ = *name++;
				*_tmp++ = 0;
			} else { 
				*_tmp++ = *name;
				*_tmp++ = 0; 
				flag = 0;
			}

		} else {	
			*_tmp++ = 0xFF;
			*_tmp++ = 0xFF;
			
		}
	}
	
	if(!flag) *(unsigned char*)(tmp) = 0x40 | LDIR_Ord; // LDIR_Ord   0x40 LAST_LONG_ENTRY
	else *(unsigned char*)(tmp) = LDIR_Ord; 

	*(unsigned char*)(tmp + 11) = FAT_ATTR_LONG_NAME;
	*(unsigned char*)(tmp + 13) = ChkSum ((unsigned char *)short_name); // LDIR_Chksum

	sector_offset = (bpb->BPB_BytsPerSec * (n) / (bpb->BPB_BytsPerSec/32))/bpb->BPB_BytsPerSec + first_root_sector;
	entre_offset  = (n)%(bpb->BPB_BytsPerSec/32);

	rc = block_read(bpb->specific.fat.BS_DrvNum,1,sector_offset,buf);
	if(rc) goto end;

	memcpy(buf + (32 *entre_offset),tmp,32);

	rc = block_write(bpb->specific.fat.BS_DrvNum,1,sector_offset,buf);
	if(rc) goto end;

	
	LDIR_Ord++;
}



	
end:
	
	free(first_dir);
	free(buf);
	free(tmp);
	free(fat_table);

	return rc;
}


int FatRemveFile(FAT_BPB *bpb,FAT_DIRECTORY *_dir,const char *filename) 
{
	FAT_DIRECTORY *dir = _dir;
	char shortname[11];
	unsigned short *fat_table =(unsigned short*) malloc(0x1000);
	char *lonf_file_name = (char*)malloc(256);
	int i;
	unsigned char sum = 0;
	int count = 0;
	unsigned char *cur_dir = 0;


	unsigned int N = 0;
	int FATOffset = 0;
	int TableEntry = 0;


	unsigned char *buf = (unsigned char *)malloc(0x1000);


	// DATA
	unsigned int sector_of_cluster = 0;

	// calcular o total numero de sectores Root Directory, RootDirSectors FAT12/16
	unsigned int root_sectors = ((bpb->BPB_RootEntCnt * 32) + (bpb->BPB_BytsPerSec - 1)) / bpb->BPB_BytsPerSec;

	// Calcular o 1º sector de dados, FirstDataSector FAT12/16
	unsigned int first_data_sector = bpb->BPB_RsvdSecCnt +\
	(bpb->BPB_FATSz16 * bpb->BPB_NumFATs) + root_sectors + bpb->BPB_HiddSec;


	// Calcular o 1º sector do Root Directory, FirstRootDirSector FAT12/16
	unsigned int first_root_sector = (first_data_sector - root_sectors);


	unsigned int *dir_salve_num = (unsigned int*) malloc(0x1000);


	// Comparar ate achar o SHORT NAME
	FileShortName(shortname,(char*)filename);

	count = 0;
	for(i = 0;i < 512/*FIXME*/; i++) {

		if((dir->DIR_Name[0] ==0)) {
			free(lonf_file_name); 
			free(fat_table);
			return -1;

		}
		else if((dir->DIR_Name[0] == 0xE5)) { dir++; continue; } // Avaliable
		else if((dir->DIR_Name[0] == 0x20)) { dir++; continue; } // space
		else if((dir->DIR_Attr == FAT_ATTR_LONG_NAME)) {


			// Fixme Nelson devemos comparar Chksum FIXME aqui
			cur_dir = (unsigned char*)(dir);

			sum = cur_dir[13];
			
			dir_salve_num[count++] = i;

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
			if(!(strcmp (lonf_file_name,filename))) { 
				dir_salve_num[count++] = i; 
				goto successfull;
			}

		}
	
		// compara short name
		if( !(strncmp (dir->DIR_Name,shortname,11)) ) { 
			dir_salve_num[count++] = i; 
			goto successfull;
		}
		else { count = 0; dir++;}
	}
	
	free(dir_salve_num);
	free(lonf_file_name);
	free(fat_table);
	return -1;

successfull:

	if(dir->DIR_Attr == FAT_ATTR_DIRECTORY) {

		// FIXME, devemos dar suport a remoçao de directório
		free(dir_salve_num);
		free(lonf_file_name);
		free(fat_table);
		return -1;	

	}


	N = dir->DIR_FstClusLO | dir->DIR_FstClusHI << 16;

	// remover entrada de directorio 

	int dir_start = (int) _dir;

	for(i=0;i < count;i++) {

		dir = (FAT_DIRECTORY*) (dir_start +(dir_salve_num[i] * 32) );

		// marcar com 0xE5
		dir->DIR_Name[0] = 0xE5;
		dir->DIR_Attr	= 0;

		dir->DIR_FstClusHI = dir->DIR_FstClusLO = 0;

		//actualiza directorio raiz
		int sector_offset = (bpb->BPB_BytsPerSec *\
		(dir_salve_num[i]) / (bpb->BPB_BytsPerSec/32))/bpb->BPB_BytsPerSec + first_root_sector;

		int entre_offset  = (dir_salve_num[i])%(bpb->BPB_BytsPerSec/32);

		if( block_read(bpb->specific.fat.BS_DrvNum,1,sector_offset,buf) ){free(lonf_file_name); free(buf); return -1;}

		memcpy(buf + (32 *entre_offset),dir,32);

		if( block_write(bpb->specific.fat.BS_DrvNum,1,sector_offset,buf) ){free(lonf_file_name); free(buf); return -1;}

	}

	free(buf);

	unsigned char flag = 0;

	while(1) {

		sector_of_cluster = ((N - 2) * bpb->BPB_SecPerClus) + first_data_sector;

		FATOffset = N *2; // For FAT16

		if(!(flag)) {
			if(ReadFat(bpb,fat_table,FATOffset)) {

				free(dir_salve_num);
				free(fat_table);
				free(lonf_file_name);
				return -1;


			}

			flag = 1;

		}

		if(RemoveFAT(bpb,sector_of_cluster)) {

			free(dir_salve_num);
			free(fat_table);
			free(lonf_file_name);
			return -1;
		}


		TableEntry = N - ((bpb->BPB_BytsPerSec/ 2) * (FATOffset / bpb->BPB_BytsPerSec));		

		N = fat_table[TableEntry];

		//For FAT16
		if( (N == 0xFFFF) || (N == 0xFFF8) ) break;

		//For FAT16
		if(TableEntry >= ((bpb->BPB_BytsPerSec/2)-1)) flag = 0;


	}


	free(dir_salve_num);
	free(fat_table);
	free(lonf_file_name);
	return 0;
}



int FatCheckFile(FAT_BPB *bpb,FAT_DIRECTORY *_dir,const char *filename) 
{
	FAT_DIRECTORY *dir = _dir;
	char shortname[11];

	char *lonf_file_name = (char*)malloc(256);
	int i;
	unsigned char sum = 0;
	int count = 0;
	unsigned char *cur_dir = 0;

	// Comparar ate achar o SHORT NAME
	FileShortName(shortname,(char*)filename);

	count = 0;
	for(i = 0;i < 512/*FIXME*/; i++) {

		if((dir->DIR_Name[0] ==0)) {
			free(lonf_file_name); 
			return -1;

		}
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
	
	free(lonf_file_name);
	return -1;

successfull:

	free(lonf_file_name);
	return 0;
}


int FatOpenFile(FAT_BPB *bpb,FAT_DIRECTORY *_dir,const char *filename,unsigned char attr,VFS *fd) 
{
	VFS *vfs = fd;
	FAT_DIRECTORY *dir = _dir;
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



	// DATA
	unsigned int first_sector_of_cluster = 0;

	// calcular o total numero de sectores Root Directory, RootDirSectors FAT12/16
	unsigned int root_sectors = ((bpb->BPB_RootEntCnt * 32) + (bpb->BPB_BytsPerSec - 1)) / bpb->BPB_BytsPerSec;

	// Calcular o 1º sector de dados, FirstDataSector FAT12/16
	unsigned int first_data_sector = bpb->BPB_RsvdSecCnt +\
	(bpb->BPB_FATSz16 * bpb->BPB_NumFATs) + root_sectors + bpb->BPB_HiddSec;




	// Directory Current
	if((*filename == '.') && (attr == ATTR_DIRECTORY)) goto successfull;


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

	switch(attr) {

	case ATTR_ARCHIVE:
	// Aqui podemos carregar o VFS
	strcpy((char*)vfs->header.filename,filename);

	vfs->header.attr 	= ATTR_ARCHIVE;
	vfs->header.size 	= dir->DIR_FileSize;
	vfs->header.dev		= bpb->specific.fat.BS_DrvNum;
	vfs->header.bps 	= bpb->BPB_BytsPerSec;
	vfs->header.count	= bpb->BPB_SecPerClus;
	vfs->header.blocks	= 0;


	//UINT32 *block		= (UINT32*)(vfs->block);

	N = dir->DIR_FstClusLO | dir->DIR_FstClusHI << 16;
	
	i = 0;
	unsigned char flag = 0;
	while(1/*TRUE*/) {

		// FirstSectorofCluster = ((N – 2) * BPB_SecPerClus) + FirstDataSector;
		lba = first_sector_of_cluster = ((N - 2) * bpb->BPB_SecPerClus) + first_data_sector;
	
		vfs->block[i] = lba; 

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

		vfs->header.blocks += 1;

		if( (N == 0xFFFF) || (N == 0xFFF8) ) break;
		//For FAT16
		if(TableEntry >= ((bpb->BPB_BytsPerSec/2)-1)) flag = 0;
		 
		i++;
	
	}
		break;
	case ATTR_DIRECTORY:
	strcpy((char*)vfs->header.filename,filename);
	vfs->header.attr 	= ATTR_DIRECTORY;
	vfs->header.size 	= dir->DIR_FileSize;
	vfs->header.dev		= bpb->specific.fat.BS_DrvNum;
	vfs->header.bps 	= bpb->BPB_BytsPerSec;
	vfs->header.count	= 0;
	vfs->header.blocks	= 0;
	


	for(i = 0;i < 512/*FIXME*/; i++) {

		if((dir->DIR_Name[0] == 0)){break;}
		else if((dir->DIR_Name[0] == 0xE5)) { dir++; continue; } // Avaliable
		else if((dir->DIR_Name[0] == 0x20)) { dir++; continue; } // space
		else if((dir->DIR_Attr == FAT_ATTR_LONG_NAME)) {

			cur_dir = (unsigned char*)dir;
			sum = cur_dir[13];

			count++;

			dir++; continue;

		}
		else if((dir->DIR_Attr == FAT_ATTR_HIDDEN)) { dir++; continue; }
		else if((dir->DIR_Attr == FAT_ATTR_SYSTEM)) { dir++; continue; }
		else if((dir->DIR_Attr == FAT_ATTR_VOLUME_ID)) { dir++; continue; }
		else if((dir->DIR_Attr != FAT_ATTR_DIRECTORY) &&\
	 	(dir->DIR_Attr != FAT_ATTR_ARCHIVE)) { dir++; continue; }
		

		// default 8.3
		
		// ChkSum (unsigned char *pFcbName)
		if(sum == ChkSum ((unsigned char*)dir->DIR_Name)) {

			// Estrair Long File Name

			char *p = (char*) (vfs->header.buffer + (vfs->header.blocks*64));
			memset(p,0,64);


			FATReadLFN(p,--dir,count);
			++dir;


			*(unsigned short*)(p + 60) = 0;
			*(unsigned short*)(p + 62) = 0;

			vfs->header.blocks++;
		
			count = 0; dir++;
			continue;


		}
		
		// Entrada padrao 8.3
		char *p = (char*) (vfs->block + (vfs->header.blocks*64));
		memset(p,0,64);

		FileMountShortName(p,(const char *)dir->DIR_Name);

		*(unsigned short*)(p + 60) = 0;
		*(unsigned short*)(p + 62) = 0;

		vfs->header.blocks++;

		count = 0; dir++;
	}


		break;

	default:
		
		goto error;
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



int FatUpdateFile(FAT_BPB *bpb,FAT_DIRECTORY *_dir,VFS *fd) 
{
	VFS *vfs = fd;
	FAT_DIRECTORY *dir = _dir;
	char shortname[11];

	unsigned int sector_offset = 0;
	unsigned int entre_offset  = 0;

	unsigned char *buf = (unsigned char *)malloc(0x1000);

	char *lonf_file_name = (char*)malloc(256);
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
	FileShortName(shortname,(const char *)vfs->header.filename);

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
			if(!(strcmp (lonf_file_name,(const char *)vfs->header.filename))) goto successfull;

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
	dir->DIR_FileSize = vfs->header.size;
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
