# wash Shell Extra Credit Project
## Danny Pham
## CSC3350
## Professor: Andy Cameron
## Introduction
The wash shell is a custom command-line shell designed for Unix-like operating 
systems, including Linux-based distributions, macOS (Terminal), and Windows WSL2. 
The "wa" stands for Washington (as in Washington State), and the "sh" stands for 
shell. The shell provides basic functionalities similar to traditional shells like 
bash and dash, allowing users to execute commands, manage directories, and 
manipulate the environment.
## Features
### Built-in Commands
- **exit**: Ends the wash shell session.
 - **Usage**: `exit`
 
- **echo**: Prints the arguments given to the console.
 - **Usage**: `echo [some text]`
 
- **pwd**: Prints the current working directory.
 - **Usage**: `pwd`
 
- **cd**: Changes the current working directory.
 - **Usage**: `cd [dir]`
 - If no arguments are provided, changes to the user's home directory.
 
- **setpath**: Sets the PATH environment variable to the directories specified by 
the user.
 - **Usage**: `setpath <dir> [dir] ... [dir]`
 
- **help**: Lists all built-in commands with short, user-friendly descriptions.
 - **Usage**: `help`
 
### External Commands
- Executes external commands by searching the specified directories in the PATH.
- Handles at least four external commands such as `ls`, `cat`, `date`, and `grep`.
### Redirection
- Supports output redirection using the syntax `command > filename`.
 - Standard output is directed to `<filename>.output` and standard error to 
`<filename>.error`.
### Custom Utility Command - head_new
- Mimics the behavior of the standard `head` command with simplified options.
- **Usage**: `./head_new [-h] [-n N] [file.txt]`
 - Prints the first N lines of the specified file or standard input.
 - Default is 7 lines if `-n` is not used.
 - The `-h` flag prints an argument usage summary.
## How to Compile, Build, and Run
1. **Compile the wash shell:**
 ```bash
 gcc -o wash wash.c
2. **Compile the head_new shell:**
 ```bash
 gcc -o head_new head_new.c
3. **Run the wash shell:**
 ```bash
 ./wash
4. **Run the head_new command:**
 ```bash
 ./head_new -h
 ./head_new 
 ./head_new file.txt
 ./head_new -n 4 file.txt
### Error Handling
The shell provides informative error messages for incorrect inputs, missing files, 
and command execution failures. Errors do not terminate the shell session.
### Sample Input and Output
 ```bash
 wash:3 pwd
 /home/user
 
 wash:3 cd /tmp
 wash:3 pwd
 /tmp
 
 wash:3 echo Hello, World!
 Hello, World!
 wash:3 setpath /bin /usr/bin
 wash:3 help
 Available commands:
 exit - End the shell session
 echo [some text] - Print text to the console
 pwd - Print the current working directory
 cd [dir] - Change the current working directory
 setpath <dir> [dir] ... [dir] - Set the executable path
 help - List built-in commands with descriptions
```
### External Commands
```bash
wash:3 ls
file1.txt file2.txt
wash:3 date
Mon Jun 9 10:21:17 UTC 2024
wash:3 df
Filesystem 1K-blocks Used Available Use% Mounted on
udev 10240 0 10240 0% /dev
tmpfs 201928 8044 193884 5% /run
/dev/sda1 20457644 1962040 17479340 11% /
```
### Redirection
```bash
wash:3 echo Hello > output
wash:3 cat output.output
Hello
wash:3 cat output.error
```
### head_new Command
```bash
wash:3 ./head_new -n 5 file.txt
Line 1
Line 2
Line 3
Line 4
Line 5
wash:3 ./head_new file.txt
Line 1
Line 2
Line 3
Line 4
Line 5
Line 6
Line 7
