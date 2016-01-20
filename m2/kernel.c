/* Morgan Cook, Josh Green, Philip Ross Milestone 1
team 1-F
*/

int PRINT_INTERRUPT = 0x10;
char ah = 0xe;

void printString(char str[]);
void printChar(char c);
int main() {
    printString("Hello World!\0");

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
}

void printChar(char c) {
    interrupt(PRINT_INTERRUPT, ah * 256 + c, 0, 0, 0);
}

