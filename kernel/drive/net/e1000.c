/*
 * File Name: i.c
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


#define ETH_TYPE_IP   0x0800  
#define ETH_TYPE_ARP  0x0806
 
#define ARP_OPC_REQUEST  0x01
#define ARP_OPC_REPLY    0x02


//little endian
#define ToNetByteOrder16(v) ((v >> 8) | (v << 8))
#define ToNetByteOrder32(v) (((v >> 24) & 0xFF) | ((v << 8) & 0xFF0000) | ((v >> 8) & 0xFF00) | ((v << 24) & 0xFF000000))
#define FromNetByteOrder16(v) ((v >> 8) | (v << 8))
#define FromNetByteOrder32(v) (((v >> 24) & 0xFF) | ((v << 8) & 0xFF0000) | ((v >> 8) & 0xFF00) | ((v << 24) & 0xFF000000))

/*
 * 	The address space within the Ethernet controller is divided up into eight main categories:
 * 	1 - PCI
 * 	2 - General Configuration and Wakeup
 * 	3 - Interrupt
 * 	4 - MAC Receive
 * 	5 - MAC Transmit
 * 	6 - PHY Receive, Transmit and Special Function
 * 	7 - Statistics
 * 	8 - Diagnostic State (not used in normal operation)
 *
 *	Internal registers and memories (including PHY) Memory 128 KB
 * 	Flash (optional) Memory 64 - 512 KB
 *	Expansion ROM (optional) Memory 64 - 512 KB
 *	Internal registers and memories, Flash (optional) I/O 8 Bytes
 *
 *	RAL (05400h + 8*n; R/W)
 *
 *	RAH (05404h + 8*n; R/W)
 */

//  Intel PRO/1000 MT Desktop (82540EM) (VID 0x8086, DID 0x100E)
//  Intel PRO/1000 T Server (82543GG) (VID 0x8086, DID 0x1004)
//  Intel PRO/1000 MT Intel (82545EM) (VID 0x8086, DID 0x100F)

#define REG_NIC_CTRL	0x00
#define REG_NIC_STATUS	0x08
#define REG_NIC_EEPROM	0x14

// Interrupts

// interrupt Cause Read
#define REG_NIC_ICR	0xC0
// Interrupt Throttling Rate 
#define REG_NIC_ITR	0xC4
// Interrupt Cause Set
#define REG_NIC_ICS	0xC8
// Interrupt Mask Set/Read
#define REG_NIC_IMS	0xD0
// Interrupt Mask Clear
#define REG_NIC_IMC	0xD8
// Interrupt Acknowledge Auto Mask
#define REG_NIC_IAM	0xE0


// Receive

// Receive Control
#define REG_NIC_RCTL	0x100
// Flow Control Receive Threshold Low
#define REG_NIC_FCRTL	0x2160
// Flow Control Receive Threshold High
#define REG_NIC_FCRTH	0x2168
// Packet Split Receive Control
#define REG_NIC_PSRCTL	0x2170

// Receive Queue 0 + 0x100 to Queue 1
// Receive Descriptor Base Low Queue 0
#define	REG_NIC_RDBAL0	0x2800
// Receive Descriptor Base High Queue 0
#define	REG_NIC_RDBAH0	0x2804
// Receive Descriptor Length Queue 0
#define	REG_NIC_RDLEN0	0x2808
// Receive Descriptor Head Queue 0
#define	REG_NIC_RDH0	0x2810
// Receive Descriptor Tail Queue 0
#define	REG_NIC_RDT0	0x2818
// Receive Interrupt Packet Delay Timer
#define	REG_NIC_RDTR	0x2820
// Receive Descriptor Control
#define	REG_NIC_RXDCTL	0x2828

// Receive Interrupt Absolute Delay Timer
#define	REG_NIC_RADV	0x282C
// Receive Small Packet Detect
#define	REG_NIC_RSRPD	0x2C00
// Receive ACK Interrupt Delay	
#define	REG_NIC_RAID	0x2c08
// CPU Vector
#define	REG_NIC_CPUVEC	0x2C10
// Receive Checksum Control
#define	REG_NIC_RXCSUM	0x5000
// Receive Filter Control
#define	REG_NIC_RFCTL	0x5008

