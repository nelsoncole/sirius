/*
 * File Name: ata.c
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

ATA ata[4];
PCI_COFIG_SPACE *ata_pci = NULL;
UINTN ata_record_dev,ata_record_channel;

VOID ata_wait(UINTN p)
{
	if(p > 3) return;

	inportb(ata[p].cmd_block_base_addr + ATA_REG_STATUS);
	inportb(ata[p].cmd_block_base_addr + ATA_REG_STATUS);
	inportb(ata[p].cmd_block_base_addr + ATA_REG_STATUS);
	inportb(ata[p].cmd_block_base_addr + ATA_REG_STATUS);

	/*int i = 4;

	while(i--) __asm__ __volatile__("out %%al,$0x80"::);*/
}


UINTN ata_status_read(UINTN p)
{

   	return (inportb(ata[p].cmd_block_base_addr + ATA_REG_STATUS) &0xff);
    

}


UINTN ata_wait_not_busy(UINTN p)
{
    	while(ata_status_read(p) &ATA_SR_BSY)
    	if(ata_status_read(p) &ATA_SR_ERR)
    	return 1;

    	return 0;

}

UINTN ata_wait_busy(UINTN p)
{
    	while(!(ata_status_read(p) &ATA_SR_BSY))
    	if(ata_status_read(p) &ATA_SR_ERR)
    	return 1;

    	return 0;

}


UINTN ata_wait_no_drq(UINTN p)
{
    	while(ata_status_read(p) &ATA_SR_DRQ)
    	if(ata_status_read(p) &ATA_SR_ERR)
    	return 1;

    	return 0;
}

UINTN ata_wait_drq(UINTN p)
{
    
    	while(!(ata_status_read(p) &ATA_SR_DRQ))
    	if(ata_status_read(p) &ATA_SR_ERR)
    	return 1;

    	return 0;
}

VOID ata_cmd_write(UINTN p,UINTN cmd)
{
           
    	// no_busy      	
	ata_wait_not_busy(p);
	outportb(ata[p].cmd_block_base_addr + ATA_REG_CMD,cmd);
	ata_wait(p);  // Esperamos 400ns

}

VOID ata_soft_reset(UINTN p)
{
    	UINT8 data =  inportb(ata[p].ctrl_block_base_addr);
    	outportb(ata[p].ctrl_block_base_addr, data | 0x4);
    	outportb(ata[p].ctrl_block_base_addr, data &0xfb);    
}

VOID ata_pio_read(UINTN p,VOID *buffer,UINTN size)
{
	__asm__ __volatile__("\
	cld;\
        rep; insw"::"D"(buffer),
       	"d"(ata[p].cmd_block_base_addr + ATA_REG_DATA),"c"(size/2));


}


VOID ata_pio_write(UINTN p,VOID *buffer,UINTN size)
{
	__asm__ __volatile__("\
	cld;\
      	rep; outsw"::"S"(buffer),
	"d"(ata[p].cmd_block_base_addr + ATA_REG_DATA),"c"(size/2));
}


