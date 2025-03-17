/*
Danny Pham
Course CSC3350
wash Shell Extra Credit Project
Professor Andy Cameron
Date: 06/11/2024
*/
#include <stdio.h> // Standard input/output header
#include <stdlib.h> // Standard library for memory allocation, process control, 
etc.
#include <string.h> // String handling functions
// Preprocessor definitions for default values and limits
#define DEFAULT_LINES 7
#define MAX_LINE_LENGTH 1024
#define MAX_LINES 1000
// Function to print usage information for the program
void print_usage() {
 printf("Usage: ./head_new [-h] [-n N] [file.txt]\n");
 printf("Options:\n");
 printf(" -h Print this help message.\n");
 printf(" -n N Print the first N lines (default is 7).\n");
 printf(" file.txt The file to read from. If not specified, reads from 
standard input.\n");
}
// Function to print the specified number of lines
void print_lines(char *lines[], int num_lines, int total_lines) {
 for (int i = 0; i < num_lines && i < total_lines; i++) {
 printf("%s", lines[i]);
 free(lines[i]); // Free the allocated memory for each line
 }
}
// Main function where the program execution begins
int main(int argc, char *argv[]) {
 int num_lines = DEFAULT_LINES; // Number of lines to print, default is 7
 char *filename = NULL; // Pointer to the filename string
 int i; // Loop counter
 // Parse command-line arguments
 for (i = 1; i < argc; i++) {
 if (strcmp(argv[i], "-h") == 0) {
 print_usage(); // Print help if -h is passed
 return 0;
 } else if (strcmp(argv[i], "-n") == 0) {
 if (i + 1 < argc) { // Check if the next argument exists
 num_lines = atoi(argv[++i]); // Convert string to integer for line 
count
 } else {
 fprintf(stderr, "Error: -n option requires an argument.\n"); // 
Error if no argument for -n
 return 1;
 }
 } else {
 filename = argv[i]; // Assume any other argument is the filename
 }
 }
 FILE *file; // File pointer
 int from_stdin = 0; // Flag to check if input is from stdin
 // Open file or set to read from stdin
 if (filename) {
 file = fopen(filename, "r"); // Open file for reading
 if (file == NULL) {
 perror("Error opening file"); // Print error if file can't be opened
 return 1;
 }
 } else {
 file = stdin; // Set file to stdin if no filename is provided
 from_stdin = 1; // Set flag for stdin
 }
 char *lines[MAX_LINES]; // Array to store lines read from file
 int count = 0; // Counter for number of lines read
 char line[MAX_LINE_LENGTH]; // Buffer to store each line read
 // Prompt user for input if reading from stdin
 if (from_stdin) {
 printf("Enter lines of text. Press CTRL+D to end input and display the 
first %d lines:\n", num_lines);
 }
 // Read lines from the file or stdin
 while (fgets(line, sizeof(line), file) != NULL && count < MAX_LINES) {
 lines[count] = strdup(line); // Duplicate line and store in array
 count++; // Increment line counter
 }
 // Print the lines and free memory if input was from stdin
 if (from_stdin) {
 printf("Your input was: \n");
 print_lines(lines, num_lines, count);
 } else {
 print_lines(lines, num_lines, count); // Print lines from file
 fclose(file); // Close the file
 }
 return 0; // Exit program
}