// MAC
// Multicast Table Array (n)
#define REG_NIC_MTA	0x5200 // 05200h-053FCh
 
// Ethernet MAC address 48-bit.
// REL: 
//	31:0 - Contains the lower 32-bit of the 48-bit Ethernet address.
// RAH:
//	15:0 - Contains the upper 16 bits of the 48-bit Ethernet address.
//  	17:16 - Address Select (ASEL)
//		00b = Destination address (required for normal mode)
//		01b = Source address
//		10b = Reserved
//		11b = Reserved
//	30:18 - Reserved
//	31 - Address Valid (AV)

// Receive Address Low (n)
#define REG_NIC_RAL	0x5400 // 05400h-05478h
// Receive Address High (n)
#define REG_NIC_RAH	0x5404 // 05404h-0547Ch


// Transmit
// Transmit Control
#define REG_NIC_TCTL	0x400
// Transmit IPG
#define REG_NIC_TIPG	0x410
// Trasmit Queue 0 + 0x100 to Queue 1
// Transmit Descriptor Base Low Queue 0
#define	REG_NIC_TDBAL0	0x3800
// Transmit Descriptor Base High Queue 0
#define	REG_NIC_TDBAH0	0x3804
// Transmit Descriptor Length Queue 0
#define	REG_NIC_TDLEN0	0x3808
// Transmit Descriptor Head Queue 0
#define	REG_NIC_TDH0	0x3810
// Transmit Descriptor Tail Queue 0
#define	REG_NIC_TDT0	0x3818
// Transmit Interrupt Packet Delay Timer
#define	REG_NIC_TDTR	0x3820
// Transmit Descriptor Control
#define	REG_NIC_TXDCTL	0x3828
// Transmit Arbitration Counter Queue 0
#define	REG_NIC_TARC0	0x3840


// Transmit Absolute Interrupt Delay Value
#define	REG_NIC_TADV	0x382C


typedef struct _transmit {
	//uint64_t addr;
	//endereço físico do buffer.
	unsigned int 	phy_l;
	unsigned int 	phy_h;
	unsigned short 	length;
	unsigned char 	cso;    	/* checksum offset */
	unsigned char 	cmd;
	unsigned char 	status;   	/* status and reserved */
	unsigned char 	css;    	/* checksum start */
	unsigned short 	special;
}__attribute__((packed)) transmit_t;

typedef struct _receive {
	//uint64_t addr;
	//endereço físico do buffer.
	unsigned int 	phy_l;
	unsigned int 	phy_h;
	unsigned short 	length;
	unsigned short 	csum;       // Packet checksum
	unsigned char 	status;      // Descriptor status 
	unsigned char 	errors;      // Descriptor Errors 
	unsigned short 	special;
}__attribute__((packed)) receive_t;

typedef struct _descriptor {
	unsigned int 	base_l;
	unsigned int 	base_h;
	unsigned int 	length;
	unsigned int	head;
	unsigned int	tail;

	unsigned int	cur;
	unsigned int	virt;

}__attribute__((packed)) descriptor_t;

typedef struct _nic {

	unsigned int magic;	
	// PCI Local Bus Interface

	unsigned int 	fun;
	unsigned int	dev;
	unsigned int 	bus;
	unsigned short	vid;
	unsigned short	did;
	unsigned char	int_line;
	unsigned short	int_pin;
	// Memory Register Base Address
	unsigned int phy_address;
	unsigned int virt_address;
	// Memory Flash Base Address
	unsigned int flash_address;
	// IO Register Base Address
	unsigned int io_address;
	// MAC address
	unsigned char mac[6];
	unsigned char ip[4];
	// buffers
	descriptor_t transmit[2], receive[2];
	transmit_t *tx;
	receive_t  *rx;
	
}__attribute__((packed)) nic_t;
nic_t nic_base[1];


