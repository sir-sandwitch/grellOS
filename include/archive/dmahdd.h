#ifndef STDIO_INCL
#define STDIO_INCL
#include "stdio.h"
#endif

#ifndef VARIABLES_INCL
#define VARIABLES_INCL
#include "variables.h"
#endif

#ifndef STRING_INCL
#define STRING_INCL
#include "string.h"
#endif

#ifndef TASK_INCL
#define TASK_INCL
#include "taskschedule.h"
#endif

#define DMA_READ 0xC8
#define DMA_WRITE 0xCA

#define BUS_PRIMARY_IO_START 0x1F0
#define BUS_PRIMARY_IO_END 0x1F7
#define BUS_PRIMARY_IO_SIZE 8
#define BUS_PRIMARY_CONTROL 0x3F6
#define BUS_PRIMARY_BUS_MASTER_COMMAND 0x1F7
#define BUS_PRIMARY_BUS_MASTER_STATUS 0x1F7
#define BUS_PRIMARY_BUS_MASTER_PRDT 0x8000
#define BUS_PRIMARY_BUS_MASTER_TRANSFER_LENGTH 0x8004
#define BUS_PRIMARY_IRQ 14
#define BUS_SECONDARY_IO_START 0x170
#define BUS_SECONDARY_IO_END 0x177
#define BUS_SECONDARY_IO_SIZE 8
#define BUS_SECONDARY_CONTROL 0x376
#define BUS_SECONDARY_IRQ 15

#define SECTOR_SIZE 512
#define PRDT_ENTRY_SIZE 16



/*dma disk drivers*/

int getDriveBusType();
int initDrive();

extern struct PRDT_entry{
    uint32_t byteAddress;
    uint32_t byteCount;
    uint32_t reserved;
};

/*bus PRIMARY register*/
typedef volatile struct BMR{
    /*Primary Bus*/
    uint8_t command;
    uint8_t status;
    uint32_t PRDT_byteAddress;
    /*Secondary Bus*/
    uint8_t command2;
    uint8_t status2;
    uint32_t PRDT_byteAddress2;
}BMR_t;


/*return info on drive*/
int* getDriveInfo(){
    int driveInfo[256];
    /*check if drive is DMA or AHCI*/
    if(getDriveBusType() == 1){
        return driveInfo;
    }
    else{
        /*DMA*/
        outb(BUS_PRIMARY_IO_START + 6, 0xA0); //select master drive
        
        for (int i = 0; i < 4; i++){
            outb(BUS_PRIMARY_IO_SIZE + 2 + i, 0x00); //send 0x00 to each register
        }

        outb(BUS_PRIMARY_IO_START + 7, 0xEC); //send identify command

        uint8_t status = inb(BUS_PRIMARY_IO_START + 7); //read status register
        if(status == 0x00){
            return driveInfo;
        }
        else{
            while((status & 0b100) == 0 && (status & 0b1) == 0){
                status = inb(BUS_PRIMARY_IO_START + 7);
            }
        }


        
        return &driveInfo;
    }
}


/*check if drive bus is AHCI or DMA*/
int getDriveBusType(){
    HBA_MEM ahciMem;
    /*check bit 31 of AHCI GHC register*/
    if((ahciMem.ghc & 0x80000000) == 0x80000000){
        return 1; //AHCI
    }
    else{
        return 0; //DMA
    }
}

void dma_read(uint32_t lba, void* buffer, uint32_t count)
{
    struct PRDT_entry prdt;
    BMR_t bmr;
    /*set up PRDT*/
    uint32_t prdtAddress = (uint32_t)&prdt;
    prdt.byteAddress = (uint32_t)&buffer;
    prdt.byteCount = (uint32_t)count;
    prdt.reserved = (uint32_t)0x000000;

    /*set up BMR*/
    //grab BMR from PCI BAR4 bus and store it in bmr variable
    bmr.command = inb(BUS_PRIMARY_IO_START);
    bmr.status = inb(BUS_PRIMARY_IO_START + 2);
    bmr.PRDT_byteAddress = prdtAddress;
    //set BMR
    //set bit 8 of BMR command register to 1 (read)
    bmr.command = bmr.command | 0x08;
    outb(BUS_PRIMARY_IO_START, bmr.command);
    outb(BUS_PRIMARY_IO_START + 4, bmr.status);
    outl(BUS_PRIMARY_IO_START + 8, bmr.PRDT_byteAddress);

    /*set up drive*/
    //clear interrupt and error bits
    outb(BUS_PRIMARY_IO_START + 7, 0x00);
    //set drive to master
    outb(BUS_PRIMARY_IO_START + 6, 0xA0);
    //set sector count
    outb(BUS_PRIMARY_IO_START + 2, count);
    //set LBA
    outb(BUS_PRIMARY_IO_START + 3, (uint8_t)(lba & 0x000000FF));
    outb(BUS_PRIMARY_IO_START + 4, (uint8_t)((lba & 0x0000FF00) >> 8));
    outb(BUS_PRIMARY_IO_START + 5, (uint8_t)((lba & 0x00FF0000) >> 16));
    outb(BUS_PRIMARY_IO_START + 6, (uint8_t)((lba & 0xFF000000) >> 24));
    //set command
    outb(BUS_PRIMARY_IO_START + 7, 0x20);
    
    /*set Start/Stop bit*/
    //set bit 0 of BMR command register to 1 (start)
    bmr.command = bmr.command | 0x01;
    outb(BUS_PRIMARY_IO_START, bmr.command);
    //wait for interrupt
    while((bmr.status & 0x04) != 0x04){
        bmr.status = inb(BUS_PRIMARY_IO_START + 2);
    }
    //clear interrupt
    outb(BUS_PRIMARY_IO_START + 2, 0x04);
    //set bit 0 of BMR command register to 0 (stop)
    bmr.command = bmr.command & 0xFE;
    outb(BUS_PRIMARY_IO_START, bmr.command);
}

