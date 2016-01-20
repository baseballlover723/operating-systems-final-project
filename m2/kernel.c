/* Morgan Cook, Josh Green, Philip Ross Milestone 1
   team 1-F
   */

int PRINT_INTERRUPT = 0x10;
int READ_CHAR_INTERRUPT = 0x16;
int MAX_BUFFER_SIZE = 80;

void printString(char str[]);
void printChar(char c);
int readChar();
void readString(char str[]);
void handleInterrupt21(int ax, int bx, int cx, int dx);

int main() {
    char line[80];
    line[0] = "m";
    line[1] = "o";
    line[2] = "r";
    printString("Hello World!\n");
    printString("Enter a line: ");
    readString(line);
    printString(line);
    handleInterrupt21(0, 0, 0, 0);
    interrupt(0x21, 0, 0, 0, 0);


    while (1) {}
    return 0;
}

void printString(char* str) {
    int index = 0;
    while (1) {
        if (str[index] == '\0') {
            break;
        }
        printChar(str[index]);
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

void handleInterrupt21(int ax, int bx, int cx, int dx) {
    printString("This is interrupt handler 21");
}



