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


#include <os.h>

// 1 Read BPB
// 2 Read Root
// 3 Read FAT
// 4 Funcao FatOpenFile(in dev_num, in filename,out vfs)
VOID *FAT = NULL;

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


VOID FileShortName(CHAR8* shortname,CONST CHAR8* filename)
{
	CHAR8* _filename = (CHAR8*)filename;
	CHAR8* _shortname = shortname;
	UINTN i;

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


	if(read_sector(v->dev_num,1,v->lba_start,bpb))
		return NULL;

	// configure device number
	bpb->specific.fat.BS_DrvNum = v->dev_num;

	return ((FAT_BPB*) bpb);

}

void FATDataUpdate(FAT_BPB *bpb,FAT_DATA *data) 
{

	// calcular o total numero de sectores Root Directory, RootDirSectors FAT12/16
	data->root_sectors = ((bpb->BPB_RootEntCnt * 32) + (bpb->BPB_BytsPerSec - 1)) / bpb->BPB_BytsPerSec;

	// Calcular o 1º sector de dados, FirstDataSector FAT12/16
	data->first_data_sector = bpb->BPB_RsvdSecCnt +\
	(bpb->BPB_FATSz16 * bpb->BPB_NumFATs) + data->root_sectors + bpb->BPB_HiddSec;

	// Calcular o 1º sector do Root Directory, FirstRootDirSector FAT12/16
	data->first_root_sector = (data->first_data_sector - data->root_sectors);


}



FAT_DIRECTORY *FatOpenRoot(FAT_BPB *_bpb,FAT_DATA *data)
{
	FAT_DIRECTORY *root = (FAT_DIRECTORY *)malloc(0x10000); //64 KiB = 2048 entradas
	FAT_BPB *bpb = _bpb;

	setmem(root,0x10000,0);


	//64 KiB = 2048 entradas

	alloc_pages(0,64,(VIRTUAL_ADDRESS *)&root);

	// calcular o total numero de sectores Root Directory, RootDirSectors FAT12/16
	data->root_sectors = ((bpb->BPB_RootEntCnt * 32) + (bpb->BPB_BytsPerSec - 1)) / bpb->BPB_BytsPerSec;
	

	// Calcular o 1º sector de dados, FirstDataSector FAT12/16
	data->first_data_sector = bpb->BPB_RsvdSecCnt +\
	(bpb->BPB_FATSz16 * bpb->BPB_NumFATs) + data->root_sectors + bpb->BPB_HiddSec;

	

	// Calcular o 1º sector do Root Directory, FirstRootDirSector FAT12/16
	data->first_root_sector = (data->first_data_sector - data->root_sectors);


	// Carregar o Root Directory na memorio, FAT12/16
	if(read_sector(bpb->specific.fat.BS_DrvNum,data->root_sectors,data->first_root_sector,root))
		return NULL;


	return ((FAT_DIRECTORY*) root);

}

FAT_DIRECTORY *FatUpdateRoot(FAT_BPB *_bpb,FAT_DATA *data,FAT_DIRECTORY *root)
{
	FAT_BPB *bpb = _bpb;

	// calcular o total numero de sectores Root Directory, RootDirSectors FAT12/16
	data->root_sectors = ((bpb->BPB_RootEntCnt * 32) + (bpb->BPB_BytsPerSec - 1)) / bpb->BPB_BytsPerSec;

	// Calcular o 1º sector de dados, FirstDataSector FAT12/16
	data->first_data_sector = bpb->BPB_RsvdSecCnt +\
	(bpb->BPB_FATSz16 * bpb->BPB_NumFATs) + data->root_sectors + bpb->BPB_HiddSec;

	// Calcular o 1º sector do Root Directory, FirstRootDirSector FAT12/16
	data->first_root_sector = (data->first_data_sector - data->root_sectors);

	// Carregar o Root Directory na memorio, FAT12/16
	if(read_sector(bpb->specific.fat.BS_DrvNum,data->root_sectors,data->first_root_sector,root))
		return NULL;


	return ((FAT_DIRECTORY*) root);

}

UINT32 ReadFat(FAT_BPB *_bpb,OUT VOID *fat_table,UINTN FATOffset)
{
	FAT_BPB *bpb = _bpb;

	// ThisFATSecNum = BPB_ResvdSecCnt + (FATOffset / BPB_BytsPerSec);
	UINT32 	ThisFATSecNum = bpb->BPB_RsvdSecCnt + (FATOffset / bpb->BPB_BytsPerSec) + bpb->BPB_HiddSec;


	if(read_sector(bpb->specific.fat.BS_DrvNum,1,ThisFATSecNum,fat_table))
		return -1;

	return 0;

}

