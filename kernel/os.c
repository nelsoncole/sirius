/*
 * File Name: os.c
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
 

VOID wait_ns(UINTN count)
{
	count /=100;	 
	while(--count)__asm__ __volatile__("out %%al,$0x80"::);
}


//  MSR
UINTN 
getmsr(UINT32 msr, UINT32 *low, UINT32 *hight)
{
	__asm__ __volatile__("rdmsr":"=a"(*low),"=d"(*hight):"c"(msr));
	return 0;
}

UINTN 
setmsr(UINT32 msr, UINT32 low, UINT32 hight)
{
	__asm__ __volatile__("wrmsr": :"a"(low),"d"(hight),"c"(msr));
	return 0;
}


SD *request_sdx()
{

	unsigned int *bf = (unsigned int *) (sd->header.buffer);

	int i;
	for(i=0;i<64;i++)
	{
		if(bf[3] == 0) break;

		bf = (unsigned int *) (sd->header.buffer + (i*1024));
	}

	if(i >= 64) return  (SD*)0;


	return  (SD*)bf;
}

SD *request_sdn(unsigned int addr)
{

	unsigned int _addr = addr + 32;
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

int conect_sd(int dev)
{
	int i;
	int rc = -1;
	// LOADER MBR
	MBR *mbr = (MBR*)malloc(0x1000);

	if(read_sector(dev,1,0,mbr) ) { 
		free(mbr);
		return -1;
	} 
	else {

		// conect device
		SD *sdx = request_sdx();

		if(!sdx) { 
			free(mbr);
			return -1;
		} 

		sdx->id[0] = 's';
		sdx->id[1] = 'd';
		sdx->id[2] = 'a';
		sdx->id[3] = '\0';

		sdx->devnum = dev;
		sdx->partnum = 12345;
		sdx->UID = mbr->unique_disk_id;
		sdx->lba_start = 0;
		sdx->lba_end = ata_sectors(sdx->devnum);
		sdx->num_sectors = sdx->lba_end;
		sdx->byte_of_sector = ata_bps(sdx->devnum);
		sdx->pid	= 1;



		// montar particao
		for(i=0;i<4;i++) {

			sdx = request_sdn((unsigned int)sdx);
			if(!sdx) { 
				continue;
			} 


			sdx->devnum = dev;
			sdx->partnum	=  i; 

			if(!mbr->part[sdx->partnum].num_sectors) {

				sdx->num_sectors = 0;
				continue;
			}

			
			sdx->id[0] = 's';
			sdx->id[1] = 'd';
			sdx->id[2] = 'a';
			sdx->id[3] = 0x31 + i;

			sdx->UID = mbr->unique_disk_id;
			sdx->lba_start = mbr->part[sdx->partnum].lba_start;
			sdx->lba_end = mbr->part[sdx->partnum].lba_start + mbr->part[sdx->partnum].num_sectors;
			sdx->num_sectors = mbr->part[sdx->partnum].num_sectors;
			sdx->byte_of_sector = 512;

			rc = 0;
		}

	}

	free(mbr);

	return rc;
}

SD *read_sdx(const char *s)
{
	unsigned int *bf = (unsigned int*) (sd->header.buffer);
	SD *_sd = NULL;

	int i;
	for(i=0;i<64;i++)
	{
		if((bf[3] == 12345) && (bf[0] == *(unsigned int*)s) ) {

			_sd = (SD*)bf;
			break;

		}

		bf = (unsigned int *) (sd->header.buffer + (i*1024));
	}


	if(i >= 64) return _sd;

	return _sd;
}

SD *read_sdn(const char *s,SD *sdx)
{

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