// Ethernet header length
#define ETHERNET_HEADER_LENGHT 14      

// ethernet header
typedef struct ether_header {

	uint8_t dst[6];
	uint8_t src[6];
	uint16_t type;
	
} __attribute__((packed)) ether_header_t;

// ARP header length
#define ARP_HEADER_LENGHT 28      

// arp struct
typedef struct  ether_arp {
	
    	uint16_t type;  	//Hardware type (HTYPE)
	uint16_t proto; 	//Protocol type (PTYPE)
	uint8_t hlen;   	//Hardware address length 
	uint8_t plen;   	//Protocol address length 
	uint16_t op;    	//Operation (OPER)

   	//#define ETH_ALEN 6 
	
	uint8_t arp_sha[6];    /* sender hardware address mac */
    	uint8_t arp_spa[4];    /* sender protocol address ip */
	
    	uint8_t arp_tha[6];    /* target hardware address mac*/
    	uint8_t arp_tpa[4];    /* target protocol address ip*/
	
} __attribute__((packed)) ether_arp_t; 

int e1000_pci_configuration_space(int bus,int dev,int fun)
{


	nic_base->fun = fun;
	nic_base->dev = dev;
	nic_base->bus = bus;


	unsigned int data;
    	// VENDOR and DEVICE ID offset 0
    	data  = read_pci_config_addr(bus,dev,fun,0);

	print("PCI Network Controller initialize\n");

	nic_base->vid = data &0xffff;
	nic_base->did = data >> 16 &0xffff;
	print("Vendor ID %X, Device ID %X\n",nic_base->vid, nic_base->did );

	nic_base->phy_address  = read_pci_config_addr(bus,dev,fun,0x10) & 0xFFFFFFF0;
	print("Memory Register Base Address 0x%x\n",nic_base->phy_address);

	nic_base->flash_address = read_pci_config_addr(bus,dev,fun,0x14);
	print("Memory Flash Base Address 0x%x\n",nic_base->flash_address);

	nic_base->io_address = read_pci_config_addr(bus,dev,fun,0x18) & 0xFFFFFFF0;

	print("IO Register Base Address 0x%x\n",nic_base->io_address);


	
	data  = read_pci_config_addr(bus,dev,fun,4);
        if( (data & 0x04) != 0x04 ) {    
        	// Bus Mastering Enable
        	write_pci_config_addr(bus,dev,fun,4,data | 0x4);

        }


	//data  = read_pci_config_addr(bus,dev,fun,0x3c);
	//write_pci_config_addr(bus,dev,fun,0x3c, data | 11);
	data  = read_pci_config_addr(bus,dev,fun,0x3c);

	nic_base->int_line = data & 0xff;
	nic_base->int_pin = data >> 8 &0xff;

	


	return 0;
}

unsigned e1000_irq = 0;
unsigned int e1000_read_cmd(nic_t *nic,unsigned int addr);
void e1000_write_cmd(nic_t *nic,unsigned int addr, unsigned int val);
int e1000_device_reset(nic_t *nic);

//	The receive control configuration registers 
//	such as RCTL (RTCL.UPE, RCTL.MPE,RCTL.BAM), MTA, RAL, and RAH.