static UINTN ata_pci_configuration_space(UINTN bus,UINTN dev,UINTN fun)
{
    	UINT32 data;
  
    	// Indentification Device
    	data  = read_pci_config_addr(bus,dev,fun,0);
    
    	ata_pci->vid = data &0xffff;
    	ata_pci->did = data >> 16 &0xffff;
    
    	// Classe code, programming interface, revision id
    	data  = read_pci_config_addr(bus,dev,fun,8);
    
    	ata_pci->clss      	= data  >> 24 &0xff;
    	ata_pci->subclss   	= data  >> 16 &0xff;
    	ata_pci->rid 		= data &0xff;



	data  = read_pci_config_addr(bus,dev,fun,8);
        if(data &0x8000) {    
        // Bus Master Enable
        data  = read_pci_config_addr(bus,dev,fun,4);
        write_pci_config_addr(bus,dev,fun,4,data | 0x4);

        } 


	    // Habilitar interrupcao (INTx#)
        data  = read_pci_config_addr(bus,dev,fun,4);
        write_pci_config_addr(bus,dev,fun,4,data & ~0x400);


	data  = read_pci_config_addr(bus,dev,fun,8);
        if(data &0x8000) {    
        	// Bus Master Enable
        	data  = read_pci_config_addr(bus,dev,fun,4);
        	write_pci_config_addr(bus,dev,fun,4,data | 0x4);

        } 


	if(ata_pci->clss == 1 && ata_pci->subclss == 6){ 
		// AHCI
        	// Compatibilidade e nativo, primary
        	data  = read_pci_config_addr(bus,dev,fun,8);
        	if(data &0x200) write_pci_config_addr(bus,dev,fun,8,data &~ 0x100);
        	else {

			// MAP.MV
			data = read_pci_config_addr(bus,dev,fun,0x90);	
			write_pci_config_addr(bus,dev,fun,0x90,data &~ 0xC3);

			data  = read_pci_config_addr(bus,dev,fun,8);
			write_pci_config_addr(bus,dev,fun,8,data  | 0x100);


		}         

        	// Compatibilidade e nativo, secundary
        	data  = read_pci_config_addr(bus,dev,fun,8);
        	if(data &0x800) write_pci_config_addr(bus,dev,fun,8,data &~ 0x400);        
        	else { 

			// MAP.MV
			data = read_pci_config_addr(bus,dev,fun,0x90);	
			write_pci_config_addr(bus,dev,fun,0x90,data &~ 0xC3);


			data  = read_pci_config_addr(bus,dev,fun,8);
			write_pci_config_addr(bus,dev,fun,8,data | 0x400);        

		}
	}

	if(ata_pci->clss == 1 && ata_pci->subclss == 1){ 
		// IDE
       		// IDE Decode Enable
       		data  = read_pci_config_addr(bus,dev,fun,0x40);
       		write_pci_config_addr(bus,dev,fun,0x40,data | 0x80008000);



		//Synchronous DMA Control Register
		// Enable UDMA
		data = read_pci_config_addr(bus,dev,fun,0x48);	
		write_pci_config_addr(bus,dev,fun,0x48,data | 0xf);


	}


	// Interrupt
    	data  = read_pci_config_addr(bus,dev,fun,0x3C);
    	ata_pci->interrupt_line  = data &0xff;
    	ata_pci->interrupt_pin   = data >> 8 &0xff;



	
    	ata_pci->bar0    =read_pci_config_addr(bus,dev,fun,0x10);
    	ata_pci->bar1    =read_pci_config_addr(bus,dev,fun,0x14);
    	ata_pci->bar2    =read_pci_config_addr(bus,dev,fun,0x18);
    	ata_pci->bar3    =read_pci_config_addr(bus,dev,fun,0x1C);
    	ata_pci->bar4    =read_pci_config_addr(bus,dev,fun,0x20);
    	ata_pci->bar5    =read_pci_config_addr(bus,dev,fun,0x24);

	
    	return (0);

}
static VOID ata_bus_install(UINTN p,UINTN irqn,UINTN dev_num,UINTN channel,\
UINTN cmd_block_base_addr,UINTN ctrl_block_base_addr,UINTN bus_master_base_addr)
{

    	ata[p].irqn = irqn;
    	ata[p].dev_num = dev_num;
    	ata[p].channel = channel;
    	ata[p].cmd_block_base_addr = cmd_block_base_addr;
    	ata[p].ctrl_block_base_addr = ctrl_block_base_addr;
    	ata[p].bus_master_base_addr = bus_master_base_addr;


}

static UINTN detect_devtype (UINTN p)
{
	UINTN spin,st;
	//ata_soft_reset(ctrl->dev_ctl);
	ata_soft_reset(p);

	ata_wait_not_busy(p);

	outportb(ata[p].cmd_block_base_addr + ATA_REG_SECCOUNT,0);	// Sector Count 7:0
	outportb(ata[p].cmd_block_base_addr + ATA_REG_LBA0,0);        	// LBA 7-0   
	outportb(ata[p].cmd_block_base_addr + ATA_REG_LBA1,0);        	// LBA 15-8
	outportb(ata[p].cmd_block_base_addr + ATA_REG_LBA2,0);        	// LBA 23-16

    
    	// Select device,
    	outportb(ata[p].cmd_block_base_addr + ATA_REG_DEVSEL,0xA0| ata[p].dev_num << 4);
	ata_wait(p);

	ata_wait_not_busy(p);

	ata_cmd_write(p,ATA_CMD_IDENTIFY_DEVICE);
	ata_wait(p);
	

	spin = 1000000;
	while(spin--) { 
		// Ignora bit de erro
		if(!(ata_status_read(p)&ATA_SR_BSY))break;
	}



	// ponto flutuante
	st = ata_status_read(p);
	if(!st) {

		return ATADEV_UNKNOWN;
	}
	

	UINTN lba1 = inportb(ata[p].cmd_block_base_addr + ATA_REG_LBA1);
	UINTN lba2 = inportb(ata[p].cmd_block_base_addr + ATA_REG_LBA2);

 
	/* differentiate ATA, ATAPI, SATA and SATAPI */
	if (lba1==0x14 && lba2==0xEB) return ATADEV_PATAPI;
	if (lba1==0x69 && lba2==0x96) return ATADEV_SATAPI;
	if (lba1==0x00 && lba2==0x00) return ATADEV_PATA;
	if (lba1==0x3c && lba2==0xc3) return ATADEV_SATA;

	return ATADEV_UNKNOWN;
}

