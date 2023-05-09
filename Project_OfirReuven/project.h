#include "linkedList.h"
#include <stdio.h>


#define SEPERATE_PARAMETERS ','
#define SEPERATE_FRAMES '\n'

int saveProject(FrameNode* curr, char* name);
void saveToFile(FILE* outputFile, Frame* frame);
FrameNode* loadProject(char* path, int* framesCount);
void readLine(FILE* inputFile, char path[], char name[], unsigned int* duration);
