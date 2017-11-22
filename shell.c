#include "shell.h"



char ** parse_args(char * line){
  pid_t f = fork();
  
  printf("running parse_args\n");
  if(f == 0){
     printf("In child\n");
    
     char ** s1 = (char **)calloc(6, sizeof(line));
    
     int i = 0;
     while(line){
       s1[i] = strsep(&line, " ");
       i++;
    
     }

     execvp(s1[0], s1);
    
  }
  else if(f > 0){
    printf("Parent terminated");
    wait(0);
    exit(0);
  }
  else{
    printf("Fork failed\n");
  }
  
}



int main(){
  char input[200];
  
  while(1){
    printf("Enter Command: ");
    fgets(input, sizeof(input), stdin);
    printf("input value: %s\n", input);

    parse_args(input);
    
  }

  return 0;
  
}
