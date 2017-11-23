#include "shell.h"

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

void execute_commands(){
  char input[100]; 
  
  printf("Enter Command: ");
  fgets(input, sizeof(input), stdin);
  printf("input: %s", input);

  //get rid of newline...add null termination
  size_t length = strlen(input);
  if(input[length - 1] == '\n'){
    input[length - 1] = '\0';
  }

  //command doesn't work, if there's a space added at the end.
  
  pid_t f = fork();
  
  if(f == 0){
    printf("\nIn Child...\n");
    
    char ** args = parse_args(input);
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
  while(1){
    execute_commands();
    
  }

  return 0;
  
}
