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
    //printf("\n\nExiting Shell...\n\n");
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

// Parse arguments on specific char
char ** parse_args(char * line, char * chr) {

  char ** s = (char **)calloc(6, sizeof(line));

  int i = 0;
  while(line) {
    s[i] = strsep(&line, chr);
    s[i] = trim_whitespace(s[i]);
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
  int new_file, stdin, old_file;
  char ** args;
  // >
  if (direction == '>') {
    // Parse on >
    args = parse_args(line, ">");

    // Create new file as write only
    new_file = open(trim_whitespace(args[1]), O_CREAT | O_WRONLY, 0644);
    stdin = dup(STDOUT_FILENO);
    old_file = dup2(new_file, stdin);
    
  } else if (direction == '<') {
    // Parse on <
    args = parse_args(line, "<");

    // Open file as read only and check if successful
    new_file = open(trim_whitespace(args[1]), O_RDONLY, 0644);
    if (new_file != -1) {
      stdin = dup(STDOUT_FILENO);
      old_file = dup2(new_file, stdin);
    }
  }
  close(new_file);
}

// Pipes
void piper(char * line) {
  int new_file, stdin, old_file;
  char ** command;
  char ** args = parse_args(line, "|");
  FILE *fp = popen(args[0], "r");
  stdin = dup(STDIN_FILENO);
  old_file = dup2(fileno(fp), STDIN_FILENO);
  command = parse_args(trim_whitespace(args[1]), " ");
  execute_commands(command);
  dup2(stdin, stdin);
  pclose(fp);
  free(command);
  free(args);
}

// Execute specified command
void execute_commands(char ** commands) {
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
      piper(cmd);
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

// Get Input
char * get_input() {
  char * input = malloc(256);
  fgets(input, 256, stdin);
  
  size_t length = strlen(input); 

  if (input[length - 1] == '\n') {
    input[length - 1] = '\0';
  }

  return input;
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
    print_current_dir();
    char * input = get_input();
    char ** commands = parse_commands(input);
    execute_commands(commands);
    free(input);
  }

  return 0;
  
}
