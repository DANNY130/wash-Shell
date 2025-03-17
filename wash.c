/*
Danny Pham
Course CSC3350
wash Shell Extra Credit Project
Professor Andy Cameron
Date: 06/11/2024
*/
#include <stdio.h> //io tasks with files
#include <stdlib.h> //OS utiltiy generals
#include <string.h> //performs string manipualtions
#include <unistd.h> //POSIX OS API
#include <ctype.h> //use for check whitespace
#include <sys/types.h> //data types used in system calls.
#include <sys/wait.h> //handle processes, wait lib
#include <fcntl.h> //file control operations -> file manipulation
#define MAX_INPUT_SIZE 256 //fixed size input for simplicity
#define MAX_PATHS 10
#define MAX_PATH_LENGTH 256
char *paths[MAX_PATHS];
int path_count = 1; //default path count is 1 which is /bin //global var to keep 
tracks of added PATHS
//Help function for shell usage isntructions
void help() {
 printf("wash - My own shell\n");
 printf("Built-in command lines:\n");
 printf(" exit - Exit the shell process.\n");
 printf(" echo - Print arguments from console.\n");
 printf(" pwd - Print the current directory.\n");
 printf(" cd [dir]- Change the current directory.\n");
 printf(" setpath <dir> [dir] ... [dir] - Set the path for executable programs.\
n");
 printf(" help - List all built-in commands.\n");
}
//Function to handle whitspace. Passing pointer of user input to directly modify 
the string pointer
char* trim_leading_space(char *str) {
 while (isspace((unsigned char)*str)) str++;
 return str;
}
//echo function
void echo(char *input) {
 char *token = strtok(input, " "); // tokenize echo and other args
 token = strtok(NULL, ""); //Get the rest of the line after "echo"
 if (token != NULL) {
 //if there are args
 if(token[0] == '$') {
 //Handle environment variables for PATH/
 char *env_var = getenv(token + 1); //point to the PATH to read by 
skipping $
 if (env_var != NULL) {
 printf("%s\n", env_var); //print all the path in env_var
 } else {
 printf("\n");
 }
 } else {
 printf("%s\n", token); // if not see $ then print args from console
 }
 }
}
void pwd() {
 char curr_dir[MAX_PATH_LENGTH];
 //use getcwd, takes 2 args, buffer path and size
 //returns pointer to the buffer, NULL if error occurred
 if (getcwd(curr_dir, sizeof(curr_dir)) != NULL) {
 // getcwd succeeded, print curr dir
 printf("%s\n", curr_dir);
 } else { //if fail, print the descriptive error message
 perror("pwd - Can't get the current directory");
 }
}
void cd(char *input) { // passing the directory.
// Skip the "cd" part and trim leading spaces
 char *dir = strtok(input + 2, " ");
 if (dir == NULL) { // get the Home directory if user inputs just cd
 dir = getenv("HOME");
 }
 if (chdir(dir) != 0) { //change to dir after cd. chdir returns 0 if it succeeds
 perror("chdir");
 }
}
void setpath(char *input) {
 char *token; //placeholder for each path elements
 int i = 0; //keeps track #of paths 
 token = strtok(input, " "); //tokenize input by splitting it at spaces ->get 
the command 'setpath'
 token = strtok(NULL, " ");//token points to setpath now ponints to first args
 //Free prev allocated memory for paths
// for (int j = 0; j < path_count; j++) {
// free(paths[j]);
// paths[j] = NULL;
// }
// //For testing
// setpath /bin /usr/bin /usr/local/bin
// New PATH: /bin /usr/bin /usr/local/bin
 //loop to extract paths
 while (token != NULL && i < MAX_PATHS) {
 paths[i] = strdup(token);
 if (paths[i] == NULL) { //make sure that paths are correctly freed and 
allocated
 perror("strdup");
 exit(EXIT_FAILURE);
 }
 token = strtok(NULL, " ");
 i++;
 }
 path_count = i;
 printf("New PATH: "); //print out the extracted path for testing
 for (int j = 0; j <path_count; j ++) {
 printf("%s ", paths[j]);
 }
 printf("\n");
}
void redirect_output(char *command, char *filename) {
 //Validate file name: not empty?
 if (filename == NULL || strlen(filename) == 0) {
 fprintf(stderr, "Error: Redirection missing filename \n");
 return;
 }
 //Check for multiple args, filenames
 char *token = strtok(filename, " "); //token vector points to first filename
 while (token != NULL) { //loop until not args or whitespace
 if (strtok(NULL, " ") != NULL) { //found some other args, then return
 fprintf(stderr, "Error: Multiple filenames provided for redirection\
n");
 return;
 }
 token = strtok(NULL, " "); //continues to look for remaining, but mostly 
set token to NULL to break loop
 }
 //Construct filenames for redirection by appending loutput and .error to the 
filename
 char output_file[MAX_PATH_LENGTH];
 char error_file[MAX_PATH_LENGTH];
 snprintf(output_file, sizeof(output_file), "%s.output",filename);
 snprintf(error_file, sizeof(error_file), "%s.error", filename);
 //open files for writing/create them if they don't exist and truncate them if 
they do.
 //if file descriptors are invalid, print an error message and return
 int out_fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
 int err_fd = open(error_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
 if (out_fd == -1 || err_fd == -1) {
 perror("open");
 return;
 }
 //Tokenize the command to pass to exedc
 char *args[MAX_INPUT_SIZE/2 + 1]; //args array of character pointers.
 // in the worst case, each args might be a single char split by space
 int arg_count = 0;
 token = strtok(command, " "); //split the command string to tokens based on 
spaces
 while(token != NULL && arg_count < MAX_INPUT_SIZE/2) {
 //loop until there's no args or input reaches Max amount
 args[arg_count++] = token;
 token = strtok(NULL, " ");
 }
 args[arg_count] = NULL; // set the last element of args to null for execvp to 
terminate
 pid_t pid = fork();
 if (pid == 0) {
 //Redirect stdout and stderr to the opened file descriptors using dup2
 //also close the original file descriptor
 dup2(out_fd, STDOUT_FILENO);
 dup2(err_fd, STDERR_FILENO);
 close(out_fd);
 close(err_fd);
 //Execute the command
 execvp(args[0], args); //exec function that replaces the cur process image 
with a new process image
 //it searches for the command in the directories listed in the PATH env 
variable
 perror("execvp");
 exit(EXIT_FAILURE);//if exec fail, the child terminated
 } else if (pid > 0) {
 //Wait for the child process to complete
 wait(NULL);
 } else {
 perror("fork");
 }
 //Close the file descriptors
 close(out_fd);
 close(err_fd);
}
void external_command(char *input) {
 //tokenize input into command and args
 char *args[MAX_INPUT_SIZE / 2 + 1];
 int arg_count = 0;
 //place args and NULL to last element to args
 char *token = strtok(input, " "); 
 while (token != NULL && arg_count < MAX_INPUT_SIZE / 2) {
 args[arg_count++] = token;
 token = strtok(NULL, " ");
 }
 args[arg_count] = NULL;
 int found = 0;
 //iterate thru each directory in the paths array
 for (int i = 0; i < path_count; i++) {
 //constructs the full path of the command by concatenate dir path w the 
command name
 char command_path[MAX_PATH_LENGTH];
 snprintf(command_path, sizeof(command_path), "%s/%s", paths[i], args[0]);
 //access is used to check if the command exists in the directory and is 
executable
 if (access(command_path, X_OK) == 0) {// -1 if not executable PATHS
 found = 1;
 //fork a child to exec the external commands
 pid_t pid = fork();
 if (pid == 0) {//in the child process, execute the command
 execv(command_path, args);
 //execv returns here when fail
 perror("execv");
 exit(EXIT_FAILURE);
 } else if (pid > 0) {
 // parent process, wait for the child to finish to prevent zombie
 wait(NULL);
 } else {
 perror("fork");
 }
 break;
 }
 }
 if (!found) {
 fprintf(stderr, "%s: command not found\n", args[0]); // print error message
if not found.
 }
}
//Test example for external cmd function: ls -l, date, cat /etc/passwd
int main(int argc, char *argv[]) {
 //Initialize paths with default /bin
 paths[0] = "/bin";
 //implement help()
 if (argc > 1 && strcmp(argv[1],"help") == 0) {
 help();
 return 0;
 }
 char input[MAX_INPUT_SIZE];
 while(1) {
 printf("wash:3 ");
 if(!fgets(input, sizeof(input), stdin)) {
 break;
 } //get user input
 //Remove trailing newline
 input[strcspn(input, "\n")] = 0;
 char *trim_input = trim_leading_space(input);
 //check for redirection
 char *redirect_pos = strstr(trim_input, ">");
 if (redirect_pos != NULL) { //if there are args
 //redirect output
 *redirect_pos = '\0'; // split cmd and filename
 char *filename = trim_leading_space(redirect_pos + 1);
 redirect_output(trim_input, filename);
 } else {
 if (strcmp(trim_input, "exit") == 0) {
 break;
 } else if (strncmp(trim_input, "echo ", 5) == 0) {
 echo(trim_input);
 } else if (strcmp(trim_input, "pwd") == 0) {
 pwd();
 } else if (strncmp(trim_input, "cd", 2) ==0) {
 cd(trim_input);
 }else if (strcmp(trim_input, "-h") == 0) {
 help();
 } else if (strncmp(trim_input, "setpath", 7) == 0) {
 setpath(trim_input);
 } else if (strncmp(trim_input, "#", 1) == 0) {
 continue;
 }
 else {
 external_command(trim_input);
 }
 }
 }
 if (path_count > 1) {
 for (int i = 0; i <path_count; i++) {
 free(paths[i]);
 printf("Freed %s\n", paths[i]);
 }
 }
 return 0;
}