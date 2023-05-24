#ifndef VARIABLES_INCL
#define VARIABLES_INCL
#include "variables.h"
#endif
#ifndef STRING_INCL
#define STRING_INCL
#include "string.h"
#endif

/*djb2 hash taken from http://www.cse.yorku.ca/~oz/hash.html*/
unsigned long hash(unsigned char* str) {
    unsigned long hash = 5381;
    int c;

    while (c = *str++) {
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }

    return hash;
}

void command_handler_main(char* cmd) {
    char commands[16][256] = { {0} };

    int j = 0;
    int q = 0;

    for (int i = 0; i < strlen(cmd); i++) {
        if (cmd[i] == ' ') {
            j++;
            q = 0;
        }
        else {
            strncat(commands[j], &cmd[i], 1);
            q++;
        }
    }

    char* tmpcmd = commands[0];
    unsigned long command = hash(tmpcmd);
    unsigned long args[16];
    for (int q = 1; q < 16; q++) {
        if (commands[q][0] != '\0') {
            tmpcmd = commands[q];
            args[q - 1] = hash(tmpcmd);
        }
    }

	/*switch cmd hash*/
    switch (command) {
        case 0xF3B6D8C: //clear
			current_loc = 0;
            clear_screen();
            break;
		case 0x7C97D2EE: //help
			kprint("Available commands:");
			kprint_newline();
			kprint("clear");
			kprint_newline();
			kprint("help");
			kprint_newline();
			kprint("echo");
			kprint_newline();
		case 0x7C9624C4: //echo
			for (int i = 1; i < 16; i++) {
				if (commands[i][0] != '\0') {
					kprint(commands[i]);
					kprint(" ");
				}
			}
			kprint_newline();
			break;
		case 0x5977D2: //fg
			//set fg color
			if (args[0] != NULL) {
				switch(args[0]) {
					case 0xF294442: //black
						fgcolor = 0x0;
						color = (bgcolor << 4) | fgcolor;
						break;
					case 0x7C94A78D: //blue
						fgcolor = 0x1;
						color = (bgcolor << 4) | fgcolor;
						break;
					case 0xF871A56: //green
						fgcolor = 0x2;
						color = (bgcolor << 4) | fgcolor;
						break;
					case 0x7C9568B0: //cyan
						fgcolor = 0x3;
						color = (bgcolor << 4) | fgcolor;
						break;
					case 0xB88A540: //red
						fgcolor = 0x4;
						color = (bgcolor << 4) | fgcolor;
						break;
					case 0xB4110202: //magenta
						fgcolor = 0x5;
						color = (bgcolor << 4) | fgcolor;
						break;
					case 0xF2CCCAD: //brown
						fgcolor = 0x6;
						color = (bgcolor << 4) | fgcolor;
						break;
					case 0x7C977C78: //gray
						fgcolor = 0x8;
						color = (bgcolor << 4) | fgcolor;
						break;
					case 0x7C9C4737: //pink
						fgcolor = 0xD;
						color = (bgcolor << 4) | fgcolor;
						break;
					case 0x297FF6E1: //yellow
						fgcolor = 0xE;
						color = (bgcolor << 4) | fgcolor;
						break;
					case 0x10A33986: //white
						fgcolor = 0xF;
						color = (bgcolor << 4) | fgcolor;
						break;
					default:
						kprint("Invalid color");
						kprint_newline();
						break;
				}
			}
			else kprint("Invalid color");
			break;	

		case 0x59774E: //bg
			if(args[0] != NULL) {
				switch(args[0]){
					case 0xFDCAE5D: //light variation of color
						switch(args[1]){
							case 0x7C977C78: //gray
								bgcolor = 0x7;
								color = (fgcolor << 4) | bgcolor;
								color = (bgcolor << 4) | fgcolor;
								break;
							case 0x7C94A78D: //blue
								bgcolor = 0x9;
								color = (fgcolor << 4) | bgcolor;
								color = (bgcolor << 4) | fgcolor;
								break;
							case 0xF871A56: //green
								bgcolor = 0xA;
								color = (fgcolor << 4) | bgcolor;
								color = (bgcolor << 4) | fgcolor;
								break;
							case 0x7C9568B0: //cyan
								bgcolor = 0xB;
								color = (fgcolor << 4) | bgcolor;
								color = (bgcolor << 4) | fgcolor;
								break;
							case 0xB88A540: //red
								bgcolor = 0xC;
								color = (fgcolor << 4) | bgcolor;
								color = (bgcolor << 4) | fgcolor;
								break;
							default:
								kprint("Invalid color");
								kprint_newline();
								break;
						}
						break;
					case 0xF294442: //black
						bgcolor = 0x0;
						color = (fgcolor << 4) | bgcolor;
						color = (bgcolor << 4) | fgcolor;
						break;
					case 0x7C94A78D: //blue
						bgcolor = 0x1;
						color = (fgcolor << 4) | bgcolor;
						color = (bgcolor << 4) | fgcolor;
						break;
					case 0xF871A56: //green
						bgcolor = 0x2;
						color = (fgcolor << 4) | bgcolor;
						color = (bgcolor << 4) | fgcolor;
						break;
					case 0x7C9568B0: //cyan
						bgcolor = 0x3;
						color = (fgcolor << 4) | bgcolor;
						color = (bgcolor << 4) | fgcolor;
						break;
					case 0xB88A540: //red
						bgcolor = 0x4;
						color = (fgcolor << 4) | bgcolor;
						color = (bgcolor << 4) | fgcolor;
						break;
					case 0xB4110202: //magenta
						bgcolor = 0x5;
						color = (fgcolor << 4) | bgcolor;
						color = (bgcolor << 4) | fgcolor;
						break;
					case 0xF2CCCAD: //brown
						bgcolor = 0x6;
						color = (fgcolor << 4) | bgcolor;
						color = (bgcolor << 4) | fgcolor;
						break;
					case 0x7C977C78: //gray
						bgcolor = 0x8;
						color = (fgcolor << 4) | bgcolor;
						color = (bgcolor << 4) | fgcolor;
						break;
					case 0x7C9C4737: //pink
						bgcolor = 0xD;
						color = (fgcolor << 4) | bgcolor;
						color = (bgcolor << 4) | fgcolor;
						break;
					case 0x297FF6E1: //yellow
						bgcolor = 0xE;
						color = (fgcolor << 4) | bgcolor;
						color = (bgcolor << 4) | fgcolor;
						break;
					case 0x10A33986: //white
						bgcolor = 0xF;
						color = (fgcolor << 4) | bgcolor;
						color = (bgcolor << 4) | fgcolor;
						break;
					default:
						kprint("Invalid color");
						kprint_newline();
						break;
				}
			}
			else kprint("Invalid color");
			break;

        default:
            kprint("Command not found, hash: 0x");
            char hashstr[17];
            ltoa(command, hashstr, 16);
            kprint(hashstr);
            kprint(", cmd: ");
            kprint(commands[0]);
			kprint_newline();
            break;
    }
	return;
}