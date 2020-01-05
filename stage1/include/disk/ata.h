#ifndef __ATA_H__
#define __ATA_H__


// base address 
uint32_t ATA_BAR0;    // Primary Command Block Base Address
uint32_t ATA_BAR1;    // Primary Control Block Base Address
uint32_t ATA_BAR2;    // Secondary Command Block Base Address
uint32_t ATA_BAR3;    // Secondary Control Block Base Address
uint32_t ATA_BAR4;    // Legacy Bus Master Base Address
uint32_t ATA_BAR5;    // AHCI Base Address / SATA Index Data Pair Base Address



#define PCI_CALSS_MASS_STORAGE  1

// Controladores de unidades ATA
#define ATA_IDE_CONTROLLER      0x1
#define ATA_RAID_CONTROLLER     0x4
#define ATA_AHCI_CONTROLLER     0x6


// IO Space Legacy BARs IDE 
#define ATA_IDE_BAR0 0x1F0  // Primary Command Block Base Address
#define ATA_IDE_BAR1 0x3F6  // Primary Control Block Base Address
#define ATA_IDE_BAR2 0x170  // Secondary Command Block Base Address
#define ATA_IDE_BAR3 0x376  // Secondary Control Block Base Address
#define ATA_IDE_BAR4 0      // Bus Master Base Address
#define ATA_IDE_BAR5 0      //



// ATA/ATAPI Command Set

#define ATA_CMD_CFA_ERASE_SECTORS               0xC0
#define ATA_CMD_CFA REQUEST_EXTENDED_ERROR_CODE 0x03
#define ATA_CMD_CHECK_MEDIA_CARD_TYPE           0xD1
#define ATA_CMD_CHECK_POWER_MODE                0xE5
#define ATA_CMD_DEVICE_RESET                    0x08
#define ATA_CMD_EXECUTE_DEVICE_DIAGNOSTIC       0x90
#define ATA_CMD_FLUSH_CACHE                     0xE7
#define ATA_CMD_FLUSH_CACHE_EXT                 0xEA
#define ATA_CMD_IDENTIFY_DEVICE                 0xEC
#define ATA_CMD_IDENTIFY_PACKET_DEVICE          0xA1
#define ATA_CMD_PACKET                          0xA0
#define ATA_CMD_READ_BUFFER                     0xE4
#define ATA_CMD_READ_DMA                        0xC8
#define ATA_CMD_READ_DMA_EXT                    0x25
#define ATA_CMD_READ_SECTORS                    0x20
#define ATA_CMD_READ_SECTORS_EXT                0x24
#define ATA_CMD_WRITE_BUFFER                    0xE8
#define ATA_CMD_WRITE_DMA                       0xCA
#define ATA_CMD_WRITE_DMA_EXT                   0x35
#define ATA_CMD_WRITE_SECTORS                   0x30
#define ATA_CMD_WRITE_SECTORS_EXT               0x34

// ATAPI descrito no SCSI
#define ATAPI_CMD_READ  0xA8
#define ATAPI_CMD_EJECT 0x1B

//ATA bits de status control (alternativo)
#define ATA_SC_HOB      0x80    // High Order Byte
#define ATA_SC_SRST     0x04    // Soft Reset
#define ATA_SC_nINE     0x02    // INTRQ




//ATA bits de status 
#define ATA_SR_BSY      0x80    // Busy
#define ATA_SR_DRDY     0x40    // Device Ready
#define ATA_SR_DF       0x20    // Device Fault
#define ATA_SR_DSC      0x10    // Device Seek Complete
#define ATA_SR_DRQ      0x08    // Data Request
#define ATA_SR_SRST     0x04    // 
#define ATA_SR_IDX      0x02    // Index
#define ATA_SR_ERR      0x01    // Error

//ATA bits de errro apos a leitura.
#define ATA_ER_BBK      0x80    // 
#define ATA_ER_UNC      0x40    //
#define ATA_ER_MC       0x20    //
#define ATA_ER_IDNF     0x10    //
#define ATA_ER_MCR      0x08    //
#define ATA_ER_ABRT     0x04    //
#define ATA_ER_TK0NF    0x02    //
#define ATA_ER_AMNF     0x01    //


// Registradores 
#define ATA_REG_DATA 0x00
#define ATA_REG_ERROR 0x01
#define ATA_REG_FEATURES 0x01
#define ATA_REG_SECCOUNT 0x02
#define ATA_REG_LBA0 0x03
#define ATA_REG_LBA1 0x04
#define ATA_REG_LBA2 0x05
#define ATA_REG_DEVSEL 0x06
#define ATA_REG_CMD 0x07
#define ATA_REG_STATUS 0x07


// Devices
#define ATA_MASTER_DEV  0x00
#define ATA_SLAVE_DEV   0x01

// Bus
#define ATA_PRIMARY     0x00
#define ATA_SECONDARY   0x01

// ATA type
#define ATA_DEVICE_TYPE     0x00
#define ATAPI_DEVICE_TYPE   0x01

#define ATADEV_UNKNOWN	0x00
#define ATADEV_PATA	0x01
#define ATADEV_SATA	0x02
#define ATADEV_PATAPI	0x03
#define ATADEV_SATAPI	0x04



// Modo de transferencia
#define ATA_PIO_MODO 0 
#define ATA_DMA_MODO 1
#define ATA_LBA48    48
#define ATA_LBA28    28


#define IDE_IRQ14 0
#define IDE_IRQ15 1



typedef struct _ATA
{
    	// bytes per sector
   	UINT32	bps;

	// mode of transfere (0 = DMA or 1 = PIO)
    	UINT32	mode;

	// LBA28 or LBA48
    	UINT32	lba_type;

	// dev_num 0 = Primary master
	// dev_num 1 = Primary slava
	// dev_num 2 = Secondary master
	// dev_num 3 = Secondary slave
    	UINT32	dev_num;

	// dev_type 0 = ATA, 1 = ATAPI
    	UINT32	dev_type;

	// channel 0 = Primary
	// channel 0 = Secondary
    	UINT32	channel;
 
    	UINT32	cmd_block_base_addr;
    	UINT32	ctrl_block_base_addr;
    	UINT32	bus_master_base_addr;

	// IRQn 14 or 15 
	UINT32	irqn;    


}__attribute__ ((packed)) ATA;


UINTN ata_initialize();
UINTN ata_read_sector(	IN UINTN p,
			IN UINTN count,
			IN UINT64 addr,
			OUT VOID *buffer);


#endif