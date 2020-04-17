/*
 * File Name: format.c
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
 

/* 	Esta ferramenta tem o foco de formatar partições de disco */


#include <sys/sys.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <fs/fat.h>


#define OEM "Sirius \0"
#define VOLLAB "Sirius v2.0"
#define FILESYSTEM_FAT12 "FAT12   "
#define FILESYSTEM_FAT16 "FAT16   "
#define FILESYSTEM_FAT32 "FAT32   "



FAT_BPB	*Super_Block = NULL;
SD dev[2];


int main(int argc, const char **argv)
{

	int i;

	fputs("Format disk (Sirius) version 0.0.1\n",stdout);

	if(argc < 1) return (-1);


	Super_Block = (FAT_BPB	*) malloc(0x1000);
	memset(Super_Block,0,0x1000);

	char sdx[8];
	char sdxn[8];
	memset(sdx,0,8);
	memset(sdxn,0,8);


	FILE *fp = fopen(argv[1],"r+");

	if(!fp) {

		fputs("error: fopen()\n",stderr);
		free(Super_Block);
		return (-1);
	}


	char *str = (char *) argv[1];

	str++;
	str = strchr(str,'/');
	memcpy(sdx,++str,3);	//sdx
	str = strchr(str,'/');
	memcpy(sdxn,++str,4); //sdxn


	SD *_dev = read_sdx(sdx);
	_dev = read_sdn(sdxn,_dev);

	if(!_dev) {

		fputs("error: dev \n",stderr);
		free(Super_Block);
		fclose(fp);
		return (-1);
	}

	memcpy(dev,_dev,32);


	
	//Super_Block->BS_jmpBoot[3];
	memcpy(Super_Block->BS_OEMName,OEM,8);
	Super_Block->BPB_BytsPerSec	= dev->byte_of_sector;
	Super_Block->BPB_SecPerClus	= 8;
	Super_Block->BPB_RsvdSecCnt	= 2;	//  = BPB + BPB_FSInfo
	Super_Block->BPB_NumFATs	= 2;

	Super_Block->BPB_RootEntCnt	= 512; // for FAT 12 and 16, 0 if FAT 32
	
	Super_Block->BPB_Media		= 0xf8; /* 0xF8 is the standard value for “fixed” (non-removable) media, 
						for removable media, 0xF0 */

	Super_Block->BPB_SecPertrk	= 0;
	Super_Block->BPB_NumHeads	= 0;

	Super_Block->BPB_HiddSec	= dev->lba_start;


	/*if(dev->num_sectors <= 65536) {

		Super_Block->BPB_TotSec16	= dev->num_sectors;
		Super_Block->BPB_TotSec32	= 0;

	}else { 


		Super_Block->BPB_TotSec16	= 0;
		Super_Block->BPB_TotSec32	= dev->num_sectors;


	}*/

	Super_Block->BPB_TotSec16	= 0;
	Super_Block->BPB_TotSec32	= dev->num_sectors;



	// calcular o total numero de sectores Root Directory, RootDirSectors FAT12/16
	unsigned int root_sectors = ((Super_Block->BPB_RootEntCnt * 32) \
	+ (Super_Block->BPB_BytsPerSec - 1)) / Super_Block->BPB_BytsPerSec;
	
	unsigned int data_sector = dev->num_sectors - (Super_Block->BPB_RsvdSecCnt \
	+ (Super_Block->BPB_NumFATs * 1/*FATSz*/) + root_sectors);
	
	unsigned long TotClus = data_sector/Super_Block->BPB_SecPerClus;

	int FatType = 0;

	if (TotClus < 4085 ) {
        	FatType = FAT12;

    	} else if (TotClus < 65525 ) {
        	FatType = FAT16;

    	}
    	else if (TotClus < 268435445) {
        	FatType = FAT32;

		Super_Block->BPB_RootEntCnt = 0;

    	} else {
        	FatType = ExFAT;
        	fputs("error: Volume muito grande, deve ser formatado em ExFAT\n",stderr);
		free(Super_Block);
		fclose(fp);
		return (-1);
    	}


	unsigned int root_dir_sectors = (( Super_Block->BPB_RootEntCnt * 32) \
	+ (Super_Block->BPB_BytsPerSec -1 )) / Super_Block->BPB_BytsPerSec;

	unsigned int tmp_val1 = dev->num_sectors - (root_dir_sectors + Super_Block->BPB_RsvdSecCnt);

	unsigned int tmp_val2 = (256 * Super_Block->BPB_SecPerClus) + Super_Block->BPB_NumFATs;

	if(FatType == FAT32) tmp_val2 = tmp_val2 / 2;

	unsigned int FATSz = (tmp_val1 + (tmp_val2 - 1)) / tmp_val2;


	char *VolLab;
	char *FilSysType;

	if(FatType == FAT32) {

		Super_Block->BPB_FATSz16 = 0;
		Super_Block->specific.fat32.BPB_FATSz32 = FATSz;
		Super_Block->specific.fat32.BPB_RootClus = 2;
		Super_Block->specific.fat32.BPB_FSInfo = Super_Block->BPB_HiddSec + 1;

		Super_Block->specific.fat32.BS_BootSig = 0x29;
		Super_Block->specific.fat32.BS_VolID = 0x20bccd50;

		memcpy(Super_Block->specific.fat32.BS_VolLab,VOLLAB,11);
		memcpy(Super_Block->specific.fat32.BS_FilSysType,FILESYSTEM_FAT32,8);

		VolLab = (char*) Super_Block->specific.fat32.BS_VolLab;
		FilSysType = (char*) Super_Block->specific.fat32.BS_FilSysType;

	} else {

		Super_Block->BPB_FATSz16 = FATSz &0xffff;
		// there is no BPB_FATSz32 in a FAT16 BPB

		Super_Block->specific.fat.BS_BootSig = 0x29;
		Super_Block->specific.fat.BS_VolID = 0x20bccd50;

		memcpy(Super_Block->specific.fat.BS_VolLab,VOLLAB,11);
		memcpy(Super_Block->specific.fat.BS_FilSysType,FILESYSTEM_FAT16,8);

		VolLab = (char*) Super_Block->specific.fat.BS_VolLab;
		FilSysType = (char*) Super_Block->specific.fat.BS_FilSysType;

	}
	
	if ( fwrite(Super_Block,1,512,fp) != 512 ) {

		fputs("error: write bpb \n",stderr);
		free(Super_Block);
		fclose(fp);
		return (-1);
		
	}


	// feixar o arquivo
	fclose(fp);

	// limpar a fat
	unsigned char *zero = (unsigned char *) malloc(0x1000);
	memset(zero,0,0x1000);


	unsigned long ThisFATSecNum = (Super_Block->BPB_HiddSec+Super_Block->BPB_RsvdSecCnt);

	for(i=0;i < (FATSz*Super_Block->BPB_NumFATs);i++) {

		if(block_write(dev->devnum,1,ThisFATSecNum + i,zero)) {
			fputs("error: fat nao formatada \n",stderr);
			free(zero);
			free(Super_Block);
			return -1;
		}
	}


	if(FatType == FAT12) {

		// FAT[0] and FAT[1]
		zero[0] = 0xf8;
		zero[1] = 0x8f;
		zero[2] = 0xff;

	} else if (FatType == FAT16) {

		// FAT[0]
		*(unsigned short*)(zero) = 0xfff8;
		// FAT[1]
		*(unsigned short*)(zero + 2) = 0xfff8;

	} else if (FatType == FAT32) {

		// FAT[0]
		*(unsigned int*)(zero) = 0x0ffffff8;
		// FAT[1]
		*(unsigned int*)(zero + 4) = 0x0ffffff8;

		*(unsigned int*)(zero + (4*Super_Block->specific.fat32.BPB_RootClus)) = 0xffffffff;
			


	} else {

		fputs("error: fat nao reservada \n",stderr);
		free(zero);
		free(Super_Block);
		return -1;

	}


	// configurar as fats reservadas

	if(block_write(dev->devnum,1,ThisFATSecNum,zero)) {
			fputs("error: fat nao reservada \n",stderr);
			free(zero);
			free(Super_Block);
			return -1;
	}


	
	unsigned int first_data_sector;
	unsigned int first_root_sector;
	memset(zero,0,0x1000);


	// limpar o root

	if(FatType == FAT16 || FatType == FAT12 ) {
	

		// Calcular o 1º sector de dados, FirstDataSector FAT12/16
		first_data_sector = Super_Block->BPB_RsvdSecCnt +\
		(Super_Block->BPB_FATSz16 * Super_Block->BPB_NumFATs) + root_sectors + Super_Block->BPB_HiddSec;

	
		// Calcular o 1º sector do Root Directory, FirstRootDirSector FAT12/16
		first_root_sector = (first_data_sector - root_sectors);

		for(i=0;i < root_sectors;i++) {

			if(block_write(dev->devnum,1,first_root_sector + i,zero)) {
				fputs("error: raiz nao formatado \n",stderr);
				free(zero);
				free(Super_Block);
				return -1;
			}

		}


	}else if (FatType == FAT32){

		// limpar o first cluster

		first_data_sector = Super_Block->BPB_RsvdSecCnt +\
		(Super_Block->specific.fat32.BPB_FATSz32 * Super_Block->BPB_NumFATs)\
		+ root_sectors + Super_Block->BPB_HiddSec;

		// FirstSectorofCluster = ((N – 2) * BPB_SecPerClus) + FirstDataSector;
		unsigned int first_sector_of_cluster = ((Super_Block->specific.fat32.BPB_RootClus - 2)\
		* Super_Block->BPB_SecPerClus) + first_data_sector;

		for(i=0;i < Super_Block->BPB_SecPerClus;i++) {

			if(block_write(dev->devnum,1,first_sector_of_cluster + i,zero)) {
				fputs("error: raiz nao formatado \n",stderr);
				free(zero);
				free(Super_Block);
				return -1;
			}

		}


	}else {

		fputs("error: o sistema de arquivos, foi corrompido \n",stderr);
		free(zero);
		free(Super_Block);
		return -1;

	}

	char str_[12];
	memcpy(str_,VolLab ,11); str_[11] = '\0';
	
	printf("Successfull:\nFileSystem Type: %s\nVolume Label: %s",FilSysType,str_);

	free(zero);
	free(Super_Block);
	
	return (0);

}
