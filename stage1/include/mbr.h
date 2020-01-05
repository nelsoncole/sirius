/*
 * File Name: mbr.h
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

#ifndef __MBR_H__
#define __MBR_H__


typedef struct _PARTITION_TABLE_ENTRY 
{
	// Drive attributes (bit 7 set = active or bootable)
	UINT8	dev_attr;

	// CHS Address of partition start 
	UINT8	chs_start[3];

	// Partition type 
	UINT8	part_type;
	// CHS address of last partition sector 
	UINT8	chs_last[3];

	// LBA of partition start  	
	UINT32 	lba_start;

	// Number of sectors in partition 
	UINT32	num_sectors; 
	
}__attribute__ ((packed)) PARTITION_TABLE_ENTRY;

typedef struct _MBR 
{
	// MBR Bootstrap (flat binary executable code) 
	CHAR8	bootstrap[440];
	
	// Optional "Unique Disk ID / Signature
	UINT32	unique_disk_id;
	
	//  Optional, reserved 0x0000
	UINT8	rsv[2];

	// First partition table entry 
	// Second partition table entry
	// Third partition table entry
	// Fourth partition table entry
	PARTITION_TABLE_ENTRY part[4];

	// (0x55, 0xAA) "Valid bootsector" signature bytes
	UINT16	bootsig;
	
}__attribute__ ((packed)) MBR;


typedef struct _VOLUME
{	// Device number
	UINT8	dev_num; 
	// Partition type 
	UINT8	part_type;
	// LBA of partition start  	
	UINT32 	lba_start;
	// Number of sectors in partition 
	UINT32	num_sectors;

}__attribute__ ((packed)) VOLUME;


#endif