UINT32 SalveFat(FAT_BPB *_bpb,OUT VOID *_fat_table,UINTN FATOffset,UINT32 val)
{
	FAT_BPB *bpb = _bpb;
	UINT16 *fat_table = (UINT16*)_fat_table;
	UINTN TableEntry;

	// ThisFATSecNum = BPB_ResvdSecCnt + (FATOffset / BPB_BytsPerSec);
	UINT32 	ThisFATSecNum = bpb->BPB_RsvdSecCnt + (FATOffset / bpb->BPB_BytsPerSec) + bpb->BPB_HiddSec;


	if(read_sector(bpb->specific.fat.BS_DrvNum,1,ThisFATSecNum,fat_table))
		return -1;

	// Para FAT16 divide por 2
	// TableEntry = N - ((BPB_BytsPerSec/ 2) * (FATOffset / BPB_BytsPerSec));
	TableEntry = (FATOffset /2) - ((bpb->BPB_BytsPerSec/ 2) * (FATOffset / bpb->BPB_BytsPerSec));
	fat_table[TableEntry] = val &0xFFFF;
	

	if(write_sector(bpb->specific.fat.BS_DrvNum,1,ThisFATSecNum,fat_table))
		return -1;


	return 0;

}

int AddFAT(VFS *vfs, FAT_BPB *bpb, unsigned int first_sector_of_cluster) 
{

	FAT_DATA *data = (FAT_DATA *)malloc(sizeof(FAT_DATA));
	unsigned short *fat_table =(unsigned short*) (FAT);

	unsigned int FATOffset = 0;
	unsigned int TableEntry = 0;
	unsigned int current_n =0;
	unsigned int new_n =0;
	int i;

	FATDataUpdate(bpb,data);
	// FirstSectorofCluster = ((N – 2) * BPB_SecPerClus) + FirstDataSector;
	unsigned int n = (((first_sector_of_cluster - data->first_data_sector)/bpb->BPB_SecPerClus) + 2 );

	// Ler a FAT, achar uma entrada NULL

	FATOffset = n *2; // For FAT16
	TableEntry = n - ((bpb->BPB_BytsPerSec/ 2) * (FATOffset / bpb->BPB_BytsPerSec));

	if(ReadFat(bpb,fat_table,FATOffset)) {free(data); return EOF;} //ERROR

	// For FAT16
	if(fat_table[TableEntry] != (EOF&0xffff)) {free(data); return EOF;} //ERROR

	// salve 
	current_n = n;
	n = 3;
	// sondar por uma entrada NULL
	for(i =3;i < 65536/*FIXME FATsize*/;i++) { //For FAT16

		FATOffset = i *2; // For FAT16
		TableEntry = i - ((bpb->BPB_BytsPerSec/ 2) * (FATOffset / bpb->BPB_BytsPerSec));

		if(ReadFat(bpb,fat_table,FATOffset) ) {free(data); return EOF;} //ERROR

	 	if(!fat_table[TableEntry])break;

		n++;


	}
	
	//salve
	new_n += n;

	// escreve o fat
	FATOffset = current_n *2; // For FAT16
	if(SalveFat(bpb,fat_table,FATOffset,new_n)) {free(data); return EOF;} //ERROR

	FATOffset = new_n *2; // For FAT16
	if(SalveFat(bpb,fat_table,FATOffset,EOF)) {free(data); return EOF;} //ERROR


	if(vfs->header.blocks >= 896) {
		
		//FIXME aqui devemos adicionar um novo bloco de 4KiB no *vfs 

		free(data); return EOF;
	}
	else {
		// FirstSectorofCluster = ((N – 2) * BPB_SecPerClus) + FirstDataSector;
		vfs->block[vfs->header.blocks] = data->first_sector_of_cluster = \
		((new_n - 2) * bpb->BPB_SecPerClus) + data->first_data_sector;

		vfs->header.blocks++;

	}


	free(data);
	return 0;

}

