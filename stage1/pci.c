/*
 * File Name: pci.c
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

#include <io.h>

UINTN read_pci_config_addr(UINTN bus,UINTN dev,UINTN fun, UINTN offset)
{
    	outportl(PCI_PORT_ADDR,CONFIG_ADDR(bus,dev,fun, offset));
    	return inportl(PCI_PORT_DATA);
}

VOID write_pci_config_addr(UINTN bus,UINTN dev,UINTN fun,UINTN offset,UINTN data)
{
    	outportl(PCI_PORT_ADDR,CONFIG_ADDR(bus,dev,fun, offset));
    	outportl(PCI_PORT_DATA,data);
}

UINTN pci_read_config_dword(UINTN bus,UINTN dev,UINTN fun,UINTN offset)
{
    	outportl(PCI_PORT_ADDR,CONFIG_ADDR(bus,dev,fun, offset));
    	return inportl(PCI_PORT_DATA);
}

VOID pci_write_config_dword(UINTN bus,UINTN dev,UINTN fun,UINTN offset,UINTN data)
{
    	outportl(PCI_PORT_ADDR,CONFIG_ADDR(bus,dev,fun, offset));
    	outportl(PCI_PORT_DATA,data);
}

// Esta funçao deve retornar o numero de barramento, o dispositivo e a funçao
// do dispositivo conectado ao barramento PCI, de acordo a classe.
UINT32 pci_scan_bcc(UINTN bcc)
{
    UINT32 data = -1;

    UINTN bus, dev, fun;

    
    for(bus = 0;bus < MAX_BUS; bus++) {
        for(dev = 0; dev < MAX_DEV; dev++){
            for(fun = 0; fun < MAX_FUN; fun++){
                outportl(PCI_PORT_ADDR,CONFIG_ADDR(bus,dev,fun, 0x8));
                data =inportl(PCI_PORT_DATA);
                if((data >> 24 &0xff) == bcc){
                    return (fun + (dev * 8) + (bus * 32));
            
                }
            }
     
        }
     
    }

    return (-1);

}
UINT32 pci_scan_bcc_scc(UINTN bcc,UINTN scc)
{
    UINT32 data = -1;

    UINTN bus, dev, fun;

    
    for(bus = 0;bus < MAX_BUS; bus++){
        for(dev = 0; dev < MAX_DEV; dev++){
            for(fun = 0; fun < MAX_FUN; fun++){
                outportl(PCI_PORT_ADDR,CONFIG_ADDR(bus,dev,fun, 0x8));
                data =inportl(PCI_PORT_DATA);
                if(((data >> 24 &0xff) == bcc) && ((data >> 16 &0xff) == scc)){
                    return (fun + (dev *8) + (bus * 32));
            
                }
            }
     
        }
     
    }

    return (-1);

}


UINT32 pci_scan_vendor(UINT16 vendor)
{


    return 0;
}
UINT32 pci_check_vendor(UINTN bus,UINTN dev,UINTN fun,UINT16 vendor)
{



    return 0;
}
