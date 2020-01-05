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
#define FAT_ATTR_ARCHIVE		0x20
#define FAT_ATTR_LONG_NAME	(FAT_ATTR_READ_ONLY | FAT_ATTR_HIDDEN\
 				| FAT_ATTR_SYSTEM| FAT_ATTR_VOLUME_ID)

#define FAT_DIR_ENTRY_SIZE	32

typedef struct _FAT_BPB {

	CHAR8	BS_jmpBoot[3];
	CHAR8	BS_OEMName[8];
	UINT16	BPB_BytsPerSec;
	UINT8	BPB_SecPerClus;
	UINT16	BPB_RsvdSecCnt;
	UINT8	BPB_NumFATs;
	UINT16	BPB_RootEntCnt;
	UINT16	BPB_TotSec16;
	UINT8	BPB_Media;
	UINT16	BPB_FATSz16;
	UINT16	BPB_SecPertrk;
	UINT16	BPB_NumHeads;
	UINT32	BPB_HiddSec;
	UINT32	BPB_TotSec32;
	
union {
 	struct 
	{// Para FAT12/FAT16

	UINT8	BS_DrvNum;
    	UINT8	BS_Reserved1;
    	UINT8	BS_BootSig;
	UINT32	BS_VolID;
 	CHAR8	BS_VolLab[11];
	CHAR8	BS_FilSysType[8];
		
	}__attribute__ ((packed)) fat;

     	struct 
       	{ // Para FAT32
	
	UINT32	BPB_FATSz32;
	UINT16	BPB_ExtFlags;
	UINT16	BPB_FSVer;
	UINT32	BPB_RootClus;
	UINT16	BPB_FSInfo;
	UINT16	BPB_BkBootSec;
	CHAR8	BPB_Reserved[12];
	UINT8	BS_DrvNum;
	UINT8	BS_Reserved1;
	UINT8	BS_BootSig;
	UINT32	BS_VolID;
	CHAR8	BS_VolLab[11];
	CHAR8	BS_FilSysType[8];

	        }__attribute__ ((packed)) fat32;
	}__attribute__ ((packed)) specific;
}__attribute__ ((packed)) FAT_BPB;

typedef struct _FAT_DIRECTORY {
	
	CHAR8	DIR_Name[8];
	CHAR8 	DIR_Name_Ext[3];
	UINT8 	DIR_Attr;
	UINT8	DIR_NTRes;
	UINT8	DIR_CrtTimeTenth;
	UINT16	DIR_CrtTime;
	UINT16	DIR_CrtDate;
	UINT16	DIR_LstAccDate;
	UINT16	DIR_FstClusHI;
	UINT16	DIR_WrtTime;
	UINT16	DIR_WrtDate;
	UINT16	DIR_FstClusLO;
	UINT32	DIR_FileSize;
	
}__attribute__ ((packed)) FAT_DIRECTORY;



typedef struct _FAT_DATA {

    	UINT32	count_of_clusters;
    	UINT32	data_sectors;
    	UINT32	first_data_sector;
    	UINT32	first_fat_sector;
    	UINT32	fat_offset;
    	UINT32	first_root_sector;
    	UINT32	first_sector_of_cluster;
    	UINT32	fat_size;
    	UINT32	fat_type;
    	UINT32	root_cluster;
    	UINT32	root_sectors;
    	UINT32	volume_total_sectors;

    	UINT32	fat_total_sectors;
    	UINT32	fat_ent_offset;

}__attribute__ ((packed)) FAT_DATA;


FAT_BPB *FatReadBPB(VOLUME *volume);
FAT_DIRECTORY *FatOpenRoot(FAT_BPB *_bpb,FAT_DATA *data);
UINT32 ReadFat(FAT_BPB *_bpb,OUT VOID *fat_table,UINTN FATOffset);
UINTN FatOpenFile(FAT_BPB *_bpb,FAT_DATA *data,FAT_DIRECTORY *_dir,CONST CHAR8 *filename,UINT8 attr,VFS *_vfs);



#endif
