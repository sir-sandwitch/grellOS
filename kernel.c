#ifndef KEYBOARD_INCL
#include "include/keyboard.h"
#define KEYBOARD_INCL
#endif
#ifndef VARIABLES_INCL
#include "include/variables.h"
#define VARIABLES_INCL
#endif
#ifndef COMMAND_INCL
#include "include/command.h"
#define COMMAND_INCL
#endif
#ifndef STDIO_INCL
#define STDIO_INCL
#include "include/stdio.h"
#endif
#ifndef STRING_INCL
#define STRING_INCL
#include "include/string.h"
#endif
#ifndef TASK_INCL
#define TASK_INCL
#include "include/taskschedule.h"
#endif

struct IDT_entry {
    unsigned short int offset_lowerbits;
    unsigned short int selector;
    unsigned char zero;
    unsigned char type_attr;
    unsigned short int offset_higherbits;
};

struct IDT_entry IDT[IDT_SIZE];


void idt_init(void)
{
    unsigned long keyboard_address;
    unsigned long idt_address;
    unsigned long idt_ptr[2];

    /* populate IDT entry of keyboard's interrupt */
    keyboard_address = (unsigned long)keyboard_handler;
    IDT[0x21].offset_lowerbits = keyboard_address & 0xffff;
    IDT[0x21].selector = KERNEL_CODE_SEGMENT_OFFSET;
    IDT[0x21].zero = 0;
    IDT[0x21].type_attr = INTERRUPT_GATE;
    IDT[0x21].offset_higherbits = (keyboard_address & 0xffff0000) >> 16;

    /*load PIT timer*/
    unsigned long timer_address;
    timer_address = (unsigned long)pit_handler;
    IDT[0x20].offset_lowerbits = timer_address & 0xffff;
    IDT[0x20].selector = KERNEL_CODE_SEGMENT_OFFSET;
    IDT[0x20].zero = 0;
    IDT[0x20].type_attr = INTERRUPT_GATE;
    IDT[0x20].offset_higherbits = (timer_address & 0xffff0000) >> 16;



    /*     Ports
    *    PIC1    PIC2
    *Command 0x20    0xA0
    *Data     0x21    0xA1
    */

    /* ICW1 - begin initialization */
    write_port(0x20 , 0x11);
    write_port(0xA0 , 0x11);

    /* ICW2 - remap offset address of IDT */
    /*
    * In x86 protected mode, we have to remap the PICs beyond 0x20 because
    * Intel have designated the first 32 interrupts as "reserved" for cpu exceptions
    */
    write_port(0x21 , 0x20);
    write_port(0xA1 , 0x28);

    /* ICW3 - setup cascading */
    write_port(0x21 , 0x00);
    write_port(0xA1 , 0x00);

    /* ICW4 - environment info */
    write_port(0x21 , 0x01);
    write_port(0xA1 , 0x01);
    /* Initialization finished */

    /* mask interrupts */
    write_port(0x21 , 0xff);
    write_port(0xA1 , 0xff);

    /* fill the IDT descriptor */
    idt_address = (unsigned long)IDT ;
    idt_ptr[0] = (sizeof (struct IDT_entry) * IDT_SIZE) + ((idt_address & 0xffff) << 16);
    idt_ptr[1] = idt_address >> 16 ;

    load_idt(idt_ptr);
}

void pit_init(void){
    unsigned long timer_frequency = 1193180; /* in Hz */
    unsigned long divisor = 0;
    unsigned char low_byte = 0, high_byte = 0;

    /* calculate the divisor */
    divisor = timer_frequency / PIT_FREQUENCY;
    low_byte = (unsigned char)(divisor & 0xff);
    high_byte = (unsigned char)((divisor >> 8) & 0xff);

    /* send the command */
    write_port(0x43, 0x36);

    /* send the frequency divisor */
    write_port(0x40, low_byte);
    write_port(0x40, high_byte);
}

void pit_handler_main(void){
    registers_t* regs;
    cycleTask(regs);
    write_port(0x20, 0x20);
}

void kb_init(void)
{
    /* enable irq 0 & 1 */
    write_port(0x21 , 0b11111100);
}

void keyboard_handler_main(void) {
    unsigned char status;
    char keycode;

    write_port(0x20, 0x20);

    status = read_port(KEYBOARD_STATUS_PORT);
    /* Lowest bit of status will be set if buffer is not empty */
    if (status & 0x01) {
        keycode = read_port(KEYBOARD_DATA_PORT);
        if (keycode < 0) {
            return;
        }

        if (keycode == ENTER_KEY_CODE) {
            //cmdbuf[strlen(cmdbuf) - 1] = '\0';
            kprint_newline();
            command_handler_main(cmdbuf);
            cmdbuf[0] = '\0';
        }
        else if (keycode == BACK_KEY_CODE) {
            if (strlen(cmdbuf) > 0) {
                cmdbuf[strlen(cmdbuf) - 1] = '\0';
                current_loc = current_loc - 2;
                vidptr[current_loc++] = ' ';
                vidptr[current_loc++] = color;
                bsflag = 1;
            }
        }
        else {
            char str[2] = { keyboard_map[(unsigned char)keycode], '\0' };
            strcat(cmdbuf, str);
            kprint(str);
        }
    }
}

void kmain(void)
{
    char *str = "grellOS v0.2a";
    clear_screen();
    kprint(str);
    kprint_newline();
    kprint_newline();

    idt_init();
    pit_init();
    kb_init();

    while (1) {
        keyboard_handler_main();
    }
}
