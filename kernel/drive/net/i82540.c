/*
 * File Name: i82540.c
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



// Memory Register Base Address
UINT32 nicbar0 = 0;
// Memory Flash Base Address
UINT32 nicbar1 = 0;
// IO Register Base Address
UINT32 nicbar2 = 0;

UINTN i82540_pci_configuration_space(UINTN bus,UINTN dev,UINTN fun);

/*
 *	RAL (05400h + 8*n; R/W)
 *
 *	RAH (05404h + 8*n; R/W)
 */
UINTN nic_install(){


	CHAR8 *p;

	UINT32 data = pci_scan_bcc(0x2);

	if(data == -1) {

		print("PCI PANIC: Network Controller not found!\n");
		return (-1);
	}


	i82540_pci_configuration_space(data  >>  8 &0xff,data  >> 3  &31,data &7);


	mem_map((PHYSICAL_ADDRESS)nicbar0,\
	(VIRTUAL_ADDRESS *)&p,0x1000*32/*128 KiB*/,0x13);


	print("REG_RAL ( 0x5400 ) 0x%x, REG_RAH ( 0x5404 ) 0x%x\n\n",(*(UINT32*)(p + 0x5400)),(*(UINT32*)(p + 0x5404)) );


	return 0;

}



UINTN i82540_pci_configuration_space(UINTN bus,UINTN dev,UINTN fun)
{

	UINT32 data;
    	// VENDOR and DEVICE ID offset 0
    	data  = read_pci_config_addr(bus,dev,fun,0);

	print("\n\nPCI Network Controller initialize\n");

	print("Vendor ID %X, Device ID %X\n",data &0xffff, data >> 16 &0xffff);

	nicbar0  = read_pci_config_addr(bus,dev,fun,0x10) & 0xFFFFFFF0;
	print("BAR0 %X\n",nicbar0);

	nicbar1  = read_pci_config_addr(bus,dev,fun,0x14);
	print("BAR1 %X\n",nicbar1);

	nicbar2  = read_pci_config_addr(bus,dev,fun,0x18) & 0xFFFFFFF0;

	print("BAR2 %X\n",nicbar2);


	return 0;
}