int e1000_send_arp ( unsigned char source_ip[4], unsigned char target_ip[4], unsigned char target_mac[6] );
int e1000(){


	char *p;

	unsigned int data = pci_scan_class(0x2);// pci_scan_bcc(0x2);

	if(data == (-1) ) {

		print("error: Network Controller not found\n");
		nic_base->magic = 0;
		return (-1);
	}


	e1000_pci_configuration_space(data  >>  24 &0xff,data  >> 16  &0xff,data &0xff);

	if(nic_base->vid != 0x8086 ||\
	(nic_base->did !=0x100E && nic_base->did !=0x1004 && nic_base->did !=0x100F ) ) {

		print("error: Vendor ID 0x%x, Device ID 0x%x\n",nic_base->vid,nic_base->did);

		nic_base->magic = (-1);
		return (-1);
	}



	// intialize global
	e1000_irq = 0;

	nic_base->magic = 0x1234;

	print("nic->int_line %d, nic->int_pin %d\n",nic_base->int_line,nic_base->int_pin);



	//mem_map((PHYSICAL_ADDRESS)nic_base->phy_address,
	//(VIRTUAL_ADDRESS *)&p,32/*128 KiB*/,0x13);

	mm_mp( (unsigned int) nic_base->phy_address, (unsigned int *) &p,32/*128 KiB*/,0x17);

	nic_base->virt_address = (unsigned int) p;

	print("Memory Virtual Address 0x%x\n",nic_base->virt_address);

	nic_base->virt_address = (unsigned int) p;

	data = (*(volatile unsigned int *) (nic_base->virt_address + REG_NIC_RAL) );

	nic_base->mac[0] = data &0xff;
	nic_base->mac[1] = data >> 8 &0xff;
	nic_base->mac[2] = data >> 16 &0xff;
	nic_base->mac[3] = data >> 24 &0xff;
	
	data = (*(volatile unsigned int *) (nic_base->virt_address + REG_NIC_RAH) );

	nic_base->mac[4] = data &0xff;
	nic_base->mac[5] = data >> 8 &0xff;


	/*print("CONTROL=0x%x\n",e1000_read_cmd( nic_base,REG_NIC_CTRL));
	
	print("STATUS=0x%x\n",e1000_read_cmd( nic_base,REG_NIC_STATUS));
	
	print("EEPROM=0x%x\n",e1000_read_cmd( nic_base,REG_NIC_EEPROM));*/

	print("MAC: ");
	for(int i=0;i<6;i++) {
		print("%x",nic_base->mac[i]);
		if(i < 5)print(":");

	}
	print("\n");



	e1000_device_reset(nic_base);

	



	return 0;

}

void e1000_handler()
{

	struct ether_header *eh;
	struct ether_arp *arp_h;
	unsigned int val;

	e1000_irq++;

	//return;


	receive_t *rx = (receive_t *) nic_base->receive[0].base_l;
	transmit_t *tx = (transmit_t *) nic_base->transmit[0].base_l;

	e1000_write_cmd( nic_base, REG_NIC_IMS, 1);		
	unsigned int status = e1000_read_cmd ( nic_base, REG_NIC_ICR );

	print("REG_NIC_ICR 0x%x\n",status);

	// Linkup
	if (status & 0x04) 
	{															
		val = e1000_read_cmd( nic_base, REG_NIC_CTRL) | 0x40;
		e1000_write_cmd( nic_base, REG_NIC_CTRL, val);

		//e1000_write_cmd( nic_base, REG_NIC_RDT0, 31);

		return;
	
	} else if (status & 0x80){

		//while ( (currentNIC->legacy_rx_descs[currentNIC->rx_cur].status & 0x01) == 0x01 ) 
		//{
			//uint32_t len = nic_base->receive[0].length;
			//e1000_write_cmd( nic_base, REG_NIC_RDT0, 0 );
		//}

	}else ;



	unsigned int *x = (unsigned int *)rx;//->phy_l;

	print("IRQ count = %d\n============= Hexa Dump, buffer do descritor receive: status 0x%x\n",e1000_irq,rx->status);
	for(int i=0;i< 4*32;i++) {

		print("0x%x ",*x++);
	}

	print("\n\n============= Hexa Dump, buffer do descritor transmit: status 0x%x\n",tx->status);

	x = (unsigned int *)tx;
	for(int i=0;i< 4*8;i++) {

		print("0x%x ",*x++);
	}

	print("\n");


	//
	// =================== ## Reagir ## ================================
	//
	
	unsigned char *buffer = 0;
	uint16_t type;

	rx = (receive_t *) nic_base->receive[0].base_l;
	
	for(int i=0;i<32;i++) {

	buffer = (unsigned char *) (unsigned int *)rx->phy_l;
	//ethernet header
	eh = (struct ether_header *) &buffer[0];

	type = FromNetByteOrder16(eh->type);
	switch ( (uint16_t) type)
	{
		case 0x0806: // arp

			print("arp found:\nethernet type 0x%x bufer numero = %d rx->phy = 0x%x\n",type,i+1,rx->phy_l);
			print("dst = %x:%x:%x:%x:%x:%x\n",eh->dst[0],eh->dst[1],eh->dst[2],eh->dst[3],eh->dst[4],eh->dst[5]);
			print("src = %x:%x:%x:%x:%x:%x\n",eh->src[0],eh->src[1],eh->src[2],eh->src[3],eh->src[4],eh->src[5]);
			arp_h = (struct ether_arp *) &buffer[14];

			if( arp_h->op == ToNetByteOrder16(ARP_OPC_REPLY) ) {

				print("replay received\n");
				return;
				
			}else if ( arp_h->op == ToNetByteOrder16(ARP_OPC_REQUEST) ) {

				print("request received\n");
				return;

			}

			return;

		break;

		default:
			if(type)print("arp not found: ethernet type 0x%x bufer numero = %d\n",type,i+1);
			//return;
		break;


	}

		rx++;
	}

	//val = e1000_read_cmd( nic_base, REG_NIC_CTRL) | 0x40;
	//e1000_write_cmd( nic_base, REG_NIC_CTRL, val);

	

}

