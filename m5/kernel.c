/* Morgan Cook, Josh Green, Philip Ross Milestone 2
   team 1-F
   */

#define PRINT_INTERRUPT 0x10
#define READ_CHAR_INTERRUPT 0x16
#define MAX_STRING_SIZE 80

#define MAX_BUFFER_SIZE 512
#define MAX_INDEX_SIZE 32

typedef struct {
  int isActive;
  int stackPointer;
} process;

process processTable[8];
int currentProcess;

void printString(char str[]);
void printChar(char c);
int readChar();
void readString(char str[]);
void readSector(char* buffer, int sector);
void writeSector(char* buffer, int sector);
int mod(a, b);
int div(a, b);
void newline();
void handleInterrupt21(int ax, int bx, int cx, int dx);
void handleTimerInterrupt(int segment, int sp);
void readFile(char* fileName, char* buffer);
void deleteFile(char* fileName);
void executeProgram(char* name);
void terminate();
void killProcess(char *proc);
void quitAll();
void clearScreen();
void writeFile(char* name, char* buffer, int numberOfSectors);
void initializeProcessTable();

int main() {
  char shell[6];
  shell[0] = 's';
  shell[1] = 'h';
  shell[2] = 'e';
  shell[3] = 'l';
  shell[4] = 'l';
  shell[5] = '\0';
  currentProcess = 0;
  initializeProcessTable();
  makeInterrupt21();
  makeTimerInterrupt();
  interrupt(0x21, 4, shell, 0, 0);
  while(1);
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
        } else if (readCharChar != 0xd) {
            str[index] = readCharChar;
            index++;
        }
        if (index > MAX_STRING_SIZE - 1 || readCharChar == 0xd) {
            str[index] = 0x0;
            printChar(0xa);
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

void writeSector(char* buffer, int sector) {
    int ax = 3 * 256 + 1;
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
    } else if(ax == 3) {
      readFile(bx, cx);
    } else if(ax == 4) {
      executeProgram(bx);
    } else if(ax == 5) {
      terminate();
    } else if(ax == 6) {
      writeSector(bx, cx);
    } else if(ax == 7) {
      deleteFile(bx);
    } else if(ax==8) {
      printhex(ax);
    	writeFile(bx, cx, dx);
    } else if(ax == 9) {
      killProcess(bx);
    } else if(ax == 10) {
      quitAll();
    } else if(ax == 11) {
      clearScreen();
    }
}

void handleTimerInterrupt(int segment, int sp) {
  int i;
  int end;
  int newSegment;
  int newSP;
  int segCheck;
  segCheck = (segment / 0x1000) - 2;
  if (segCheck == currentProcess) {
    processTable[currentProcess].stackPointer = sp;
  }
  for (i = currentProcess+1; i < 8; i++) {
    if (processTable[i].isActive) {
      newSegment = (i + 2) * 0x1000;
      newSP = processTable[i].stackPointer;
      currentProcess = i;
      returnFromTimer(newSegment, newSP);
    }
  }
  for (i = 0; i <= currentProcess; i++) {
    if (processTable[i].isActive) {
      newSegment = (i + 2) * 0x1000;
      newSP = processTable[i].stackPointer;
      currentProcess = i;
      returnFromTimer(newSegment, newSP);
    }
  }
  
  returnFromTimer(segment, sp);
}

void readFile(char* fileName, char* buffer) {
    int index;
    int i;
    int matches;
    int sectorNumb;
    char directory[MAX_BUFFER_SIZE];
    buffer[0] = 'F';
    buffer[1] = 'N';
    buffer[2] = 'F';
    buffer[3] = '\n';
    buffer[4] = '\0';
    readSector(directory, 2);
    for (index = 0; index < MAX_BUFFER_SIZE; index += MAX_INDEX_SIZE) {
        matches = 1;
        for (i = 0; i<6 && fileName[i] != '\0'; i++) {
            if (directory[i + index] != fileName[i]) {
                matches = 0;
                break;
            }
        }
        if (i < 6 && directory[i + index] != fileName[i]) {
          matches = 0;
        }
        if (matches) {
            for (i=0; directory[6 + index + i] != 0x00; i++) {
              /*printString("read sector ");
                printhex(directory[6+index+i]);
                printString("\n");*/
                readSector(buffer + MAX_BUFFER_SIZE * i, directory[6 + index + i]);
            }
            return;
        }
    }    
}

