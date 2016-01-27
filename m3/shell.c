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
#define BAD_COMMAND "Invalid command"

#define TYPE_COMMAND "type"

int executeCommand(char *command);

int main() {
  char *command;
  while(1) {
    interrupt(0x21, PRINT_STRING, SHELL_OUT, 0, 0);
    interrupt(0x21, READ_STRING, command, 0, 0);
    if (!executeCommand(command)) {
      interrupt(0x21, PRINT_STRING, BAD_COMMAND, 0, 0);
      interrupt(0x21, TERMINATE, 0, 0, 0);
    }
  }
}

int executeCommand(char *command) {
  return 0;
}
