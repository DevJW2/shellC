#include "shell.h"


/* void print_currentdir()
Input: None
Returns: None
Description: Prints current working directory
*/
void print_current_dir() {
  char dir[512];
  getcwd(dir, sizeof(dir));
  printf("%s$ ", dir);
}

/* void exit_shell(char * line)
Input: char * line
Returns: None
Description: Checks if input is equal to "exit" and exits if true
*/
void exit_shell(char * line) {
  if (!strcmp(line, "exit")) {
    //printf("\n\nExiting Shell...\n\n");
    exit(0); 
  }
}

/* void cd(char ** line)
Input: char ** line
Returns: None
Description: Uses chdir() to change directory
*/
void cd(char ** line) {
  if (!strcmp(line[0], "cd") && line[1]) {
    chdir(line[1]);
    //printf("Directory Changed To...\n");
    //print_current_dir();
  }
}

/* char * trim_whitespace(char * cmd)
Input: char * cmd
Returns: Input with no spaces before and after
Description: Trims whitespace off the front and end of the cmd
*/
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

/* char ** parse_args(char * line, char * chr)
Input: char * line, char * chr
Returns: char ** / array of strings
Description: Splits line on specified char
*/
char ** parse_args(char * line, char * chr) {

  char ** s = (char **)calloc(6, sizeof(line));

  int i = 0;
  while(line) {
    s[i] = strsep(&line, chr);
    s[i] = trim_whitespace(s[i]);
    printf("s[%d]: %s\n", i,s[i]);
    i++;
  }

  //printf("\n");
  return s;
  
}

/* char ** parse_commands(char * line )
Input: char * line
Returns: char ** / array of strings
Description: Splits line on semicolon
*/
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

int size(char ** args){
  int i = 0;
  while(* args++){
    i++;
  }
  return i;
}

/* void redirect(char * line, char direction)
Input: char * line, char direction
Returns: None
Description: Parses on > or < depending on the direction.
If >, then the function creates a new file and uses dup2 to
redirect stdin into the new file.
If <, then the function opens an existing file and uses dup2
to redirect stdin into the open file.
*/
void redirect(char * line, char direction) {
  int new_file, stdin, old_file;
  char ** args;
  // >
  if (direction == '>') {
    // Parse on >
    args = parse_args(line, ">");
    //printf("%s", line);
    // Create new file as write only
    if(args[1]){
      new_file = open(trim_whitespace(args[1]), O_CREAT | O_WRONLY, 0644);
      stdin = dup(STDOUT_FILENO);
      old_file = dup2(new_file, STDOUT_FILENO);
      
    }
    close(new_file);
        
  } else if (direction == '<') {
    // Parse on <
    args = parse_args(line, "<");
    

    // Open file as read only and check if successful
    
    if (args[1]) {
      new_file = open(trim_whitespace(args[1]), O_RDONLY, 0644);
      stdin = dup(STDIN_FILENO);
      old_file = dup2(new_file, STDIN_FILENO);
      
    }
    close(new_file);
    
  }

  
}

/* void piper(char * line)
Input: char * line
Returns: None
Description: Redirects stdin to stdout of other command
*/
void piper(char * line) {
  int new_file, stdin, old_file;
  char ** command;
  char ** args = parse_args(line, "|");
  FILE *fp = popen(args[0], "r");
  stdin = dup(STDIN_FILENO);
  old_file = dup2(fileno(fp), STDIN_FILENO);
  command = parse_args(trim_whitespace(args[1]), " ");
  execute_commands(command);
  dup2(stdin, STDIN_FILENO);
  pclose(fp);
  free(command);
  free(args);
  return;
}

/* void execute_commands(char ** commands)
Input: char ** commands
Returns: None
Description: Check for exit or cd, and execvp other commands in a child process
*/
void execute_commands(char ** commands) {
  
  // Exit Check
  exit_shell(commands[0]);
   
  // Change Directory Check
  cd(commands);
    
  pid_t f = fork();
    
  if (f == 0) { // Child
    execvp(commands[0], commands);
    exit(0);
  } else { // Parent
    int status;
    wait(&status);
  }
}

/* void execute_input(char * input)
Input: char * input
Returns: None
Description: Check for redirection or piping, execute_commands otherwise
*/
void execute_input(char * input) {
  char ** commands = parse_args(input, ";");
  int i = 0;
  while (commands[i]) {
    // Trim spaces
    char * cmd = trim_whitespace(commands[i]);
     // Check If Redirecting
    if (strchr(cmd, '>')) {
      printf("Redirect >\n");
      redirect(cmd, '>');
    } else if (strchr(cmd, '<')) {
      printf("Redirect <\n");
      redirect(cmd, '<');
    } else if (strchr(cmd, '|')) { // Check If Piping
      printf("Pipe\n");
      piper(cmd);
    } else {
      printf("Execute Command\n");
      char ** args = parse_args(commands[i], " ");
      execute_commands(args);
      free(args);
      }
    i++;
  }
  free(commands);
}

/* char * get_input()
Input: None
Returns: char * / user's input
Description: Uses fgets to get input and removes newline
*/
char * get_input() {
  char * input = malloc(256);
  fgets(input, 256, stdin);
  
  size_t length = strlen(input); 

  if (input[length - 1] == '\n') {
    input[length - 1] = '\0';
  }

  return input;
}

/* void sighandler(int signo)
Input: int signo
Description: Prints current working directory and flushes stdout buffer
*/
void sighandler(int signo) {
  if (signo == SIGINT) {
    printf("\n");
    print_current_dir();
    fflush(stdout);
  }
}

