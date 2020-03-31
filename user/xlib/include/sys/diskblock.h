#ifndef __DISKBLOCK_H
#define __DISKBLOCK_H


int block_read(int media_id,int count,unsigned int offset /*LBA START*/,void *buffer);

int block_write(int media_id,int count,unsigned int offset /*LBA START*/,void *buffer);

unsigned int block_sectors(int media_id);
unsigned int block_bps(int media_id);



#endif
