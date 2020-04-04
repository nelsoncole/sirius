/*
 * File Name: dev.c
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

#include <sys/sys.h>
#include <stdlib.h>
#include <string.h>

#include <stdio.h>

static int checksum_sdx(int dev)
{

	SD *sdx = (SD*) (stdsd->header.buffer);

	int i;
	for(i=0;i<64;i++)
	{
		if((sdx->devnum == dev) && (sdx->partnum == 12345)) {
			return -1;
		}

		sdx = (SD*) (stdsd->header.buffer + (i*1024));
	}

	if(i >= 64) return 0;


	return -1;
}

SD* loader_sdx(int dev)
{

	SD *sdx = (SD*) (stdsd->header.buffer);

	int i;
	for(i=0;i<64;i++)
	{
		if((sdx->devnum == dev) && (sdx->partnum == 12345)) {
			return (sdx);
		}

		sdx = (SD*) (stdsd->header.buffer + (i*1024));
	}


	return NULL;
}

SD *request_sdx()
{

	unsigned int *bf = (unsigned int *) (stdsd->header.buffer);
	SD *sdx = NULL;

	int i;
	for(i=0;i<64;i++)
	{
		if(bf[3] == 0) {
			sdx = (SD*) bf;
			sdx->pid = i;
			break;
		}

		bf = (unsigned int *) (stdsd->header.buffer + (i*1024));
	}

	if(i >= 64) return  (SD*)0;


	return  (SD*)bf;
}

SD *request_sdn(unsigned int addr)
{

	unsigned int _addr = addr;

	unsigned int *bf = (unsigned int *) (_addr);

	int i;
	for(i=0;i<32-1;i++)
	{
		if(bf[6] == 0) break;

		bf = (unsigned int *) (_addr + (i*32));
	}

	if(i >= 32-1) return  (SD*)0;


	return  (SD*)bf;
}

int mount_sd(int dev)
{
	int i;
	int rc = -1;
	int pid = 0;
	int bps = 0;

	// LOADER MBR
	MBR *mbr = (MBR*)malloc(0x1000);

	if(block_read(dev,1,0,mbr) ) 
	{ 

		free(mbr);
		return -1;
	} 
	else {


		// cheksum
		if(checksum_sdx(dev))
		{

			free(mbr);
			return -1;

		}

		// conect device
		SD *sdx = request_sdx();

		if(!sdx) { 
	
			free(mbr);
			return -1;
		} 


		pid = 0x61 + sdx->pid - 1;

		sdx->id[0] = 's';
		sdx->id[1] = 'd';
		sdx->id[2] = pid;
		sdx->id[3] = '\0';

		sdx->devnum = dev;
		sdx->partnum = 12345;
		sdx->UID = mbr->unique_disk_id;
		sdx->lba_start = 0;
		sdx->lba_end = block_sectors(sdx->devnum);
		sdx->num_sectors = sdx->lba_end;
		sdx->byte_of_sector =  bps = block_bps(sdx->devnum);

		sdx++;

		// montar particao MBR
		for(i=0;i<4;i++) {

			sdx = request_sdn((unsigned int)sdx);
			if(!sdx) { 
				continue;
			} 
			
			sdx->id[0] = 's';
			sdx->id[1] = 'd';
			sdx->id[2] = pid;
			sdx->id[3] = 0x31 + i;

			sdx->devnum = dev;
			sdx->partnum	= i;

			if(!mbr->part[sdx->partnum].num_sectors) {
				sdx->num_sectors = 0;
				continue;
			} 


			sdx->UID = mbr->unique_disk_id;
			sdx->lba_start = mbr->part[sdx->partnum].lba_start;
			sdx->lba_end = mbr->part[sdx->partnum].lba_start + mbr->part[sdx->partnum].num_sectors;
			sdx->num_sectors = mbr->part[sdx->partnum].num_sectors;
			sdx->byte_of_sector = bps;

			rc = 0;
		}

	}

	free(mbr);

	return rc;
}

int update_mount_sd(int dev)
{
	int i;
	int rc = -1;
	int pid = 0;
	int bps = 0;

	SD *sdx = loader_sdx(dev);

	// cheksum
	if(!sdx)
	{
		// unidade nao existe
		return -1;

	}

	// LOADER MBR
	MBR *mbr = (MBR*)malloc(0x1000);

	if(block_read(dev,1,0,mbr) ) 
	{ 
		free(mbr);
		return -1;
	} 
	else {


		if(!sdx || sdx->partnum != 12345 ) { 
	
			free(mbr);
			return -1;
		} 


		pid = 0x61 + sdx->pid - 1;
		bps = sdx->byte_of_sector;
		sdx++;

		memset(sdx,0,31*32);



		// montar particao MBR
		for(i=0;i<4;i++) {

			sdx = request_sdn((unsigned int)sdx);

			if(!sdx) { 
				continue;
			} 
			
			sdx->id[0] = 's';
			sdx->id[1] = 'd';
			sdx->id[2] = pid;
			sdx->id[3] = 0x31 + i;

			sdx->devnum = dev;
			sdx->partnum	= i;

			if(!mbr->part[sdx->partnum].num_sectors) {
				sdx->num_sectors = 0;
				continue;
			} 


			sdx->UID = mbr->unique_disk_id;
			sdx->lba_start = mbr->part[sdx->partnum].lba_start;
			sdx->lba_end = mbr->part[sdx->partnum].lba_start + mbr->part[sdx->partnum].num_sectors;
			sdx->num_sectors = mbr->part[sdx->partnum].num_sectors;
			sdx->byte_of_sector = bps;

			rc = 0;
		}

	}

	free(mbr);

	return rc;
}


SD *read_sdx(const char *s)
{
	unsigned int *bf = (unsigned int*) (stdsd->header.buffer);
	SD *_sd = NULL;

	int i;
	for(i=0;i<64;i++)
	{
		if((bf[3] == 12345) && (bf[0] == *(unsigned int*)s) ) {

			_sd = (SD*)bf;
			break;

		}

		bf = (unsigned int *) (stdsd->header.buffer + (i*1024));
	}


	if(i >= 64) return _sd;

	return _sd;
}

SD *read_sdn(const char *s,SD *sdx)
{


	if(!sdx) return 0;

	unsigned int addr = (unsigned int) (sdx);
	addr +=32;

	unsigned int *bf = (unsigned int*) (addr);
	SD *_sd = NULL;

	int i;
	for(i=0;i<32-1;i++)
	{
		if((bf[6] != 0) && (bf[0] == *(unsigned int*)s) ) {

			_sd = (SD*)bf;
			break;

		}

		bf = (unsigned int *) (addr + (i*32));
	}


	if(i >= 32-1) return _sd;

	return _sd;
}


char *pathneme(char *dest,const char *src)
{
	char *p_dest = dest;
	const char *p_src = src;

	while(*p_src) 
	{	
		if(*p_src == '/') { 

			*p_dest = '\0';
			p_src++;

			return (char*)p_src;

		}

		*p_dest++ = *p_src++;
	}

	*p_dest = '\0';

	return (char*)p_src;
}
