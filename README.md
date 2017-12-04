# shellC
by Jeffrey Weng, Marcus Ng

### Features:
* Fork and execution of commands
* Command parsing, commands are separated by semicolons
* Simple redirection using > and <
* Simple piping using |
* Change directory
* Exit shell

### Attempted to Implement:

### Bugs:
- If you tap C-d in the shell, the shell prompt keeps printing
- Shell prompt prints a lot during stdin redirection

### Files & Function Headers:

#### shell.c
* Contains all functions that implement the features above

```C
/* void print_currentdir()
Input: None
Returns: None
Description: Prints current working directory
*/

/* void exit_shell(char * line)
Input: char * line
Returns: None
Description: Checks if input is equal to "exit" and exits if true
*/

/* void cd(char ** line)
Input: char ** line
Returns: None
Description: Uses chdir() to change directory
*/

/* char * trim_whitespace(char * cmd)
Input: char * cmd
Returns: Input with no spaces before and after
Description: Trims whitespace off the front and end of the cmd
*/

/* char ** parse_args(char * line, char * chr)
Input: char * line, char * chr
Returns: char ** / array of strings
Description: Splits line on specified char
*/

/* char ** parse_commands(char * line )
Input: char * line
Returns: char ** / array of strings
Description: Splits line on semicolon
*/

/* void redirect(char * line, char direction)
Input: char * line, char direction
Returns: None
Description: Parses on > or < depending on the direction.
If >, then the function creates a new file and uses dup2 to
redirect stdin into the new file.
If <, then the function opens an existing file and uses dup2
to redirect stdin into the open file.
*/

/* void piper(char * line)
Input: char * line
Returns: None
Description: Redirects stdin to stdout of other command
*/

/* void execute_commands(char ** commands)
Input: char ** commands
Returns: None
Description: Check for exit or cd, and execvp other commands in a child process
*/

/* void execute_input(char * input)
Input: char * input
Returns: None
Description: Check for redirection or piping, execute_commands otherwise
*/

/* char * get_input()
Input: None
Returns: char * / user's input
Description: Uses fgets to get input and removes newline
*/

/* void sighandler(int signo)
Input: int signo
Description: Prints current working directory and flushes stdout buffer
*/

```

#### main.c
* Main Execution of program

```C
/* int main()
Input: None
Returns: int
Description: Shell execution
*/
```