static UINTN ata_identify_device(UINTN p,UINT16 *buffer)
{


	switch(detect_devtype(p)) {

		case ATADEV_PATA:
		print("Uinidade%d PATA\n",p);
        	ata_pio_read(p,buffer,512);
        	ata_wait_not_busy(p);
        	ata_wait_no_drq(p);


		// config ata[n]
		ata[p].dev_type	= (buffer[0]&0x8000)? 0xffff:ATADEV_PATA;
        	ata[p].lba_type	= (buffer[83]&0x0400)? ATA_LBA48:ATA_LBA28;
        	ata[p].mode 	= ATA_PIO_MODO;//FIXME(buffer[49]&0x0100)? ATA_DMA_MODO:ATA_PIO_MODO;
		ata[p].bps 	= 512; 

			break;

		case ATADEV_PATAPI:
		print("Uinidade%d PATAPI\n",p);
		ata_cmd_write(p,ATA_CMD_IDENTIFY_PACKET_DEVICE);
        	ata_wait(p);
        	ata_wait_drq(p); 
        	ata_pio_read(p,buffer,512);
        	ata_wait_not_busy(p);
        	ata_wait_no_drq(p);


		// config ata[n]
		ata[p].dev_type	=(buffer[0]&0x8000)? ATADEV_PATAPI : 0xffff;
        	ata[p].lba_type	= ATA_LBA28;
        	ata[p].mode 	= ATA_PIO_MODO;//FIXME(buffer[49]&0x0100)? ATA_DMA_MODO:ATA_PIO_MODO;
        	ata[p].bps 	= 2048; 

			break;

		case ATADEV_SATA:

			print("Uinidade%d SATA\n",p);
			for(;;);

			break;

		case ATADEV_SATAPI:

			print("Uinidade%d SATAPI\n",p);
			for(;;);

			break;
		case ATADEV_UNKNOWN:
		ata[p].dev_type	= ATADEV_UNKNOWN;
		print("Uinidade%d Not found\n",p);

			break;

	}

	return 0;   
}


static VOID set_ata_device_and_sector(UINTN p,UINTN count,UINT64 addr)
{


    	switch(ata[p].lba_type)
    	{
    		case 28:
        	//Mode LBA28
		outportb(ata[p].cmd_block_base_addr + ATA_REG_SECCOUNT,(UINT8)count);	// Sector Count 7:0
	    	outportb(ata[p].cmd_block_base_addr + ATA_REG_LBA0,(UINT8)(addr &0xff));	// LBA 7-0   
	    	outportb(ata[p].cmd_block_base_addr + ATA_REG_LBA1,(UINT8)(addr >> 8 &0xff)); 	 // LBA 15-8
	    	outportb(ata[p].cmd_block_base_addr + ATA_REG_LBA2,(UINT8)(addr >> 16 &0xff));	 // LBA 23-16
         	// Modo LBA active, Select device, and LBA 27-24
        	outportb(ata[p].cmd_block_base_addr + ATA_REG_DEVSEL,0x40 |(ata[p].dev_num<<4) | ((UINT8)(addr>>24 &0x0f)));
        	// verifique se e a mesma unidade para nao esperar pelos 400ns
        	if(ata_record_dev != ata[p].dev_num && ata_record_channel != ata[p].channel) {
            	ata_wait(p);
            	//verifique erro
            	ata_record_dev      = ata[p].dev_num;
            	ata_record_channel  = ata[p].channel;}
        		break;
    		case 48:
        	//Mode LBA48
        	outportb(ata[p].cmd_block_base_addr + ATA_REG_SECCOUNT,0);		// Sector Count 15:8
	    	outportb(ata[p].cmd_block_base_addr + ATA_REG_LBA0,(UINT8)(addr >> 24 &0xff)); 	// LBA 31-24   
	    	outportb(ata[p].cmd_block_base_addr + ATA_REG_LBA1,(UINT8)(addr >> 32 &0xff));	        // LBA 39-32
	    	outportb(ata[p].cmd_block_base_addr + ATA_REG_LBA2,(UINT8)(addr >> 40 &0xff));	        // LBA 47-40
	    	outportb(ata[p].cmd_block_base_addr + ATA_REG_SECCOUNT,count);	        // Sector Count 7:0
	    	outportb(ata[p].cmd_block_base_addr + ATA_REG_LBA0,(UINT8)(addr &0xff));		// LBA 7-0   
	    	outportb(ata[p].cmd_block_base_addr + ATA_REG_LBA1,(UINT8)(addr >> 8 &0xff));          // LBA 15-8
	    	outportb(ata[p].cmd_block_base_addr + ATA_REG_LBA2,(UINT8)(addr >> 16 &0xff));	        // LBA 23-16
		// Modo LBA active, Select device,        
        	outportb(ata[p].cmd_block_base_addr + ATA_REG_DEVSEL,0x40 | ata[p].dev_num<<4);   
        	// verifique se e a mesma unidade para nao esperar pelos 400ns
        	if(ata_record_dev != ata[p].dev_num && ata_record_channel != ata[p].channel) {
            	ata_wait(p);
            	//verifique erro
            	ata_record_dev      = ata[p].dev_num;
            	ata_record_channel  = ata[p].channel;}
        		break;
    		case 0:
        	print("PANIC,ATA Modo CHS not suport Unidade%d\n",p);
		for(;;);
        		break;

       }
       
}