void dma_write(uint32_t lba, void* buffer, uint32_t count)
{
    struct PRDT_entry prdt;
    BMR_t bmr;
    /*set up PRDT*/
    uint32_t prdtAddress = (uint32_t)&prdt;
    prdt.byteAddress = (uint32_t)&buffer;
    prdt.byteCount = (uint32_t)count;
    prdt.reserved = (uint32_t)0x000000;

    /*set up BMR*/
    //grab BMR from PCI BAR4 bus and store it in bmr variable
    bmr.command = inb(BUS_PRIMARY_IO_START);
    bmr.status = inb(BUS_PRIMARY_IO_START + 2);
    bmr.PRDT_byteAddress = prdtAddress;
    //set BMR
    //set bit 8 of BMR command register to 0 (write)
    bmr.command &= ~(1 << 8);
    outb(BUS_PRIMARY_IO_START, bmr.command);
    outb(BUS_PRIMARY_IO_START + 4, bmr.status);
    outl(BUS_PRIMARY_IO_START + 8, bmr.PRDT_byteAddress);

    /*set up drive*/
    //clear interrupt and error bits
    outb(BUS_PRIMARY_IO_START + 7, 0x00);
    //set drive to master
    outb(BUS_PRIMARY_IO_START + 6, 0xA0);
    //set sector count
    outb(BUS_PRIMARY_IO_START + 2, count);
    //set LBA
    outb(BUS_PRIMARY_IO_START + 3, (uint8_t)(lba & 0x000000FF));
    outb(BUS_PRIMARY_IO_START + 4, (uint8_t)((lba & 0x0000FF00) >> 8));
    outb(BUS_PRIMARY_IO_START + 5, (uint8_t)((lba & 0x00FF0000) >> 16));
    outb(BUS_PRIMARY_IO_START + 6, (uint8_t)((lba & 0xFF000000) >> 24));
    //set command
    outb(BUS_PRIMARY_IO_START + 7, 0x30);
    
    /*set Start/Stop bit*/
    //set bit 0 of BMR command register to 1 (start)
    bmr.command = bmr.command | 0x01;
    outb(BUS_PRIMARY_IO_START, bmr.command);
    //wait for interrupt
    while((bmr.status & 0x04) != 0x04){
        bmr.status = inb(BUS_PRIMARY_IO_START + 2);
    }
    //clear interrupt
    outb(BUS_PRIMARY_IO_START + 2, 0x04);
    //set bit 0 of BMR command register to 0 (stop)
    bmr.command &= 0b11111110;
    outb(BUS_PRIMARY_IO_START, bmr.command);

}

/*initialize DMA drive*/
int initDrive(){
    /*check if drive is DMA or AHCI*/
    if(getDriveBusType() == 0){
        /*initialize DMA drive*/
        struct PRDT_entry prdt;
        BMR_t bmr;
        /*set up PRDT*/
        uint32_t prdtAddress = (uint32_t)&prdt;
        prdt.byteAddress = (uint32_t)0x000000;
        prdt.byteCount = (uint32_t)0x000000;
        prdt.reserved = (uint32_t)0x000000;

    
        //grab BMR from PCI BAR4 bus and store it in bmr variable
        bmr.command = inb(BUS_PRIMARY_IO_START);
        bmr.status = inb(BUS_PRIMARY_IO_START + 2);
        bmr.PRDT_byteAddress = prdtAddress;
        outb(BUS_PRIMARY_IO_START, bmr.command);
        outb(BUS_PRIMARY_IO_START + 4, bmr.status);
        outl(BUS_PRIMARY_IO_START + 8, bmr.PRDT_byteAddress);

        
        return 1;
    }
    else{
        /*fail out*/
        return 0;
    }
}