/*
 * File Name: pci.h
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
#ifndef __PCI_H__
#define __PCI_H__

#define MAX_BUS 2  // 256
#define MAX_DEV 32 // 32
#define MAX_FUN 8  // 8
#define PCI_PORT_ADDR 0xCF8
#define PCI_PORT_DATA 0xCFC



#define CONFIG_ADDR(bus,device,fn,offset)\
                       (\
                       (((uint32_t)(bus) &0xff) << 16)|\
                       (((uint32_t)(device) &0x3f) << 11)|\
                       (((uint32_t)(fn) &0x07) << 8)|\
                       ((uint32_t)(offset) &0xfc)|0x80000000)

typedef struct _PCI_COFIG_SPACE{
    	UINT16	vid;
    	UINT16	did;
   	UINT16	cmd;
    	UINT16	sts;
    	UINT8	pif;
    	UINT8	rid;
    	UINT8	clss;
    	UINT8	subclss;
    	UINT8	primary_master_latency_timer;
    	UINT8	header_type;
    	UINT8	BIST;
    	UINT32	bar0;
    	UINT32	bar1; 
    	UINT32	bar2;
    	UINT32	bar3;
    	UINT32	bar4;
    	UINT32	bar5;
    	UINT16 	subsystem_vendor_id;
    	UINT16	subsystem_id;  
    	UINT32	capabilities_pointer;
    	UINT8	interrupt_line;
    	UINT8	interrupt_pin;

}__attribute__((packed)) PCI_COFIG_SPACE;


UINTN read_pci_config_addr(UINTN bus,UINTN dev,UINTN fun, UINTN offset);
VOID write_pci_config_addr(UINTN bus,UINTN dev,UINTN fun,UINTN offset,UINTN data);
UINTN pci_read_config_dword(UINTN bus,UINTN dev,UINTN fun,UINTN offset);
VOID pci_write_config_dword(UINTN bus,UINTN dev,UINTN fun,UINTN offset,UINTN data);

UINT32 pci_scan_bcc(UINTN bcc);
UINT32 pci_scan_bcc_scc(UINTN bcc,UINTN scc);

UINT32 pci_scan_vendor(UINT16 vendor);
UINT32 pci_check_vendor(UINTN bus,UINTN dev,UINTN fun,UINT16 vendor);









#endif