UINTN ata_read_sector(	IN UINTN p,
			IN UINTN count,
			IN UINT64 addr,
			OUT VOID *buffer)
{ 





	switch(ata[p].dev_type) {

		case ATADEV_UNKNOWN:
		return -1;
			break;

		case ATADEV_PATA:

		//select device, lba, count
        	set_ata_device_and_sector(p,count,addr);
		
     		switch(ata[p].mode) 
		{

			case ATA_PIO_MODO:
			switch(ata[p].lba_type) 
			{
				case ATA_LBA28:
				ata_cmd_write(p,ATA_CMD_READ_SECTORS);
					break;
				case ATA_LBA48:
				ata_cmd_write(p,ATA_CMD_READ_SECTORS_EXT);
					break;

			}
			ata_wait_not_busy(p);
            		if(ata_wait_drq(p) != 0)return -1;
            		ata_pio_read(p,buffer,ata[p].bps);
            		ata_wait_not_busy(p);
            		if(ata_wait_no_drq(p) != 0)return -1;

				break;

			case ATA_DMA_MODO:
			return -1;
				break;

		}
			break;

		case ATADEV_PATAPI:
		return -1;
			break;

	}
        	
        return 0;

}

UINTN ata_initialize()
{
	UINTN p;
	UINT16 *buffer = (UINT16*)alloc(1);
	ata_pci = (PCI_COFIG_SPACE *)alloc(1);
    	UINT32 data = pci_scan_bcc(PCI_CALSS_MASS_STORAGE);

    	if(data  == -1) {
    		print("ATA/ATAPI Controller not found!\n");
    		for(;;);
    	}


    	data = ata_pci_configuration_space(data  >>  8 &0xff,data  >> 3  &31,data &7);

    	if(data) {
        	print("PANIC: IDE/AHCI PCI Configuration Space\n");
		for(;;);       
    	}


    	// Initialize base address
    	// IDE legacy
    	ATA_BAR0 = (ata_pci->bar0 & ~7) + ATA_IDE_BAR0 * ( !ata_pci->bar0);
    	ATA_BAR1 = (ata_pci->bar1 & ~3) + ATA_IDE_BAR1 * ( !ata_pci->bar1);       
    	ATA_BAR2 = (ata_pci->bar2 & ~7) + ATA_IDE_BAR2 * ( !ata_pci->bar2);
    	ATA_BAR3 = (ata_pci->bar3 & ~3) + ATA_IDE_BAR3 * ( !ata_pci->bar3);
    	ATA_BAR4 = (ata_pci->bar4 & ~0x7) + ATA_IDE_BAR4 * ( !ata_pci->bar4);
    	ATA_BAR5 = (ata_pci->bar5 & ~0xf) + ATA_IDE_BAR5 * ( !ata_pci->bar5);


	// Install  bus, ports IDE
    	ata_bus_install(0,14/*IDE_IRQ14*/,0,ATA_PRIMARY,ATA_BAR0,ATA_BAR1,ATA_BAR4 + 0);
    	ata_bus_install(1,14/*IDE_IRQ14*/,1,ATA_PRIMARY,ATA_BAR0,ATA_BAR1,ATA_BAR4 + 0);
    	ata_bus_install(2,15/*IDE_IRQ15*/,0,ATA_SECONDARY,ATA_BAR2,ATA_BAR3,ATA_BAR4 + 8);
    	ata_bus_install(3,15/*IDE_IRQ15*/,1,ATA_SECONDARY,ATA_BAR2,ATA_BAR3,ATA_BAR4 + 8);


	
	for(p = 0; p < 4; p++)
	ata_identify_device(p,buffer);


	ata_record_dev = ata_record_channel = -1;

    	

    	return (0);
                                                                                                 
}

