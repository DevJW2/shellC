#include "shell.h"

// Signal Handler
void sighandler(int signo) {
  if (signo == SIGINT) {
    printf("\n\n\nShell exiting due to SIGINT...\n\n");
    exit(0);
  }
}

// Print Current Directory
void print_current_dir() {
  char dir[512];
  getcwd(dir, sizeof(dir));
  printf("%s $ ", dir);
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
    printf("Directory Changed To...\n");
    print_current_dir();
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
  print_current_dir();
  fgets(input, sizeof(input), stdin);
  printf("input: %s", input);
  
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
    
    char ** args = parse_args(cmd);
   
    // Change Directory Check
    cd(args);
    
    pid_t f = fork();
    
    if (f == 0) {
     
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
