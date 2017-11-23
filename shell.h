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


char ** parse_args(char * line);
char ** parse_commands(char * line);
void execute_commands();

