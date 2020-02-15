#ifndef __DISKBLOCK_H
#define __DISKBLOCK_H


int block_read(int media_id,int count,unsigned int offset /*LBA START*/,void *buffer);

int block_write(int media_id,int count,unsigned int offset /*LBA START*/,void *buffer);



#endif