UINTN FatCreateFile(FAT_BPB *_bpb,FAT_DATA *data,FAT_DIRECTORY *_dir,CONST CHAR8 *filename,UINT8 attr)
{
	CHAR8 short_name[11];
	CHAR8 *tmp = (CHAR8*)malloc(0x1000);
	CHAR8 *buf = (CHAR8*)malloc(0x1000);
	FAT_DIRECTORY *first_dir = (FAT_DIRECTORY *)malloc(0x1000);
	UINT16 *fat_table =(UINT16*) (FAT);
	FAT_DIRECTORY *dir = _dir;
	FAT_BPB *bpb = _bpb;
	CHAR8 *_tmp;
	CHAR8 *name = (CHAR8*)filename;
	UINTN name_size =0;
	UINTN dir_size =0;
	UINT32 sector_offset = 0;
	UINT32 entre_offset  = 0;

	UINTN  FATOffset = 0;
	UINTN  TableEntry = 0;
	UINT32 FstClus = 0;

	UINTN rc = 0;	
	CHAR8 ch = 0;

	UINTN i,n =0;
	UINTN LDIR_Ord = 0;
	UINT8 flag = 0;


	FstClus = 3;
	FATOffset = 0;

	rc = ReadFat(bpb,fat_table,FATOffset);
	if(rc) goto end; // error

	for(i =3;i < 65536/*FIXME FATsize*/;i++) { //For FAT16

		FATOffset = i *2; // For FAT16
		TableEntry = i - ((bpb->BPB_BytsPerSec/ 2) * (FATOffset / bpb->BPB_BytsPerSec));

		rc = ReadFat(bpb,fat_table,FATOffset);
		if(rc) goto end; // error

	 	if(!fat_table[TableEntry])break;

		FstClus++;


	}
	

	name_size = strlen(filename);

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
	name = (CHAR8*)filename;
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
	setmem(_tmp,32,0);

	// Short name 8.3
	copymem(_tmp,short_name,11);


	if(attr == 0)*(UINT8*)(_tmp + 11) = FAT_ATTR_ARCHIVE;
	else if(attr == 1) *(UINT8*)(_tmp + 11) = FAT_ATTR_DIRECTORY;

	*(UINT8*)(_tmp + 12) = 0; // DIR_NTRes

	*(UINT8*)(_tmp + 13) = 0; // DIR_CrtTimeTenth

	*(UINT16*)(_tmp + 14) = 0; // DIR_CrtTime

	*(UINT16*)(_tmp + 16) = 0; // DIR_CrtDate

	*(UINT16*)(_tmp + 18) = 0; // DIR_LstAccDate

	*(UINT16*)(_tmp + 20) = FstClus << 16; // DIR_FstClusHI

	*(UINT16*)(_tmp + 22) = 0; // DIR_WrtTime

	*(UINT16*)(_tmp + 24) = 0; // DIR_WrtDate

	*(UINT16*)(_tmp + 26) = FstClus; // DIR_FstClusLO

	*(UINT32*)(_tmp + 28) = 0; // DIR_FileSize

	
	sector_offset = (bpb->BPB_BytsPerSec * (n) / (bpb->BPB_BytsPerSec/32))/bpb->BPB_BytsPerSec + data->first_root_sector;
	entre_offset  = (n)%(bpb->BPB_BytsPerSec/32);

	rc = read_sector(bpb->specific.fat.BS_DrvNum,1,sector_offset,buf);
	if(rc) goto end;

	copymem(buf + (32 *entre_offset),tmp,32);

	rc = write_sector(bpb->specific.fat.BS_DrvNum,1,sector_offset,buf);
	if(rc) goto end;

	// salvando a FAT
	rc = SalveFat(bpb,fat_table,FstClus*2,0xFFFFFFFF);
	if(rc)goto end;
	
	// if for sub-directory
	if(attr == 1) {
	// escreve o first directory '.'

		setmem(first_dir,512,0);
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
		UINT32 lba = data->first_sector_of_cluster = ((FstClus - 2) * bpb->BPB_SecPerClus) + data->first_data_sector;

		for(i=0;i< bpb->BPB_SecPerClus;i++) {
			rc = write_sector(bpb->specific.fat.BS_DrvNum,1,lba+i,first_dir);
			if(rc) goto end;

			setmem(first_dir,512,0);

		}

	

	}

	// Escreve entrada LFN

	name = (CHAR8*)filename;
	flag = 1;
	LDIR_Ord = 1;
	while(flag) {

	n--;
	
	_tmp = tmp;
	setmem(_tmp,32,0);
	
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
	
	if(!flag) *(UINT8*)(tmp) = 0x40 | LDIR_Ord; // LDIR_Ord   0x40 LAST_LONG_ENTRY
	else *(UINT8*)(tmp) = LDIR_Ord; 

	*(UINT8*)(tmp + 11) = FAT_ATTR_LONG_NAME;
	*(UINT8*)(tmp + 13) = ChkSum ((unsigned char *)short_name); // LDIR_Chksum

	sector_offset = (bpb->BPB_BytsPerSec * (n) / (bpb->BPB_BytsPerSec/32))/bpb->BPB_BytsPerSec + data->first_root_sector;
	entre_offset  = (n)%(bpb->BPB_BytsPerSec/32);

	rc = read_sector(bpb->specific.fat.BS_DrvNum,1,sector_offset,buf);
	if(rc) goto end;

	copymem(buf + (32 *entre_offset),tmp,32);

	rc = write_sector(bpb->specific.fat.BS_DrvNum,1,sector_offset,buf);
	if(rc) goto end;

	
	LDIR_Ord++;
}



	
end:
	

	
	free(first_dir);
	free(buf);
	free(tmp);

	return rc;
}

