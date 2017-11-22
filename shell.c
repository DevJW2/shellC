#include "shell.h"


char ** parse_args(char * line){
  char ** s1 = (char **)calloc(6, sizeof(line));

  int i = 0;
  while(line){
    s1[i] = strsep(&line, " ");
    i++;
    
  }
  return s1; 
}

int main(){
}