void e1000_irq_wait() {

	if(nic_base->magic != 0x1234) {
		return;
	}

	while(!e1000_irq);
}


unsigned int e1000_read_cmd(nic_t *nic,unsigned int addr) 
{	
	return (*(volatile unsigned int*) (nic->virt_address + addr));

}

void e1000_write_cmd(nic_t *nic,unsigned int addr, unsigned int val) 
{	
	(*(volatile unsigned int*) (nic->virt_address + addr)) = val;

}

int e1000_device_reset(nic_t *nic) {
	int i;
	unsigned int val;

	// Link Reset
	// PHY Reset
	//val = e1000_read_cmd ( nic, REG_NIC_CTRL);
	//e1000_write_cmd( nic, REG_NIC_CTRL, val | 0x80000004);
	

	// This register contains the lower bits of the 64-bit transmit Descriptor base address. The base
	// register indicates the start of the circular transmit descriptor queue. Since each descriptor is 16 bits
	// in length, the lower four bits are ignored as the Transmit Descriptor Base Address must point to a
	// 16-byte aligned block of data.

	nic->transmit[0].base_l = (unsigned int) malloc_virtual_to_physical(0x40000,&nic->transmit[0].virt); // 256KiB
	memset((void*)nic->transmit[0].virt,0,0x10000);
	nic->tx = (transmit_t *) (nic->transmit[0].virt);

	// This register contains the upper 32 bits of the 64-bit transmit Descriptor base address.
	nic->transmit[0].base_h = 0;

	// This value must be a multiple of 128 bytes (the maximum cache line size).
	// Since each descriptor is 16 bits in length, 128*8 = 1024
	nic->transmit[0].length = 128;
	
	// This register contains the head pointer for the transmit descriptor ring
	// It points to a 16-byte datum
	nic->transmit[0].head	 = 0;

	// This register contains the tail pointer for the transmit descriptor ring.
	// It points to a 16-byte datum
	nic->transmit[0].tail = 0;
		
	nic->transmit[0].cur = 0;

	
	for(i=0;i <8;i++) {
		nic->tx[i].phy_l = nic->transmit[0].base_l + 0x1000 + (0x1000*i);
		nic->tx[i].phy_h = 0;
		

		nic->tx[i].cmd = 0;
		nic->tx[i].status = 0;
	}


	// Receive

	// This register contains the lower bits of the 64-bit descriptor base address. The four low-order
	// register bits are always ignored. The Receive Descriptor Base Address must point to a 16-byte
	// aligned block of data.
	nic->receive[0].base_l = (unsigned int) malloc_virtual_to_physical(0x40000,&nic->receive[0].virt); // 256 KiB
	memset((void*)nic->receive[0].virt,0,0x21000);
	nic->rx = (receive_t *) (nic->receive[0].virt);
	

	// This register contains the upper 32 bits of the 64-bit Descriptor Base Address.
	nic->receive[0].base_h = 0;

	// This value must be a multiple of 128 bytes (the maximum cache line size).
	// Since each descriptor is 16 bits in length, 128*4 = 512
	nic->receive[0].length = 512;

	// This register contains the head pointer for the receive descriptor buffer.
	nic->receive[0].head	 = 0;

	nic->receive[0].tail = 31;


	nic->receive[0].cur = 0;

	for(i=0;i <32;i++) {
		nic->rx[i].phy_l = nic->receive[0].base_l + 0x1000 + (0x1000*i);
		nic->rx[i].phy_h = 0;

		nic->rx[i].status = 0;
	}

	
	// MTA(n) = 0
	for (i=0; i < 128; i++) 
		e1000_write_cmd( nic, REG_NIC_MTA + (i * 4), 0 );


	// bits:
	//	0 - TXDW Sets the mask for the Transmit Descriptor Written Back.
	//	1 - TXQE Sets the mask for the Transmit Queue Empty.
	//	2 - LSC Sets the mask for the Link Status Change.
	//	3 - RXSEQ Sets the mask for the Receive Sequence Error.
	// 	7 - RXT0 Sets the mask for the Receiver Timer Interrupt.
	//	14:13 - GPI Sets the mask for the General Purpose Interrupts.

	e1000_write_cmd( nic, REG_NIC_IMS, 0x1F6DE/*0x1F6DC*/ );
	e1000_read_cmd( nic, REG_NIC_ICR);

	// receive

	e1000_write_cmd( nic, REG_NIC_RDBAL0, nic->receive[0].base_l);
	e1000_write_cmd( nic, REG_NIC_RDBAH0, nic->receive[0].base_h);
	e1000_write_cmd( nic, REG_NIC_RDLEN0, nic->receive[0].length);
	e1000_write_cmd( nic, REG_NIC_RDH0, nic->receive[0].head);
	e1000_write_cmd( nic, REG_NIC_RDT0, nic->receive[0].tail);

	// Receive Control
	// bits:
	//	0 - Reserved
	//	1 - Receiver Enable
	//	2 - Store Bad Packets
	//	3 - Unicast Promiscuous Enabled
	//	4 - Multicast Promiscuous Enabled
	//	5 - Long Packet Reception Enable
	//	7:6 - Loopback mode. 
	//		00b = Normal operation.
	//		01b = MAC loopback
	//		10b = Undefined
	// 		11b = Reserved
	//	9:8 - Receive Descriptor Minimum Threshold Size 
	//		00b = Free buffer threshold is set to 1/2 of RDLEN 
	//		01b = Free buffer threshold is set to 1/4 of RDLEN.
	//		10b = Free buffer threshold is set to 1/8 of RDLEN.
	//		11b = Reserved.
	//	11:10 - Descriptor Type
	//		00b = Legacy description type.
	//		01b = Packet split description type.
	//		10b = Reserved.
	//		11b = Reserved.
	//	13:12 - Multicast Offset
	//		00b = bits [47:36] of received destination multicast address.
	//		01b = bits [46:35] of received destination multicast address.
	//		10b = bits [45:34] of received destination multicast address.
	//		11b = bits [43:32] of received destination multicast address.
	//	14 - Reserved
	//	15 - Broadcast Accept Mode. (0b = Ignore broadcast, 1b = Accept broadcast packets.)
	//	17:16 - Receive Buffer Size
	//		RCTL.BSEX = 0b:
	//			00b = 2048 Bytes.
	//			01b = 1024 Bytes.
	//			10b = 512 Bytes.
	//			11b = 256 Bytes.
	//		RCTL.BSEX = 1b:
	//			00b = Reserved; software should not program this value.
	//			01b = 16384 Bytes.
	//			10b = 8192 Bytes.
	//			11b = 4096 Bytes.
	//	18 - VLAN Filter Enable
	//	19 - Canonical Form Indicator Enable
	//	20 - Canonical Form Indicator bit value
	//	21 - Reserved
	//	22 - Discard Pause Frames (DPF controls the DMA function of flow control PAUSE packets
	//					addressed to the station address (RAH/L[0]).)
	//	23 - Pass MAC Control Frames
	//	24 - Reserved
	//	25 - Buffer Size Extension (RCTL.BSEX)
	//	26 - Strip Ethernet CRC from incoming packet
	//	30:27 - (	Determines a flexible buffer size. When this field = 0000b, the buffer
	//			size is determined by BSIZE. If this field is something other than
	//			0000b, the receive buffer size is the number represented in KB:
	//			For example, 0001b = 1 KB (1024 Bytes).)
	//	31 - Reserved
	e1000_write_cmd( nic, REG_NIC_RCTL, 0x402801E);



	// Transmit
	e1000_write_cmd( nic, REG_NIC_TDBAL0 , nic->transmit[0].base_l);
	e1000_write_cmd( nic, REG_NIC_TDBAH0 , nic->transmit[0].base_h);
	e1000_write_cmd( nic, REG_NIC_TDLEN0 , nic->transmit[0].length);
	e1000_write_cmd( nic, REG_NIC_TDH0 , nic->transmit[0].head);
	e1000_write_cmd( nic, REG_NIC_TDT0 , nic->transmit[0].tail);

	e1000_write_cmd( nic, REG_NIC_TXDCTL , (0x01000000 | 0x003F0000)); 


	// bits:
	//	0 - Reserved
	//	1 - Transmit Enable (EN)
	//	2 - Reserved
	//	3 - Pad Short Packets
	//	11:4 - Collision Threshold
	//	21:12 - Collision Distance (	Half-Duplex – 512-byte time (200h) 
	//					Full-Duplex – 64 byte time (3Fh))
	//	22 - Software XOFF Transmission
	//	23 - Reserved
	//	24 - Re-transmit on Late Collision (TCTL.RTLC)
	//	25 - Underrun No Re-Transmit
	//	27:26 - Tx Descriptor Minimum Threshold
	//	28 - Multiple Request Support
	//	31:29 - Reserved 
	
	e1000_write_cmd( nic, REG_NIC_TCTL, ( 0x00000ff0 | 0x003ff000 | 0x00000008 | 0x00000002));


	// bits:
	//	9:0 - IPG Back to Back (IPGT)
	//	19:10 - IPG Part 1 (IPGT1)
	//	29:20 - IPG After Deferral (IPGT2)
	//	31:30 - Reserved

	// IPGT = 8
    	// IPGR1 = 2
    	// IPGR2 = 10
	e1000_write_cmd( nic, REG_NIC_TIPG,(  0xA << 20 | 0x2 << 10 | 0x00000008));
	




	/*

	SLU (bit 6) - Set Link Up
	When the MAC link mode is set for 10/100/1000Base-T mode
	(internal/external PHY), Set Link Up must be set to 1b. to permit
	the MAC to recognize the LINK signal from the PHY, which
	indicates the PHY has established link, and to receive and
	transmit data
	The “Set Link Up” is normally initialized to 0b. However, if bit 10
	of Word 14h/24h is set in the EEPROM/NVM then it is initialized
	to 1b.


	*/
	val = e1000_read_cmd ( nic, REG_NIC_CTRL);
	e1000_write_cmd( nic, REG_NIC_CTRL, val | 0x40);


	return (-1);

}