UINTN FatOpenFile(FAT_BPB *_bpb,FAT_DATA *data,FAT_DIRECTORY *_dir,CONST CHAR8 *filename,UINT8 attr,VFS *_vfs) 
{
	VFS *vfs = _vfs;
	FAT_BPB *bpb = _bpb;
	FAT_DIRECTORY *dir = _dir;
	CHAR8 shortname[11];
	UINT32 lba = 0;
	UINT32 N = 0;
	UINTN FATOffset = 0;
	UINTN TableEntry = 0;
	UINT16 *fat_table =(UINT16*) (FAT);

	char *lonf_file_name = (char*)malloc(256);
	int i;
	unsigned char sum = 0;
	int count = 0;
	unsigned char *cur_dir = NULL;


	// Directory Current
	if((*filename == '.') && (attr == ATTR_DIRECTORY)) goto successfull;


	// Comparar ate achar o SHORT NAME
	FileShortName(shortname,(CHAR8*)filename);

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
	strcpy(vfs->header.filename,filename);

	vfs->header.attr 	= ATTR_ARCHIVE;
	vfs->header.size 	= dir->DIR_FileSize;
	vfs->header.dev		= bpb->specific.fat.BS_DrvNum;
	vfs->header.bps 	= bpb->BPB_BytsPerSec;
	vfs->header.count	= bpb->BPB_SecPerClus;
	vfs->header.blocks	= 0;


	//UINT32 *block		= (UINT32*)(vfs->block);

	N = dir->DIR_FstClusLO | dir->DIR_FstClusHI << 16;
	
	i = 0;
	UINT8 flag = 0;
	while(TRUE) {

		// FirstSectorofCluster = ((N – 2) * BPB_SecPerClus) + FirstDataSector;
		lba = data->first_sector_of_cluster = ((N - 2) * bpb->BPB_SecPerClus) + data->first_data_sector;
	
		vfs->block[i] = lba; 

		FATOffset = N *2; // For FAT16

		if(!(flag)) {
			if(ReadFat(bpb,fat_table,FATOffset))
				goto error;

			//flag = 1;

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
	strcpy(vfs->header.filename,filename);
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

			CHAR8 *p = (CHAR8*) (vfs->block + (vfs->header.blocks*64));
			setmem(p,64,0);


			FATReadLFN(p,--dir,count);
			++dir;


			*(UINT16*)(p + 60) = 0;
			*(UINT16*)(p + 62) = 0;

			vfs->header.blocks++;
		
			count = 0; dir++;
			continue;


		}
		
		// Entrada padrao 8.3
		CHAR8 *p = (CHAR8*) (vfs->block + (vfs->header.blocks*64));
		setmem(p,64,0);

		FileMountShortName(p,(const char *)dir->DIR_Name);

		*(UINT16*)(p + 60) = 0;
		*(UINT16*)(p + 62) = 0;

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
	return -1;
done:
	free(lonf_file_name);	
	return 0;
}



UINTN FatUpdateFile(FAT_BPB *_bpb,FAT_DATA *data,FAT_DIRECTORY *_dir,VFS *_vfs) 
{
	VFS *vfs = _vfs;
	FAT_BPB *bpb = _bpb;
	FAT_DIRECTORY *dir = _dir;
	CHAR8 shortname[11];

	UINT32 sector_offset = 0;
	UINT32 entre_offset  = 0;

	UINT8 *buf = (UINT8 *)malloc(0x1000);

	char *lonf_file_name = (char*)malloc(256);
	int i;
	unsigned char sum = 0;
	int count = 0;
	unsigned char *cur_dir;
	// Comparar ate achar o SHORT NAME
	FileShortName(shortname,vfs->header.filename);

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
			if(!(strcmp (lonf_file_name,vfs->header.filename))) goto successfull;

		}
	
		// compara short name
		if( !(strncmp (dir->DIR_Name,shortname,11)) ) goto successfull;
		else { count = 0; dir++;}
	}
	
	{free(lonf_file_name); free(buf); return -1;}

successfull:

	// Aqui podemos actualizar o directory padrao
	dir->DIR_FileSize = vfs->header.size;
	//salvar no disco

	sector_offset = (bpb->BPB_BytsPerSec * (i) / (bpb->BPB_BytsPerSec/32))/bpb->BPB_BytsPerSec + data->first_root_sector;
	entre_offset  = (i)%(bpb->BPB_BytsPerSec/32);

	if( read_sector(bpb->specific.fat.BS_DrvNum,1,sector_offset,buf) ){free(lonf_file_name); free(buf); return -1;}

	copymem(buf + (32 *entre_offset),dir,32);

	if( write_sector(bpb->specific.fat.BS_DrvNum,1,sector_offset,buf) ){free(lonf_file_name); free(buf); return -1;}
	
	free(buf);
	free(lonf_file_name);
	return 0;
}