void deleteFile(char* fileName) {
  int index;
  int i;
  int matches;
  int sectorNum;
  char directory[MAX_BUFFER_SIZE];
  char map[MAX_BUFFER_SIZE];
  readSector(directory, 2);
  readSector(map, 1);

  for (index = 0; index < MAX_BUFFER_SIZE; index += MAX_INDEX_SIZE) {
    matches = 1;
    for (i = 0; i<6 && fileName[i] != '\0'; i++) {
      if (directory[i + index] != fileName[i]) {
        matches = 0;
        break;
      }
    }
    if (i < 6 && directory[i + index] != fileName[i]) {
      matches = 0;
    }
    if (matches) {
      directory[index] = 0x00;
      for (i=0; directory[6 + index + i] != 0x00; i++) {
        sectorNum = directory[6 + index + i];
        map[sectorNum - 1] = 0x00;
      }
      writeSector(directory, 2);
      writeSector(map, 1);
      return;
    }
  }
}

void executeProgram(char* name) {
  int i, j;
    int segment;
    char buffer[13312];
    readFile(name, buffer);
    setKernelDataSegment();
    for (i = 0; i < 8; i++) {
      if (!processTable[i].isActive) {
        processTable[i].stackPointer = 0xff00;
        break;
      }
    }
    restoreDataSegment();

    for (j = 0; j < 13312; j++) {        
      putInMemory((i + 2) * 0x1000, j, buffer[j]);
    }
    if (i != 8) {
      initializeProgram((i + 2) * 0x1000);
    }
    setKernelDataSegment();
    processTable[i].isActive = 1;
    restoreDataSegment();
    /*terminate();*/
}

void terminate() {
  setKernelDataSegment();
  processTable[currentProcess].isActive = 0;
  processTable[currentProcess].stackPointer = 0xff00;
  while(1);
}

void killProcess(char *proc) {
  int pid;
  pid = proc[0] - '0';
  setKernelDataSegment();
  processTable[pid].isActive = 0;
  restoreDataSegment();
}

void quitAll() {
  /* Sets all processes to inactive */
  int i;
  setKernelDataSegment();
  for (i = 0; i < 8; i++) {
    processTable[i].isActive = 0;
  }
  terminate();
}

void clearScreen() {
  char newline[2];
  int i;
  newline[0] = '\n';
  newline[1] = '\0';
  for (i = 0; i < 100; i++) {
    printString(newline);
  }
}

void writeFile(char* name, char* buffer, int numberOfSectors) {
    char map[MAX_BUFFER_SIZE];
    char directory[MAX_BUFFER_SIZE];
    int index;
    int i;
    int sector;
    int fileSize = numberOfSectors;
    
    readSector(map, 1);
    readSector(directory, 2);

    for (index = 0; index < MAX_BUFFER_SIZE; index += MAX_INDEX_SIZE) {

        if (directory[index] == 0x00) {
            break;
        }
    }
    if (index >= MAX_BUFFER_SIZE) {
        return;
    }

    for (i = 0; i < 6; i++) {
    	if (name[i] == '\0') {
    		break;
    	}
        directory[index + i] = name[i];
    }
    for (i = i; i < 6; i++) {
        directory[index + i] = 0x00;
    }

    sector = 3;
    while (fileSize > 0) {
      if (map[sector] == 0x00) {
        fileSize--;
      }
      if (sector >= MAX_BUFFER_SIZE) {
        return;
      }
      sector++;
    }
    sector = 3;
    
    while(numberOfSectors > 0) {
      /*for (sector = 0; sector < MAX_BUFFER_SIZE; sector++) {*/
      if (map[sector] == 0x00) {
        map[sector] = 0xFF;
        numberOfSectors--;
        directory[index + i] = sector + 1;
        i++;
        writeSector(buffer, sector + 1);
        buffer += MAX_BUFFER_SIZE;
      }
      if (sector >= MAX_BUFFER_SIZE) {
        return;
      }
      sector++;
    }
    while (i < MAX_INDEX_SIZE) {
        directory[index + i] = 0x00;
        i++;
    }
    writeSector(map, 1);
    writeSector(directory, 2);
}

void initializeProcessTable() {
  int i;
  int active;
  int stackPointer;
  active = 0;
  stackPointer = 0xff00;
  for (i = 0; i < 8; i++) {
    processTable[i].isActive = active;
    processTable[i].stackPointer = stackPointer;
  }
}

