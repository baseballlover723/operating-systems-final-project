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

#define SHELL_OUT "SHELL>"
#define BAD_COMMAND "Invalid command\n"

#define TYPE_COMMAND "type"

int executeCommand(char *command);
int strEquals(char* str1, char* str2);

char *validCommands[10];

int main() {
  char *command;
  validCommands[0] = "hello\0";
  while(1) {
    interrupt(0x21, PRINT_STRING, SHELL_OUT, 0, 0);
    interrupt(0x21, READ_STRING, command, 0, 0);
    if (!executeCommand(command)) {
      interrupt(0x21, PRINT_STRING, BAD_COMMAND, 0, 0);
     /* interrupt(0x21, TERMINATE, 0, 0, 0);
    */}
  }
}

int executeCommand(char *command) {
    int i = 0;
    char blah[10];
    while(1) {
        if (!validCommands[i]) {
            return 0;
        }
        if (strEquals(command, validCommands[i])) {
            interrupt(0x21, PRINT_STRING, "valid command\n", 0, 0);
            return 1;
        }
        i++;
    }
    return 0;
}

int strEquals(char* str1, char* str2) {
    int i = 0;
    while(str1[i] != '\0' && str1[i] != ' '  && str2[i] != '\0') {
        if (str1[i] != str2[i]) {
            return 0;
        }
        i++;
    }
    return (str1[i] == '\0' || str1[i] == ' ') && str2[i] == '\0';
}
