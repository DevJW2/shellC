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

void redirect(char ** line){

  printf("Execute redirection\n");
 
  int new_file = open(line[2], O_CREAT | O_WRONLY | O_RDONLY, 0644);
  //dup2(new_file, 1);

  char input[100];
            
  printf("Redirection Input: ");
  int count = 0;
  while(1){ 
    fgets(input, sizeof(input), stdin);
    write(new_file, input, sizeof(input));
  }
 
  close(new_file);

  printf("Finish execution\n");
  //check user input
  //make sure there is a command in front
  //make sure there is a redirection symbol
  //make sure there is a file at the end

  //wait for further commands to input into file
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
      int count = 0; 
      while(args[count]){
	count++;
      }

      printf("Argument number: %d\n\n", count);
      if(count == 3){
	if(!strcmp(args[1], ">")){
	  redirect(args);
	}
	else{
	  execvp(args[0], args);
	}
      }
      else{
	execvp(args[0], args);
      }

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
