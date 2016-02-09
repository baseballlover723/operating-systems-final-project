/* Morgan Cook, Josh Green, Philip Ross
   Milestone 3
   Team 1-F
*/

#define PRINT_STRING 0 /* args: char* */
#define READ_STRING 1 /* args: char* */
#define READ_SECTOR 2 /* args: char*, int */
#define READ_FILE 3 /* args: char*, char* */
#define EXECUTE_PROG 4 /* args: char* */
#define TERMINATE 5
#define WRITE_SECTOR 6 /* args: char*, int */
#define DELETE_FILE 7 /* args: char* */
#define WRITE_FILE 8 /* args: char* char* int */
#define KILL_PROCESS 9 /* args: int */
#define QUIT 10
#define CLEAR 11

#define MAX_COMMAND_SIZE 80
#define NUM_COMMANDS 10
#define MAX_BUFFER_SIZE 13312
#define BUFFER_INC 512

#define TYPE_SIZE 5
#define EXECUTE_SIZE 8
#define DELETE_SIZE 7
#define COPY_SIZE 5
#define DIR_SIZE 4
#define CREATE_SIZE 7
#define KILL_SIZE 5
#define QUIT_SIZE 5
#define CLEAR_SIZE 6
#define HELP_SIZE 5

int executeCommand(char *command);
void getCommand(char *command, char *name);
int getArg(char *command, char *arg, int i);
int strEquals(char* str1, char* str2);
void printDirectory();
void createFile(char *filename);
void fillType(char *arr);
void fillExecute(char *arr);
void fillDelete(char *arr);
void fillCopy(char *arr);
void fillDir(char *arr);
void fillCreate(char *arr);
void fillKill(char *arr);
void fillQuit(char *arr);
void fillClear(char *arr);
void fillHelp(char *arr);
char* itoa(int n, char* buffer);
int mod(int a, int b);

int main() {
  char *command;
  char shell[7];
  char bad[60];
  enableInterrupts();
  shell[0] = 's';
  shell[1] = 'h';
  shell[2] = 'e';
  shell[3] = 'l';
  shell[4] = 'l';
  shell[5] = '>';
  shell[6] = '\0';
  bad[0] = 'i';
  bad[1] = 'n';
  bad[2] = 'v';
  bad[3] = 'a';
  bad[4] = 'l';
  bad[5] = 'i';
  bad[6] = 'd';
  bad[7] = ' ';
  bad[8] = 'c';
  bad[9] = 'o';
  bad[10] = 'm';
  bad[11] = 'm';
  bad[12] = 'a';
  bad[13] = 'n';
  bad[14] = 'd';
  bad[15] = ',';
  bad[16] = ' ';
  bad[17] = 't';
  bad[18] = 'y';
  bad[19] = 'p';
  bad[20] = 'e';
  bad[21] = ' ';
  bad[22] = '"';
  bad[23] = 'h';
  bad[24] = 'e';
  bad[25] = 'l';
  bad[26] = 'p';
  bad[27] = '"';
  bad[28] = ' ';
  bad[29] = 't';
  bad[30] = 'o';
  bad[31] = ' ';
  bad[32] = 's';
  bad[33] = 'e';
  bad[34] = 'e';
  bad[35] = ' ';
  bad[36] = 't';
  bad[37] = 'h';
  bad[38] = 'e';
  bad[39] = ' ';
  bad[40] = 'a';
  bad[41] = 'v';
  bad[42] = 'a';
  bad[43] = 'i';
  bad[44] = 'l';
  bad[45] = 'a';
  bad[46] = 'b';
  bad[47] = 'l';
  bad[48] = 'e';
  bad[49] = ' ';
  bad[50] = 'c';
  bad[51] = 'o';
  bad[52] = 'm';
  bad[53] = 'm';
  bad[54] = 'a';
  bad[55] = 'n';
  bad[56] = 'd';
  bad[57] = 's';
  bad[58] = '\n';
  bad[59] = '\0';
  while(1) {
    interrupt(0x21, PRINT_STRING, shell, 0, 0);
    interrupt(0x21, READ_STRING, command, 0, 0);
    if (!executeCommand(command)) {
      interrupt(0x21, PRINT_STRING, bad, 0, 0);
    }
  }
}

