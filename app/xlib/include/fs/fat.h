/*
 * File Name: fat.h
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

#ifndef __FAT_H__
#define __FAT_H__

#define FAT12 12
#define FAT16 16
#define FAT32 32
#define ExFAT 0

#define FAT_LFN 0x80
#define FAT_SFN 0x00


#define FAT_ATTR_READ_ONLY	0x01
#define FAT_ATTR_HIDDEN		0x02
#define FAT_ATTR_SYSTEM		0x04
#define FAT_ATTR_VOLUME_ID	0x08
#define FAT_ATTR_DIRECTORY	0x10
#define FAT_ATTR_ARCHIVE	0x20
#define FAT_ATTR_LONG_NAME	(FAT_ATTR_READ_ONLY | FAT_ATTR_HIDDEN\
 				| FAT_ATTR_SYSTEM| FAT_ATTR_VOLUME_ID)

#define FAT_ATTR_LONG_NAME_MASK	(FAT_ATTR_READ_ONLY | FAT_ATTR_HIDDEN | FAT_ATTR_SYSTEM |\
				FAT_ATTR_VOLUME_ID | FAT_ATTR_DIRECTORY | FAT_ATTR_ARCHIVE)

#define FAT_DIR_ENTRY_SIZE	32

typedef struct _FAT_BPB {

	char		BS_jmpBoot[3];
	char		BS_OEMName[8];
	unsigned short	BPB_BytsPerSec;
	unsigned char	BPB_SecPerClus;
	unsigned short	BPB_RsvdSecCnt;
	unsigned char	BPB_NumFATs;
	unsigned short	BPB_RootEntCnt;
	unsigned short	BPB_TotSec16;
	unsigned char	BPB_Media;
	unsigned short	BPB_FATSz16;
	unsigned short	BPB_SecPertrk;
	unsigned short	BPB_NumHeads;
	unsigned int	BPB_HiddSec;
	unsigned int	BPB_TotSec32;
	
union {
 	struct 
	{// Para FAT12/FAT16

	unsigned char	BS_DrvNum;
    	unsigned char	BS_Reserved1;
    	unsigned char	BS_BootSig;
	unsigned int	BS_VolID;
 	char		BS_VolLab[11];
	char		BS_FilSysType[8];
		
	}__attribute__ ((packed)) fat;

     	struct 
       	{ // Para FAT32
	
	unsigned int	BPB_FATSz32;
	unsigned short	BPB_ExtFlags;
	unsigned short	BPB_FSVer;
	unsigned int	BPB_RootClus;
	unsigned short	BPB_FSInfo;
	unsigned short	BPB_BkBootSec;
	char		BPB_Reserved[12];
	unsigned char	BS_DrvNum;
	unsigned char	BS_Reserved1;
	unsigned char	BS_BootSig;
	unsigned int	BS_VolID;
	char		BS_VolLab[11];
	char		BS_FilSysType[8];

	        }__attribute__ ((packed)) fat32;
	}__attribute__ ((packed)) specific;
}__attribute__ ((packed)) FAT_BPB;

typedef struct _FAT_DIRECTORY {
	
	char		DIR_Name[8];
	char	 	DIR_Name_Ext[3];
	unsigned char 	DIR_Attr;
	unsigned char	DIR_NTRes;
	unsigned char	DIR_CrtTimeTenth;
	unsigned short	DIR_CrtTime;
	unsigned short	DIR_CrtDate;
	unsigned short	DIR_LstAccDate;
	unsigned short	DIR_FstClusHI;
	unsigned short	DIR_WrtTime;
	unsigned short	DIR_WrtDate;
	unsigned short	DIR_FstClusLO;
	unsigned int	DIR_FileSize;
	
}__attribute__ ((packed)) FAT_DIRECTORY;

FAT_BPB *FatReadBPB(VOLUME *volume);
int ReadFat(FAT_BPB *bpb,void *fat_table,int FATOffset);
FAT_DIRECTORY *FatOpenRoot(FAT_BPB *bpb);
int FatUpdateRoot(FAT_BPB *bpb,FAT_DIRECTORY *root);
int FatCreateFile(FAT_BPB *bpb,FAT_DIRECTORY *_dir,const char *filename,unsigned char attr);
int FatRemveFile(FAT_BPB *bpb,FAT_DIRECTORY *_dir,const char *filename);
int FatCheckFile(FAT_BPB *bpb,FAT_DIRECTORY *_dir,const char *filename);
int FatOpenFile(FAT_BPB *bpb,FAT_DIRECTORY *_dir,const char *filename,unsigned char attr,FILE *fd);
int FatUpdateFile(FAT_BPB *bpb,FAT_DIRECTORY *_dir,FILE *fd);

unsigned char ChkSum (unsigned char *pFcbName);


#endif
