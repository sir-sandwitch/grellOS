//sata read/write functions

#define	SATA_SIG_ATA	0x00000101	// SATA drive
#define	SATA_SIG_ATAPI	0xEB140101	// SATAPI drive
#define	SATA_SIG_SEMB	0xC33C0101	// Enclosure management bridge
#define	SATA_SIG_PM	0x96690101	// Port multiplier
 
#define AHCI_DEV_NULL 0
#define AHCI_DEV_SATA 1
#define AHCI_DEV_SEMB 2
#define AHCI_DEV_PM 3
#define AHCI_DEV_SATAPI 4
 
#define HBA_PORT_IPM_ACTIVE 1
#define HBA_PORT_DET_PRESENT 3
 
void probe_port(HBA_MEM *abar);
 
// Check device type
static int check_type(HBA_PORT *port);
#define	AHCI_BASE	0x400000	// 4M
 
#define HBA_PxCMD_ST    0x0001
#define HBA_PxCMD_FRE   0x0010
#define HBA_PxCMD_FR    0x4000
#define HBA_PxCMD_CR    0x8000
 
extern void port_rebase(HBA_PORT *port, int portno);
 
// Start command engine
extern void start_cmd(HBA_PORT *port);
 
// Stop command engine
extern void stop_cmd(HBA_PORT *port);

extern void sata_read(unsigned long lba, unsigned long count, unsigned long* buf, HBA_PORT* port);