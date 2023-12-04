extern void kprint(char *str);

extern void kprint_newline(void);

extern void clear_screen(void);

extern char inb(unsigned short int port);

extern void outb(unsigned short int port, unsigned char data);

extern void outl(unsigned short int port, unsigned long data);

extern unsigned long inl(unsigned short int port);

extern void memset(void* ptr, int value, unsigned int num);