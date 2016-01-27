/* Morgan Cook, Josh Green, Philip Ross Milestone 2
   team 1-F
   */

int PRINT_INTERRUPT = 0x10;
int READ_CHAR_INTERRUPT = 0x16;
int MAX_BUFFER_SIZE = 80;

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

int main() {
    /*    char line[80];
          char buffer[512];
          printString("Hello World!");

          newline();

          printString("Enter a line: ");
          readString(line);
          printString(line);

          newline(); 

          readSector(buffer, 30);
          printString(buffer);

          newline();

          handleInterrupt21(0, 0, 0, 0);
          interrupt(0x21, 0, 0, 0, 0);

          newline();

*/
    char buffer[13312];  /* this is the maximum size of a file */
        makeInterrupt21();

        /*interrupt(0x21, 3, "messag\0", buffer, 0);  /* read the file into buffer */
        /*interrupt(0x21, 0, buffer, 0, 0);      /* print out the file */
        interrupt(0x21, 4, "tstprg\0", 0x2000, 0);
    printString("End of program");
    while (1) {}
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
    printString("This is interrupt handler 21\n");
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
    }
}

void readFile(char* fileName, char* buffer) {
    int index;
    int i;
    int matches;
    int sectorNumb;
    char* directory;
    directory = buffer;
    readSector(directory, 2);
    for (index = 0; index < 16; index++) {
        directory += index * 32;
        matches = 1;
        for (i = 0; i<6; i++) {
            if (directory[i] != fileName[i]) {
                matches = 0;
                break;
            }
        }
        if (matches) {
            sectorNumb = 0;
            while (1) {
                sectorNumb = directory[i];
                if (sectorNumb == 0) {
                    break;
                }
                readSector(buffer, sectorNumb);
                buffer += 512;
                i++;
            }
            break;
            /*printString(buffer);*/
        }
    }    
}

void executeProgram(char* name, int segment) {
  int i = 0;
  char currChar;
  char buffer[13312];
  readFile(name, buffer);
  printString(buffer);
  

  while(1) {
    if(buffer[i] == "\0") {
      break;
    }
    putInMemory(segment, segment + i, buffer[i]);
    i++;
    }
  launchProgram(segment);
}

