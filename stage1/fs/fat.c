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

#include <io.h>

// 1 Read BPB
// 2 Read Root
// 3 Read FAT
// 4 Funcao FatOpenFile(in dev_num, in filename,out vfs)
VOID *FAT = NULL;

static VOID FileShortName(CHAR8* shortname,CONST CHAR8* filename)
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
}

FAT_BPB *FatReadBPB(VOLUME *volume) 
{
	VOLUME *v = volume;
	
	FAT_BPB *bpb = (FAT_BPB*)alloc(1);


	if(read_sector(v->dev_num,1,v->lba_start,bpb))
		return NULL;

	// configure device number
	bpb->specific.fat.BS_DrvNum = v->dev_num;

	return ((FAT_BPB*) bpb);

}


FAT_DIRECTORY *FatOpenRoot(FAT_BPB *_bpb,FAT_DATA *data)
{
	FAT_DIRECTORY *root = (FAT_DIRECTORY *)alloc(4); //16 KiB = 512 entradas
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

	UINTN i;
	// Comparar ate achar o SHORT NAME
	FileShortName(shortname,(CHAR8*)filename);

	for(i = 0;i < 128/*FIXME*/; i++) {

		if((dir->DIR_Attr == FAT_ATTR_LONG_NAME)) { dir++; continue; }

		if( !(strncmp (dir->DIR_Name,shortname,11)) )
			goto successfull;
		else dir++;
	}
	
	return -1;

successfull:

	// Aqui podemos carregar o VFS
	strncpy(vfs->header.filename,dir->DIR_Name,11);
	vfs->header.attr 	= dir->DIR_Attr;
	vfs->header.size 	= dir->DIR_FileSize;
	vfs->header.dev		= bpb->specific.fat.BS_DrvNum;
	vfs->header.bps 	= bpb->BPB_BytsPerSec;
	vfs->header.count	= bpb->BPB_SecPerClus;
	vfs->header.blocks	= 1;

	N = dir->DIR_FstClusLO;// | dir->DIR_FstClusHI <<16;
	
	i = 0;
	UINT8 flag = 0;
	while(TRUE) {

		// FirstSectorofCluster = ((N – 2) * BPB_SecPerClus) + FirstDataSector;
		lba = data->first_sector_of_cluster = ((N - 2) * bpb->BPB_SecPerClus) + data->first_data_sector;
	
		vfs->block[i] = lba; 

		FATOffset = N *2;

		if(!(flag)) {
			if(ReadFat(bpb,fat_table,FATOffset))
				return -1;

			flag = 1;

		}
		// Para FAT16 divide por 2
		// TableEntry = N - ((BPB_BytsPerSec/ 2) * (FATOffset / BPB_BytsPerSec));
		TableEntry = N - ((bpb->BPB_BytsPerSec/ 2) * (FATOffset / bpb->BPB_BytsPerSec));		

		N = fat_table[TableEntry];

		if( (N == 0xFFFF) || (N == 0xFFF8) ) break;

		if(!(TableEntry)) flag = 0;

		i++;
	
	}


	vfs->header.blocks += i;

	return 0;
}
