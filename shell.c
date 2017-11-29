#include "shell.h"

// Signal Handler
void sighandler(int signo) {
  if (signo == SIGINT) {
    printf("\n\n\nShell exiting due to SIGINT...\n\n");
    exit(0);
  }
}

// Exit Shell
void exit_shell(char * line) {
  if (strcmp(line, "exit") == 0) {
    printf("\n\nExiting Shell...\n\n");
    exit(0); 
  }
}

// Trim whitespace before and after input
char * trim_whitespace(char * cmd) {
  char * end = cmd + strlen(cmd) - 1;
  // Trim front - Not null and is space
  while (*cmd && isspace(*cmd)) {
    cmd++;
  }
  // Trim end - Set end spaces to null
  while (isspace(*end) && end > cmd) {
    *end-- = '\0';
  }
  // Only trims end
  /*
  int len = strlen(cmd) - 1;
  while (len >= 0) {
    if (isspace(cmd[len]) && !isspace(cmd[len-1])) {
      cmd[len] = '\0';
      return;
    }
    len--;
  }
  */
  return cmd;
}

char ** parse_args(char * line) {

  char ** s = (char **)calloc(6, sizeof(line));

  int i = 0;
  while(line) {
    s[i] = strsep(&line, " ");
    printf("s[%d]: %s\n", i,s[i]);
    i++;
  }

  printf("\n");
  return s;
  
}

char ** parse_commands(char * line) {
  char ** s = (char **)calloc(6, sizeof(line));

  int i = 0;
  while(line){
    s[i] = strsep(&line, ";");
    printf("s[%d]: %s\n", i,s[i]);
    i++;
  }

  printf("\n");
  return s;

}

void execute_commands() {
  
  char input[100];
  printf("_______________________________________________________________\n");  
  printf("Enter Command: ");
  fgets(input, sizeof(input), stdin);
  printf("input: %s", input);

  // Exit
  exit_shell(input);

  // Change Directory
  //cd(input);
  
  //get rid of newline...add null termination
  size_t length = strlen(input); 
  
  if(input[length - 1] == '\n'){
    input[length - 1] = '\0';
  }

  //command doesn't work, if there's a space added at the end. -- FIXED
  
  char ** commands = parse_commands(input);
  int value = 0;
    
  while(commands[value]){
    pid_t f = fork();

    // Trim whitespace
    char * cmd = trim_whitespace(commands[value]);
      
    if (f == 0) {
      char ** args = parse_args(cmd);
      execvp(args[0], args);

      exit(0);
    }
    else {
      printf("\nIn Parent...\n");
      wait(0);
      printf("\n--Child terminated--\n\n");
      printf("_______________________________________________________________\n");
      value++;
    }
  }
}



int main() {
  signal(SIGINT, sighandler);
   
  while(1){
    execute_commands();
  }

  return 0;
  
}
