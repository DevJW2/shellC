#include "shell.h"


// Print Current Directory
void print_current_dir() {
  char dir[512];
  getcwd(dir, sizeof(dir));
  printf("%s$ ", dir);
}

// Exit Shell
void exit_shell(char * line) {
  if (!strcmp(line, "exit")) {
    printf("\n\nExiting Shell...\n\n");
    exit(0); 
  }
}

// Change Directory
void cd(char ** line) {
  if (!strcmp(line[0], "cd") && line[1]) {
    chdir(line[1]);
    //printf("Directory Changed To...\n");
    //print_current_dir();
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
  return cmd;
}

char ** parse_args(char * line, char * chr) {

  char ** s = (char **)calloc(6, sizeof(line));

  int i = 0;
  while(line) {
    s[i] = strsep(&line, chr);
    //printf("s[%d]: %s\n", i,s[i]);
    i++;
  }

  //printf("\n");
  return s;
  
}

char ** parse_commands(char * line) {
  char ** s = (char **)calloc(6, sizeof(line));

  int i = 0;
  while(line){
    s[i] = strsep(&line, ";");
    //printf("s[%d]: %s\n", i,s[i]);
    i++;
  }

  printf("\n");
  return s;

}

void redirect(char * line, char direction) {

  printf("Execute redirection %c\n", direction);
  int new_file, copy_file, old_file;
  char ** args;
  // >
  if (direction == '>') {
    // Parse on >
    args = parse_args(line, ">");
    // Create new file and copy
    new_file = open(trim_whitespace(args[1]), O_CREAT | O_WRONLY, 0644);
    copy_file = dup(STDOUT_FILENO);
    old_file = dup2(new_file, STDOUT_FILENO);
    
  }
}


void execute_commands() {
  
  char input[100];
  print_current_dir();
  fgets(input, sizeof(input), stdin);
  //printf("input: %s", input);
  
  //get rid of newline...add null termination
  size_t length = strlen(input); 
  
  if(input[length - 1] == '\n'){
    input[length - 1] = '\0';
  }
  
  char ** commands = parse_commands(input);
  int value = 0;
    
  while(commands[value]){

    // Trim whitespace
    char * cmd = trim_whitespace(commands[value]);

    // Exit Check
    exit_shell(cmd);

    // Check If Redirecting
    if (strchr(cmd, '>')) {
      redirect(cmd, '>');
    } else if (strchr(cmd, '<')) {
      redirect(cmd, '<');
    }

    // Check If Piping
    if (strchr(cmd, '|')) {
      printf("PIPIN");
    }
		      
    char ** args = parse_args(cmd, " ");
   
    // Change Directory Check
    cd(args);
    
    pid_t f = fork();
    
    if (f == 0) {

      execvp(args[0], args);
      exit(0);
      
    } else {
      int status;
      wait(&status);
      value++;
    }
  }
}

// Signal Handler
void sighandler(int signo) {
  if (signo == SIGINT) {
    printf("\n");
    print_current_dir();
    fflush(stdout);
  }
}


int main() {
  signal(SIGINT, sighandler);
  while(1){
    printf("\n");
    execute_commands();
  }

  return 0;
  
}
