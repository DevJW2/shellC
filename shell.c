#include "shell.h"

void ** parse_args(char * line) {
  pid_t f = fork();
  
  printf("running parse_args\n");
  if (f == 0) {
     printf("In child\n");
    
     char ** s = (char **)calloc(6, sizeof(line));
    
     int i = 0;
     while(line) {
       s[i] = strsep(&line, "\n");
       i++;
     }

     printf("Executing command...\n");
     (execvp(s[0], s);
  } else {
    wait(NULL);
    printf("Parent destroyed\n");
    exit(0);
  }

  
}



int main() {
  char input[200];
  
  while(1){
    printf("\nEnter Command: ");
    fgets(input, sizeof(input), stdin);
    printf("input value: %s\n", input);

    parse_args(input);
    
  }

  return 0;
  
}