int executeCommand(char *command) {
  int i = 0;
  int numSectors = 0;
  int bufferLocation = 0;
  char newline[2];
  char name[MAX_COMMAND_SIZE];
  char arg[MAX_COMMAND_SIZE];
  char secondArg[MAX_COMMAND_SIZE];
  char buffer[MAX_BUFFER_SIZE];
  char type[TYPE_SIZE];
  char execute[EXECUTE_SIZE];
  char delete[DELETE_SIZE];
  char copy[COPY_SIZE];
  char dir[DIR_SIZE];
  char create[CREATE_SIZE];
  char kill[KILL_SIZE];
  char quit[QUIT_SIZE];
  char clear[CLEAR_SIZE];
  char help[HELP_SIZE];
  newline[0] = '\n';
  newline[1] = '\0';
  fillType(type);
  fillExecute(execute);
  fillDelete(delete);
  fillCopy(copy);
  fillDir(dir);
  fillCreate(create);
  fillKill(kill);
  fillQuit(quit);
  fillClear(clear);
  fillHelp(help);

  getCommand(command, name);
  if (strEquals(name, type)) {
    i = TYPE_SIZE;
    i = getArg(command, arg, i);
    interrupt(0x21, READ_FILE, arg, buffer, 0);
    interrupt(0x21, PRINT_STRING, buffer, 0, 0);
    return 1;
  } else if (strEquals(name, execute)) {
    i = EXECUTE_SIZE;
    i = getArg(command, arg, i);
    interrupt(0x21, EXECUTE_PROG, arg, 0, 0);
    return 1;
  } else if (strEquals(name, delete)) {
    i = DELETE_SIZE;
    i = getArg(command, arg, i);
    interrupt(0x21, DELETE_FILE, arg, 0, 0);
    return 1;
  } else if (strEquals(name, copy)) {
    i = COPY_SIZE;
    i = getArg(command, arg, i);
    i = getArg(command, secondArg, i);
    interrupt(0x21, READ_FILE, arg, buffer, 0);

    while(buffer[bufferLocation] != 0x00){
      bufferLocation += BUFFER_INC;
      numSectors++;
    }
    interrupt(0x21, WRITE_FILE, secondArg, buffer, numSectors);
    return 1;
  } else if (strEquals(name, dir)) {
    printDirectory();
    return 1;
  } else if (strEquals(name, create)) {
    i = CREATE_SIZE;
    i = getArg(command, arg, i);
    createFile(arg);
    return 1;
  } else if (strEquals(name, kill)) {
    i = KILL_SIZE;
    i = getArg(command, arg, i);
    interrupt(0x21, KILL_PROCESS, arg, 0, 0);
    return 1;
  } else if (strEquals(name, quit)) {
    interrupt(0x21, QUIT, 0, 0, 0);
    return 1;
  } else if (strEquals(name, clear)) {
    interrupt(0x21, CLEAR, 0, 0, 0);
    return 1;
  } else if (strEquals(name, help)) {
    interrupt(0x21, PRINT_STRING, type, 0, 0);
    interrupt(0x21, PRINT_STRING, newline, 0, 0);
    interrupt(0x21, PRINT_STRING, execute, 0, 0);
    interrupt(0x21, PRINT_STRING, newline, 0, 0);
    interrupt(0x21, PRINT_STRING, delete, 0, 0);
    interrupt(0x21, PRINT_STRING, newline, 0, 0);
    interrupt(0x21, PRINT_STRING, copy, 0, 0);
    interrupt(0x21, PRINT_STRING, newline, 0, 0);
    interrupt(0x21, PRINT_STRING, dir, 0, 0);
    interrupt(0x21, PRINT_STRING, newline, 0, 0);
    interrupt(0x21, PRINT_STRING, kill, 0, 0);
    interrupt(0x21, PRINT_STRING, newline, 0, 0);
    interrupt(0x21, PRINT_STRING, quit, 0, 0);
    interrupt(0x21, PRINT_STRING, newline, 0, 0);
    interrupt(0x21, PRINT_STRING, clear, 0, 0);
    interrupt(0x21, PRINT_STRING, newline, 0, 0);
    return 1;
  }
  return 0;
}

void getCommand(char *command, char *name) {
  /* Given a full command string, returns the name of the command */
  int i = 0;

  while(command[i] != '\0' && command[i] != ' ') {
    name[i] = command[i];
    i++;
  }
  name[i] = '\0';
}

int getArg(char *command, char *arg, int i) {
  /* Given a full command string, returns the location that it stopped */
  int j = 0;

  while(command[i] != '\0' && command[i] != ' ') {
    arg[j] = command[i];
    j++;
    i++;
  }
  i++;
  arg[j] = '\0';
  return i;
}

int strEquals(char* str1, char* str2) {
    int i = 0;
    while(str1[i] != '\0' && str2[i] != '\0') {
        if (str1[i] != str2[i]) {
            return 0;
        }
        i++;
    }
    return str1[i] == '\0' && str2[i] == '\0';
}

