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

#define MAX_COMMAND_SIZE 80
#define NUM_COMMANDS 10
#define MAX_BUFFER_SIZE 13312

#define SHELL_OUT "SHELL>"
#define BAD_COMMAND "Invalid command\n"

#define TYPE_SIZE 5
#define EXECUTE_SIZE 8


int executeCommand(char *command);
void getCommand(char *command, char *name);
void getArg(char *command, char *arg);
int strEquals(char* str1, char* str2);
void fillType(char *arr);
void fillExecute(char *arr);

int main() {
  char *command;
  while(1) {
    interrupt(0x21, PRINT_STRING, SHELL_OUT, 0, 0);
    interrupt(0x21, READ_STRING, command, 0, 0);
    if (!executeCommand(command)) {
      interrupt(0x21, PRINT_STRING, BAD_COMMAND, 0, 0);
    }
  }
}

int executeCommand(char *command) {
  int i = 0;
  char name[MAX_COMMAND_SIZE];
  char arg[MAX_COMMAND_SIZE];
  char buffer[MAX_BUFFER_SIZE];
  char type[TYPE_SIZE];
  char execute[EXECUTE_SIZE];
  fillType(type);
  fillExecute(execute);

  getCommand(command, name);
  if (strEquals(name, type)) {
    getArg(command, arg);
    interrupt(0x21, READ_FILE, arg, buffer, 0, 0);
    interrupt(0x21, PRINT_STRING, buffer, 0, 0);
    return 1;
  } else if (strEquals(name, execute)) {
    getArg(command, arg);
    interrupt(0x21, EXECUTE_PROG, arg, 0x2000, 0);
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

void getArg(char *command, char *arg) {
  /* Given a full command string, returns the argument given or "" */
  int i = 0;
  int j = 0;
  int flag = 0;

  while(command[i] != '\0') {
    if (flag) {
      arg[j] = command[i];
      j++;
    }
    if (command[i] == ' ') {
      flag = 1;
    }
    i++;
  }
  arg[j] = '\0';
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
