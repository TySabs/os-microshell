/*********************************************************************
 * Name: Tyler Saballus
 * Date: 9/29/2018
 * Operating Systems 480 - Hutchins - Boley/Crumpton
 * Assignment 3 - Microshell Simulation
 ***********************************************************************/

#include <iostream>
#include <unistd.h>
#include <string.h>
#include <list>
#include <algorithm>
#include <stddef.h>
#include <sys/types.h>
#include <sys/wait.h>

const int MAX_INPUT_LENGTH = 1024;
const int MAX_ARG_LENGTH = 64;
using std::cerr;
using std::endl;
using std::list;
using std::copy;
using std::string;


/***********************************************************************
 * printShellPrompt function
 *
 * Arguments(0): None
 *
 * Returns: void
 *
 * Use: Prints the shell prompt to standard output for the user
 ***********************************************************************/
void printShellPrompt() {
  char shellPrompt[] = "480shell> ";
  write(1, shellPrompt, strlen(shellPrompt));
}

/***********************************************************************
 * checkForQuit function
 *
 * Arguments(1):
 *  1. char *input - a command typed in by the user
 *
 * Returns: a boolean - true if user type "q" or "quit", false otherwise
 *
 * Use: Decides if a user entered "q" or "quit"
 ***********************************************************************/
bool checkForQuit(char *input) {
  int quitFlag1 = 1,
      quitFlag2 = 1;
  // Check to make sure user did not type quit or q
  quitFlag1 = strncmp(input, "quit", 4);
  quitFlag2 = strncmp(input, "q", 1);

  if (quitFlag1 == 0 || quitFlag2 == 0) {
    cerr << endl << "Quitting the program..." << endl << endl;
    return false;
  } else {
    return true;
  }
}

/***********************************************************************
 * executeCommand function
 *
 * Arguments(1):
 *  1. char *cmdString - a command to be executed
 *
 * Returns: void
 *
 * Use: Takes a string, tokenizes it, and executes it as a command
 ***********************************************************************/
void executeCommand(char *cmdString) {

  char *args[MAX_ARG_LENGTH],
        **argPtr = args,
        *token = NULL;;

  // Tokenize command string
  token = strtok(cmdString, " ");
  while (token != NULL) {
    *argPtr++ = token;
    token = strtok(NULL, " ");
  }

  // Set next pointer in args to NULL
  *argPtr = NULL;

  // Execute the command
  execvp(args[0], args);
  cerr << "Error trying to execute the command: " << args[0] << endl;
} // end executeCommand function

/***********************************************************************
 *
 *  main program
 *
 ***********************************************************************/
int main(int argc, char *argv[]) {
  char input[MAX_INPUT_LENGTH] = {};
  int readSize, status;
  bool isLooping = true;

  pid_t pid1, pid2;

  while (isLooping) {
    // Print shell prompt and get user input
    printShellPrompt();
    readSize = read(0, input, MAX_INPUT_LENGTH);

    // Attach a null terminator onto the end of the input string
    input[readSize - 1] = '\0';

    // Check to see if q or quit was entered, if so, end the loop
    isLooping = checkForQuit(input);
    if (isLooping == false) break;

    // Fork into Parent/Child
    if ((pid1 = fork()) < 0) {
      cerr << "Fork Error!" << endl;
    }

    // In child process
    if (pid1 == 0) {
      const char *inputCopy = strdup(input),
                  *pipeChars = " || ";

      // Make a copy of input before manipulating it
      string data(inputCopy);

      size_t foundIndex = -1;

      // Find pipe chars
      foundIndex = data.find(pipeChars);

      // Pipe block
      if (foundIndex != std::string::npos) {
        string leftString, rightString;

        // Build left and right commands as c++ strings
        leftString = data.substr(0, foundIndex);
        rightString = data.substr(foundIndex + 4);

        char leftCmd[leftString.size()],
              rightCmd[rightString.size()];

        // Convert commands to c strings, commands will be ready to be executed
        strcpy(leftCmd, leftString.c_str());
        strcpy(rightCmd, rightString.c_str());

        int pipes[2];

        // Open pipe
        if (pipe(pipes) == -1) {
          cerr << "Error! Pipe Failed." << endl;
          exit(-5);
        }

        // Fork into Grandchild/Child
        if ((pid2 = fork()) < 0) {
          cerr << "Fork Error!" << endl;
        }

        // In grandchild process -- left side
        if (pid2 == 0) {
          // Close unused read end of pipe and standard output
          close(pipes[0]);
          close(1);

          // Dup write end of pipe to standard output
          dup(pipes[1]);

          executeCommand(leftCmd);

          // Close write end of pipe
          close(pipes[1]);

        // In child process -- right side
        } else {
          // Close unused write end of pipe and standard input
          close(pipes[1]);
          close(0);

          // Dup read end of pipe to standard input
          dup(pipes[0]);

          // Wait for grandchild (left command) to finish
          wait(NULL);

          executeCommand(rightCmd);

          // Close read end of pipe
          close(pipes[0]);
        } // End pipe fork

      // Non-pipe block
      } else {
        executeCommand(input);
      } // End child/grandchild if block

      exit(127);

    // In parent process
    } else {
      // Wait for the child
      if ((pid1 = waitpid(pid1, &status, 0)) < 0) {
        cerr << "waitpid error!" << endl;
      }
    } // End parent/child if block

  } // end while loop

  return 0;
} // end main function
