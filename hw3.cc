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

const int MAX_INPUT_LENGTH = 1024;
using std::cerr;
using std::endl;
using std::list;
using std::copy;
using std::string;

void printShellPrompt() {
  char shellPrompt[] = "480shell> ";
  write(1, shellPrompt, strlen(shellPrompt));
}

bool checkForQuit(char *input) {
  int quitFlag1 = 1,
      quitFlag2 = 1;
  // Check to make sure user did not type quit
  quitFlag1 = strncmp(input, "quit", 4);
  quitFlag2 = strncmp(input, "q", 1);

  if (quitFlag1 == 0 || quitFlag2 == 0) {
    cerr << "Quitting the program..." << endl;
    return false;
  } else {
    return true;
  }
}

void executeCommand(char *cmdString) {
  char *argToken, *cmd1;
  list<char*> argList;

  argToken = strtok(cmdString, " ");
  cmd1 = argToken;

  argList.push_back(argToken);

  argToken = strtok(NULL, " ");

  while (argToken != NULL) {
    argList.push_back(argToken);
    argToken = strtok(NULL, " ");
  }

  int argSize = argList.size();

  if (argSize == 1) argSize++;

  char *args1[argSize];

  if (argList.size() == 1) {
    args1[0] = cmd1;
    args1[1] = NULL;
  } else {
    copy(argList.begin(), argList.end(), args1);
  }

  cerr << "argList size: " << argList.size() << endl;
  execvp(cmd1, args1);
  cerr << "Error trying to execute the command: " << cmd1 << endl;
}

int main(int argc, char *argv[]) {
  char input[MAX_INPUT_LENGTH] = {};
  int readSize, status;
  bool isLooping = true;

  pid_t pid1;

  while (isLooping) {
    printShellPrompt();
    readSize = read(0, input, MAX_INPUT_LENGTH);
    input[readSize - 1] = '\0';

    isLooping = checkForQuit(input);
    if (isLooping == false) break;

    // Fork into Parent/Child
    if ((pid1 = fork()) < 0) {
      cerr << "Fork Error!" << endl;
    }

    // In child process
    if (pid1 == 0) {
      cerr << "In child process" << endl;

      const char *inputCopy = strdup(input),
                  *pipeChars = " || ";

      string data(inputCopy);

      size_t foundIndex = -1;

      foundIndex = data.find(pipeChars);

      // Pipe block
      if (foundIndex != std::string::npos) {
        string leftString, rightString;

        leftString = data.substr(0, foundIndex);
        rightString = data.substr(foundIndex + 4);

        cerr << "Left Cmd: " << leftString << " -- " << "Right Cmd: " << rightString << endl;

      // Non-pipe block
      } else {
        executeCommand(input);
      }

      exit(127);

    // In parent process
    } else {
      if ((pid1 = waitpid(pid1, &status, 0)) < 0) {
        cerr << "waitpid error!" << endl;
      }
    }

  } // end while loop

  write(1, "\n", 1);
  return 0;
}
