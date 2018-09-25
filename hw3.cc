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

int main(int argc, char *argv[]) {
  char input1[MAX_INPUT_LENGTH] = {};
  int readSize,
        quitFlag1 = 1,
        quitFlag2 = 1;

  while (quitFlag1 != 0 && quitFlag2 != 0) {
    printShellPrompt();
    readSize = read(0, input1, MAX_INPUT_LENGTH);
    input1[readSize] = '\0';

    // Check to make sure user did not type quit
    quitFlag1 = strncmp(input1, "quit", 4);
    quitFlag2 = strncmp(input1, "q", 1);

    if (quitFlag1 == 0 || quitFlag2 == 0) {
      cout << "Quitting the program..." << endl;
      exit(0);
    }

    system(input1);
  }

  write(1, "\n", 1);
  return 0;
}
