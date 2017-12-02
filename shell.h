#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <time.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <signal.h> 
#include <fcntl.h>
#include <errno.h>
#include <ctype.h>

void sighandler(int signo);
void print_current_dir();
void exit_shell(char * line);
void cd(char ** line);
char * trim_whitespace(char * cmd);
char ** parse_args(char * line);
char ** parse_commands(char * line);
void redirect(char ** line);
void execute_commands();

