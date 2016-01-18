/* Morgan Cook, Josh Green, Philip Ross Milestone 1
team 1-F
*/

int videoMemoryStart = 0xB8000;
int currentAddress = 0x8000;
int color = 0x7;

void writeString(char str[]);
void writeChar(char c);
int main() {
    writeString("Hello World!\0");

    while(1) {}
    return 0;
}

void writeString(char* str) {    
    int index = 0;
    while (1) {
        if (str[index] == '\0') {
            break;
        }
        writeChar(str[index]);
        index++;
    }
}

void writeChar(char c) {
    putInMemory(0xB000, currentAddress, c);
    putInMemory(0xB000, currentAddress + 1, color);
    currentAddress += 2;
}

