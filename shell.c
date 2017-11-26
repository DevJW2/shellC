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
}

// Count numbers of tokens in line
int count_char(char c, char * line) {
  int ctr = 0;
  // While line is not null
  while (*line) {
    // If character found
    if (c == *line) {
      ctr++;
    }
    line++;
  }
  return ctr;
}

void trim_whitespace(char * line) {
  // While line is not null
  int len = strlen(line) - 1;
  while (len >= 0) {
    if (isspace(line[len]) && !isspace(line[len-1])) {
      line[len] = '\0';
      return;
    }
    len--;
  }
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

  // Token counter test
  printf("TOKEN COUNT TEST\n");
  char* s = "i have; a;; lot; of; semico;lons;";
  printf("Char '%s' has %d semicolons\n", s, count_char(';', s));
    
  printf("Enter Command: ");
  fgets(input, sizeof(input), stdin);
  // Trim whitespace
  trim_whitespace(input);
  printf("input: %s", input);
  
  //get rid of newline...add null termination
  size_t length = strlen(input); 
  
  if(input[length - 1] == '\n'){
    input[length - 1] = '\0';
  }

  //command doesn't work, if there's a space added at the end. -- FIXED
  
  pid_t f = fork();
  
  if(f == 0){
    printf("\nIn Child...\n");

    
    char ** args = parse_args(input);

    //Loop through an array of commands
    
    execvp(args[0], args);
    
    printf("\nExecution Done!\n");
    exit(0);
  }
  else{
    printf("\nIn Parent...\n");
    wait(0);
    printf("\n--Child terminated--\n\n");

  }
}



int main() {
  signal(SIGINT, sighandler);
   
  while(1){
    execute_commands();
  }

  return 0;
  
}
