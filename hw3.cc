/*********************************************************************
 * Name: Tyler Saballus
 * Date: 9/29/2018
 * Operating Systems 480 - Hutchins - Boley/Crumpton
 * Assignment 3 - Microshell Simulation
 ***********************************************************************/

#include <iostream>
#include <unistd.h>
#include <string.h>

const int MAX_INPUT_LENGTH = 128;
using std::cout;
using std::endl;

int main(int argc, char *argv[]) {
  char shellPrompt[] = "480shell> ",
        input1[MAX_INPUT_LENGTH];

  int readSize,
        quitFlag = 1;

  write(1, shellPrompt, strlen(shellPrompt));
  readSize = read(0, input1, MAX_INPUT_LENGTH);
  input1[readSize] = '\0';

  write(1, input1, strlen(input1));

  // Check to make sure user did not type quit
  quitFlag = strncmp(input1, "quit", 4);

  if (quitFlag == 0) {
    cout << "Quitting the program..." << endl;
  }

  system(input1);

  write(1, "\n", 1);
  return 0;
}
