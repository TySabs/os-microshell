/*********************************************************************
 * Name: Tyler Saballus
 * Date: 9/29/2018
 * Operating Systems 480 - Hutchins - Boley/Crumpton
 * Assignment 3 - Microshell Simulation
 ***********************************************************************/

#include <iostream>
#include <unistd.h>
#include <string.h>

const int MAX_INPUT_LENGTH = 1024;
using std::cout;
using std::endl;

void printShellPrompt() {
  char shellPrompt[] = "480shell> ";
  write(1, shellPrompt, strlen(shellPrompt));
}

bool checkForQuit(char *input1) {
  int quitFlag1 = 1,
      quitFlag2 = 1;
  // Check to make sure user did not type quit
  quitFlag1 = strncmp(input1, "quit", 4);
  quitFlag2 = strncmp(input1, "q", 1);

  if (quitFlag1 == 0 || quitFlag2 == 0) {
    cout << "Quitting the program..." << endl;
    return false;
  } else {
    return true;
  }
}

int main(int argc, char *argv[]) {
  char input1[MAX_INPUT_LENGTH] = {};
  int readSize;
  bool isLooping = true;

  pid_t pid1;

  while (isLooping == true) {
    printShellPrompt();
    readSize = read(0, input1, MAX_INPUT_LENGTH);
    input1[readSize] = '\0';

    isLooping = checkForQuit(input1);

    system(input1);
  } // end while loop

  write(1, "\n", 1);
  return 0;
}