void printDirectory() {
  char directory[512];
  char fileName[7];
  char newline[2];
  int index;
  int i;
  int size = 0;
  char sizeStr[4];
  newline[0] = '\n';
  newline[1] = '\0';

  interrupt(0x21, READ_SECTOR, directory, 2, 0);
  
  for (index = 0; index < 512; index += 32) {
    size = 0;
      for (i = 0; i < 6 && directory[index + i] != 0x00; i++) {
      fileName[i] = directory[index + i];
    }
    fileName[i] = '\0';
    while(i < 32 && directory[index + 6 + size] != 0x00) {
        size++;
    }

    interrupt(0x21, PRINT_STRING, fileName, 0, 0);
    if (fileName[0] != '\0') {
        interrupt(0x21, PRINT_STRING, itoa(size, sizeStr), 0, 0);
        interrupt(0x21, PRINT_STRING, newline, 0, 0);
    }  
  }
}

void createFile(char *filename) {
  char file[MAX_BUFFER_SIZE];
  char line[512];
  int numSectors;
  int i;
  int index = 0;

  while(1) {
    interrupt(0x21, READ_STRING, line, 0, 0);
    if (line[0] == 0x00) {
      numSectors = countSectors(file);
      interrupt(0x21, WRITE_FILE, filename, file, numSectors);
      return;
    } else {
      index = cpyStr(line, file, index);
      for (i = 0; i < 512; i++) {
        line[i] = '\0';
      }
    }
  }
}

int cpyStr(char *line, char *file, int index) {
  /*Copies line into file, starting at index. Returns new index to start from*/
  int i = 0;
  while (line[i] != '\0' && i < 512) {
    file[i + index] = line[i];
    i++;
  }
  file[i + index] = '\n';
  return index + i + 1;
}

int countSectors(char *file) {
  int i;
  int count = 0;
  for(i = 0; i < MAX_BUFFER_SIZE; i += 512) {
    if (file[i] == 0x00) {
      return count;
    } else {
      count++;
    }
  }
  return count;
}

void fillType(char *arr) {
  arr[0] = 't';
  arr[1] = 'y';
  arr[2] = 'p';
  arr[3] = 'e';
  arr[4] = '\0';
}

void fillExecute(char *arr) {
  arr[0] = 'e';
  arr[1] = 'x';
  arr[2] = 'e';
  arr[3] = 'c';
  arr[4] = 'u';
  arr[5] = 't';
  arr[6] = 'e';
  arr[7] = '\0';
}

void fillDelete(char *arr) {
  arr[0] = 'd';
  arr[1] = 'e';
  arr[2] = 'l';
  arr[3] = 'e';
  arr[4] = 't';
  arr[5] = 'e';
  arr[6] = '\0';
}

void fillCopy(char *arr) {
  arr[0] = 'c';
  arr[1] = 'o';
  arr[2] = 'p';
  arr[3] = 'y';
  arr[4] = '\0';
}

void fillDir(char *arr) {
  arr[0] = 'd';
  arr[1] = 'i';
  arr[2] = 'r';
  arr[3] = '\0';
}

void fillCreate(char *arr) {
  arr[0] = 'c';
  arr[1] = 'r';
  arr[2] = 'e';
  arr[3] = 'a';
  arr[4] = 't';
  arr[5] = 'e';
  arr[6] = '\0';
}

void fillKill(char *arr) {
  arr[0] = 'k';
  arr[1] = 'i';
  arr[2] = 'l';
  arr[3] = 'l';
  arr[4] = '\0';
}

void fillQuit(char *arr) {
  arr[0] = 'q';
  arr[1] = 'u';
  arr[2] = 'i';
  arr[3] = 't';
  arr[4] = '\0';
}

void fillClear(char *arr) {
  arr[0] = 'c';
  arr[1] = 'l';
  arr[2] = 'e';
  arr[3] = 'a';
  arr[4] = 'r';
  arr[5] = '\0';
}

void fillHelp(char *arr) {
  arr[0] = 'h';
  arr[1] = 'e';
  arr[2] = 'l';
  arr[3] = 'p';
  arr[4] = '\0';
}

char* itoa(int n, char* buffer) {
    buffer[0] = ' ';
    buffer[1] = (mod(n, 100) / 10) + '0';
    buffer[2] = mod(n, 10) + '0';
    buffer[3] = '\0';
    if (buffer[1] == '0') {
        buffer[1] = buffer[2];
        buffer[2] = buffer[3];
    }
    return buffer;
}

int mod(int a, int b) {
    while (a >= b) {
        a = a - b;
    }
    return a;
}
