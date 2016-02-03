/* Morgan Cook, Josh Green, Philip Ross
   Milestone 3
   Team 1-F
*/

#define PRINT_STRING 0 /* args: char* */
#define READ_STRING 1 /* args: char* */
#define READ_SECTOR 2 /* args: char*, int */
#define READ_FILE 3 /* args: char*, char* */
#define EXECUTE_PROG 4 /* args: char*, int */
#define TERMINATE 5
#define WRITE_SECTOR 6 /* args: char*, int */
#define DELETE_FILE 7 /* args: char* */
#define WRITE_FILE 8 /* args: char* char* int */

#define MAX_COMMAND_SIZE 80
#define NUM_COMMANDS 10
#define MAX_BUFFER_SIZE 13312
#define BUFFER_INC 512

#define BAD_COMMAND "Invalid command\n"

#define TYPE_SIZE 5
#define EXECUTE_SIZE 8
#define DELETE_SIZE 7
#define COPY_SIZE 5

int executeCommand(char *command);
void getCommand(char *command, char *name);
int getArg(char *command, char *arg, int i);
int strEquals(char* str1, char* str2);
void fillType(char *arr);
void fillExecute(char *arr);
void fillDelete(char *arr);
void fillCopy(char *arr);

int main() {
  char *command;
  char shell[7];
  shell[0] = 's';
  shell[1] = 'h';
  shell[2] = 'e';
  shell[3] = 'l';
  shell[4] = 'l';
  shell[5] = '>';
  shell[6] = '\0';
  while(1) {
    interrupt(0x21, PRINT_STRING, shell, 0, 0);
    interrupt(0x21, READ_STRING, command, 0, 0);
    if (!executeCommand(command)) {
      interrupt(0x21, PRINT_STRING, BAD_COMMAND, 0, 0);
    }
  }
}

int executeCommand(char *command) {
  int i = 0;
  int numSectors = 0;
  int bufferLocation = 0;
  char name[MAX_COMMAND_SIZE];
  char arg[MAX_COMMAND_SIZE];
  char secondArg[MAX_COMMAND_SIZE];
  char buffer[MAX_BUFFER_SIZE];
  char type[TYPE_SIZE];
  char execute[EXECUTE_SIZE];
  char delete[DELETE_SIZE];
  char copy[COPY_SIZE];
  fillType(type);
  fillExecute(execute);
  fillDelete(delete);
  fillCopy(copy);

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
    interrupt(0x21, EXECUTE_PROG, arg, 0x2000, 0);
    return 1;
  } else if (strEquals(name, delete)) {
    i = DELETE_SIZE;
    i = getArg(command, arg), i;
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

  while(command[i] != '\0' && command[i] != " ") {
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
