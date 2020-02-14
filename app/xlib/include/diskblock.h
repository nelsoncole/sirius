#ifndef __DISKBLOCK_H__
#define __DISKBLOCK_H__

#define ATTR_ARCHIVE 	0
#define ATTR_DIRECTORY 	1


int block_read(int media_id,int count,unsigned int offset /*LBA START*/,void *buffer);

int block_write(int media_id,int count,unsigned int offset /*LBA START*/,void *buffer);


#include <ctype.h>
#include "fs/fat.h"



#endif
