#include "shell.h"

int main(){
  signal(SIGINT, sighandler);
  while(1){
    printf("\n");
    print_current_dir();
    char * input = get_input();
    execute_input(input);
    free(input);
  }
  return 0;
}
