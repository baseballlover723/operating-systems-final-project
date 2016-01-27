/* Morgan Cook, Josh Green, Philip Ross Milestone 2
   team 1-F
   */

#define PRINT_INTERRUPT 0x10
#define READ_CHAR_INTERRUPT 0x16
#define MAX_BUFFER_SIZE 80

#define SHELL "shell\0"

void printString(char str[]);
void printChar(char c);
int readChar();
void readString(char str[]);
void readSector(char* buffer, int sector);
int mod(a, b);
int div(a, b);
void newline();
void handleInterrupt21(int ax, int bx, int cx, int dx);
void readFile(char* fileName, char* buffer);
void executeProgram(char* name, int segment);
void terminate();

int main() {
  makeInterrupt21();
  interrupt(0x21, 4, SHELL, 0x2000, 0);
  return 0;
}

void newline() {
    printString("\n****\n\n");
} 

void printString(char* str) {
    int index = 0;
    while (1) {
        if (str[index] == '\0') {
            break;
        } else if (str[index] == '\n') {
            printChar(0xa);
            printChar(0xd);
        } else {
            printChar(str[index]);
        }
        index++;

    }
    return;
}

void printChar(char c) {
    interrupt(PRINT_INTERRUPT, 0xe * 256 + c, 0, 0, 0);
}

int readChar() {
    return interrupt(READ_CHAR_INTERRUPT, 0, 0, 0, 0);
}

void readString(char str[]) {
    int index = 0;
    while (1) {
        int readCharChar = readChar();
        printChar(readCharChar);
        if (readCharChar == 0x8) {
            printChar(' ');
            printChar(readCharChar);
            str[index] = '\0';
            if (index > 0) {
                index--;
            }
        } else {
            str[index] = readCharChar;
            index++;
        }
        if (index > MAX_BUFFER_SIZE - 2 || readCharChar == 0xd) {
            str[index] = 0xa;
            str[index + 1] = 0x0;
            printString("\n");
            break;
        }
    }
}

int mod(int a, int b) {
    while (a >= b) {
        a = a - b;
    }
    return a;
}

int div(a, b) {
    int quotient = 0;
    while ((quotient  + 1)* b  <= a) {
        quotient  = quotient  + 1;
    }
    return quotient;
}

void readSector(char* buffer, int sector) {
    int ax = 2 * 256 + 1;
    int bx = buffer;
    int cx = div(sector, 36) * 256 + mod(sector, 18) + 1;
    int dx = mod(div(sector, 18), 2) * 256;
    interrupt(0x13, ax, bx, cx, dx);
}

void handleInterrupt21(int ax, int bx, int cx, int dx) {
    /*readFile(bx, cx);*/
    if (ax == 0) {
        printString(bx);
    } else if(ax ==1) {
        readString(bx);
    } else if(ax == 2) {
        readSector(bx, cx);
        printString(bx);
    } else if(ax == 3) {
        readFile(bx, cx);
    } else if(ax == 4) {
        executeProgram(bx, cx);
    } else if(ax == 5) {
      terminate();
    }
}

void readFile(char* fileName, char* buffer) {
    int index;
    int i;
    int matches;
    int sectorNumb;
    char directory[512];
    readSector(directory, 2);
    
    for (index = 0; index < 512; index += 32) {
        matches = 1;
        for (i = 0; i<6; i++) {
            if (directory[i + index] != fileName[i]) {
                matches = 0;
                break;
            }
        }
        if (matches) {
            for (i=0; directory[6 + index + i] != 0x00; i++) {
                printString("read sector ");
                printhex(directory[6+index+i]);
                printString("\n");
                readSector(buffer + 512 * i, directory[6 + index + i]);
            }
            return;
            /*printString(buffer);*/
        }
    }    
}

void executeProgram(char* name, int segment) {
    int i;
    char buffer[13312];
    readFile(name, buffer);

    for (i = 0; i < 13312; i++) {        
        putInMemory(segment, i, buffer[i]);
    }
    launchProgram(segment);
}

void terminate() {
  interrupt(0x21, 4, SHELL, 0x2000, 0);
}

