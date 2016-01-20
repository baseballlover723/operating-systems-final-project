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

int main() {    
    char line[80];
    char buffer[512];
    char buffer2[512];
    printString("Hello World!");
    
    newline();
    
    printString("Enter a line to test read and print string: ");
    readString(line);
    printString(line);
    
    newline(); 
    
    readSector(buffer, 30);
    printString(buffer);
    
    newline();

    printString("Enter a line to test interrupt21: ");
    makeInterrupt21();
    interrupt(0x21,1,line,2,0);
    interrupt(0x21,0,line,2,0);
    interrupt(0x21,2,buffer2,30,0);

    newline();

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
    if (ax == 0) {
        printString(bx);
    } else if(ax ==1) {
        readString(bx);
    } else if(ax == 2) {
        readSector(bx, cx);
        printString(bx);
    } else if(ax >= 3) {
        printString("Error -> First parameter must be less than three!");
    }

}
