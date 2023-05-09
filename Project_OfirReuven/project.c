#include "linkedList.h"
#include "project.h"
#include "common.h"
#include <string.h>

#pragma warning(disable : 4996)

/* ___saveProject___
Saves project to a file.
Input: head of the frames list, path to the file.
Output: 0 if success, -1 if error.
*/
int saveProject(FrameNode* curr, char* path)
{
	FILE* outputFile = fopen(path, "w");

	if (!outputFile)
	{
		printf("%sError opening file\n%s", RED, WHITE);
		return ERROR;
	}

	//fputc(' ', outputFile);
	while (curr)
	{
		saveToFile(outputFile, curr->frame);
		curr = curr->next;
	}

	fclose(outputFile);

	return 0;
}


/* ___saveToFile___
Saves a frame to a file.
Input: file, frame.
Output: None
*/
void saveToFile(FILE* outputFile, Frame* frame)
{
	
	char temp[STR_LEN + 1] = { 0 };

	// Writes path, name and duration to the file
	strcpy(temp, frame->path);
	fwrite(temp, 1, strlen(temp), outputFile);
	fputc(',', outputFile);

	strcpy(temp, frame->name);
	fwrite(temp, 1, strlen(temp), outputFile);

	fprintf(outputFile, ",%d\n", frame->duration);
}


/* ___loadProject___
Loads project from a file.
Input: path to the project file.
Output: head of the frame list saved in the file.
*/
FrameNode* loadProject(char* path, int* framesCount)
{
	FrameNode* curr = NULL;
	FrameNode* prev = NULL;
	FrameNode* head = NULL;
	FILE* inputFile = fopen(path, "r");
	int index = 0;
	char ch = ' ';
	char name[STR_LEN] = { 0 };
	unsigned int duration = 0;
	int lineLength = 0;

	if (!inputFile)
	{
		printf("%sError! - cant open file, creating a new project\n%s", RED, WHITE);
		return ERROR;
	}

	// Reads every line from the file and creates a frame node from it
	while ((ch = fgetc(inputFile)) != EOF && ch != ' ' && ch != SEPERATE_FRAMES)
	{
		path[index] = ch;
		readLine(inputFile, path, name, &duration);

		curr = createFrame(path, duration, name);
		
		if (!head) // Head has to be assigned
		{
			head = curr;
		}
		if (prev)
		{
			prev->next = curr;
		}

		prev = curr;
		curr = curr->next;

		duration = 0;
		(*framesCount)++;
	}

	fclose(inputFile);
	return head;
}



/* ___readLine___
Reads one line from project file and seperates it to path, name and duration.
Input: project file, path, name and pointer to the duration
Output: None
*/
void readLine(FILE* inputFile, char path[], char name[], unsigned int* duration)
{
	char ch = ' ';
	int index = 1;

	// Gets path
	while ((ch = fgetc(inputFile)) != SEPERATE_PARAMETERS && ch != EOF)
	{
		path[index] = ch;
		index++;
	}
	path[index] = 0;
	index = 0;

	// Get name
	while ((ch = fgetc(inputFile)) != SEPERATE_PARAMETERS && ch != EOF)
	{
		name[index] = ch;
		index++;
	}
	name[index] = 0;
	index = 0;

	// Get duration
	while ((ch = fgetc(inputFile)) != SEPERATE_FRAMES && ch != EOF)
	{
		*duration *= MULTIPLE_BY_10;
		*duration += ch - ASCII_NUM_START;
		index++;
	}
}