/*
	Esta função baseado no código do Fred Nora

*/


int e1000_send_arp ( unsigned char source_ip[4], unsigned char target_ip[4], unsigned char target_mac[6] ) {
	int i;
	ether_header_t *eh;
	ether_arp_t *arp;

	if(nic_base->magic != 0x1234) {
		return (-1);
	}


	//configurando a estrutura do dispositivo,

	nic_base->ip[0] = source_ip[0];  //192;
	nic_base->ip[1] = source_ip[1];  //168;
	nic_base->ip[2] = source_ip[2];  //1;    	
	nic_base->ip[3] = source_ip[3];  //112; 


	//
	// ====================== ## ETH HEADER ## ====================
	//
	
	eh = (void *) malloc ( sizeof(struct ether_header ) );
	
	if ( (void*) eh == NULL)
	{
		print("struct eh fail");
		return (-1);
	}
	
	for( i=0; i<6; i++)
	{
		eh->src[i] = nic_base->mac[i];    	//source ok
		eh->dst[i] = target_mac[i];             //dest. (broadcast)	
	}	
	
	eh->type = (uint16_t) ToNetByteOrder16(ETH_TYPE_ARP);

	//
	// ==================== ## ARP ## ==========================
	//

	arp = (void *) malloc ( sizeof(struct  ether_arp) );
	
	if ( (void*) arp == NULL)
	{
		print("struct h fail");
		while(1);
	}
	
    	//Hardware type (HTYPE)
	arp->type = 0x0100; // (00 01)
	
	//Protocol type (PTYPE)
	arp->proto = 0x0008;  //(08 00)    
	
	//Hardware address length (MAC)
	arp->hlen = 6;
	
	////Protocol address length (IP)
	arp->plen = 4;
	
	
	//Operation (OPER) (dois bytes invertidos)
	//arp->op = ToNetByteOrder16(ARP_OPC_REPLY);  	
    	arp->op = ToNetByteOrder16(ARP_OPC_REQUEST);
	
	//mac
	for( i=0; i<6; i++)
	{
		arp->arp_sha[i] = nic_base->mac[i];  //sender mac
		arp->arp_tha[i] = target_mac[i];               //target mac
	}	
	
	//ip
	for ( i=0; i<4; i++)
	{
		arp->arp_spa[i] = source_ip[i];    //sender ip
		arp->arp_tpa[i] = target_ip[i];    //target ip
	}


	// ## quem ? ##
	//uint16_t old = 0;//currentNIC->tx_cur;
	

	// ## Copiando o pacote no buffer ##
	
	//pegando o endereço virtual do buffer na estrutura do dispositivo.	
	transmit_t *tx = (transmit_t *)nic_base->tx;// nic_base->transmit[0].virt + (16*old);

	unsigned char *buffer = (unsigned char *)tx->phy_l;
	
	unsigned char *src_ethernet = (unsigned char *) eh; 
	unsigned char *src_arp      = (unsigned char *) arp;
	
	//copiando o header ethernet
	//copiando o arp logo após do header ethernet


	for(i=0; i<14;i++){
		buffer[i] = src_ethernet[i];
	}

	for(i=0; i<28;i++){
		buffer[i + 14] = src_arp[i];
	}


	// Ethernet frame length = ethernet header (MAC + MAC + ethernet type) + ethernet data (ARP header)
	//O comprimento deve ser o tamanho do header etherne + o tamanho do arp.
	
	//len;
	tx->length = (ETHERNET_HEADER_LENGHT + ARP_HEADER_LENGHT);


	//cmd: bits
	//IDE VLE DEXT RSV RS IC IFCS EOP
        //IDE (bit 7) - Interrupt Delay Enable
        //VLE (bit 6) - VLAN Packet Enable
        //DEXT (bit 5) - Descriptor extension (#importante: '0' for legacy mode)
        //RSV (bit 4) - Reserved
        //RS (bit 3) - Report status
        //IC (bit 2) - Insert checksum
        //IFCS (bit 1) - Insert FCS (CRC)
        //EOP (bit 0) - End of packet

	tx->cmd = 0x1B;
	tx->status = 0;
	
	e1000_write_cmd( nic_base, REG_NIC_TDT0, 7);

	free(eh);
	free(arp);

	
	int spin = 10000000;
	
	while (!((long)tx->status) && spin)
	{
		spin--;
		//nothing
	}

	if(!spin) return (-1);
	
	return 0;
 			

}
