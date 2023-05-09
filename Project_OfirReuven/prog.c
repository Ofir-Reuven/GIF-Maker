#pragma warning(disable : 4996)
#define CV_IGNORE_DEBUG_BUILD_GUARD
#include <stdio.h>
#include <opencv2/imgcodecs/imgcodecs_c.h>
#include <opencv2/core/core_c.h>
#include <opencv2/highgui/highgui_c.h>

#include "linkedList.h"
#include "view.h"
#include "common.h"
#include "project.h"

#define CREATE 0
#define LOAD 1
enum options {EXIT, ADD_FRAME, REMOVE_FRAME, CHANGE_INDEX, CHANGE_DURATION, CHANGE_ALL_DURATIONS, PRINT_FRAMES, PLAY, SAVE, PLAY_REVERSED};
void printMenu();
void printStartingMenu();
int getOption(int min, int max, int isIndex);
int setLoadType();
int main()
{
	FrameNode* frameList = NULL;
	FrameNode* frameNode = NULL;
	FrameNode* prev = NULL;
	int loadType = 0;
	char temp[STR_LEN] = { 0 }; // Used for string inputs
	enum options option = 0;
	int index = 0;
	int framesCount = 0;
	unsigned int duration = 0;

	printStartingMenu();
	option = getOption(CREATE, LOAD, FALSE);
	if (option == LOAD)
	{
		printf("%sEnter the path of the project (including project name):\n%s", YELLOW, WHITE);
		ffgets(temp, STR_LEN);
		frameList = loadProject(temp, &framesCount);
		if (frameList != ERROR)
		{
			printf("%sProject loaded successfully\n\n", GREEN);
		}
		else
		{
			frameList = NULL;
		}
	}
	else
	{
		printf("%sWorking on a new project.\n", GREEN);
	}

	do
	{
		printMenu();
		option = getOption(EXIT, PLAY_REVERSED, FALSE);
		printf("%s", WHITE);
		switch (option)
		{
			case EXIT:
				deleteAllFrames(frameList);
				printf("Bye!");
				break;
			case ADD_FRAME:
				printf("*** Creating new frame ***\n");
				frameNode = buildFrame(frameList);
				if (frameNode)
				{
					addFrameToEnd(&frameList, frameNode);
					framesCount++;
				}
				break;
			case REMOVE_FRAME:
				printf("Enter the name of the frame you wish to erase\n");
				ffgets(temp, STR_LEN);
				if (deleteSpecificFrame(&frameList, temp))
				{
					framesCount--;
				}
				break;
			case CHANGE_INDEX:
				printf("Enter the name of the frame:\n");
				ffgets(temp, STR_LEN);
				printf("Enter the new index in the movie you wish to place the frame:\n");
				getOption(1, framesCount, TRUE);
				changeFrameIndex(&frameList, temp, index);
				break;
			case CHANGE_DURATION:
				printf("Enter the name of the frame:\n");
				ffgets(temp, STR_LEN);
				printf("Enter duration for the frame:");
				scanf("%u", &duration);
				changeFrameDuration(frameList, temp, duration);
				break;
			case CHANGE_ALL_DURATIONS:
				printf("Enter duration for all frames:\n");
				scanf("%u", &duration);
				changeAllFramesDurations(frameList, duration);
				break;
			case PRINT_FRAMES:
				printFrames(frameList);
				break;
			case PLAY:
				loadType = setLoadType();
				play(frameList, loadType);
				break;
			case SAVE:
				printf("Enter a path for the project:\n");
				ffgets(temp, STR_LEN);
				if (!saveProject(frameList, temp))
				{
					printf("Project saved successfully\n");
				}
				break;
			case PLAY_REVERSED:
				loadType = setLoadType();
				playReversed(frameList, loadType);
				break;
			default:
				printf("Invalid option, try again\n");
		}
	} while (option != EXIT);
	

	return 0;
}

/* ___printMenu___
Prints menu
Input: None
Output: None
*/
void printMenu()
{
	printf("\n%sWhat would you like to do?\n", YELLOW);
	printf("%s  [0] Exit\n", WHITE);
	printf("%s  [1] Add new frame\n", CYAN);
	printf("%s  [2] Remove a frame\n", WHITE);
	printf("%s  [3] Change frame index\n", CYAN);
	printf("%s  [4] Change frame duration\n", WHITE);
	printf("%s  [5] Change duration of all frames\n", CYAN);
	printf("%s  [6] List frames\n", WHITE);
	printf("%s  [7] Play movie!\n", CYAN);
	printf("%s  [8] Save project\n", WHITE);
	printf("%s  [9] Play movie in reversed order!\n", CYAN);
}

/* ___printStartingMenu___
Prints starting menu.
Input: None
Output: None
*/
void printStartingMenu()
{
	printf("%s  Welcome to Magshimim Movie Maker! what would you like to do?\n", YELLOW);
	printf("%s  [0] Create a new project\n", WHITE);
	printf("%s  [1] Load existing project\n", CYAN);
}

/* ___getOption___
Gets option from the user.
Input: min value of option, max value of option.
Output: chosen option
*/
int getOption(int min, int max, int isIndex)
{
	int option = 0;

	printf("%s", WHITE);
	scanf("%d", &option);
	getchar();
	while (option < min || option > max)
	{
		if (isIndex) // Print messages regarding index choice
		{
			printf("%sThe movie contains only %d frames!\n", RED, max);
			printf("%sEnter the new index in the movie you wish to place the frame:\n", RED);
		}
		else
		{
			printf("%sYou should type one of the options - %d-%d!\n", RED, min, max);
			printMenu();
		}
		printf("%s", WHITE);
		scanf("%d", &option);
		getchar();
	}

	return option;
}

/* ___setLoadType___
Sets load type for images when playing the movie.
Input: None
Output: Load type
*/
int setLoadType()
{
	int loadType = 0;

	printf("%sDo you want to play the movie in grayscale? (0 - yes, otherwise no)\n%s", YELLOW, WHITE);
	scanf("%d", &loadType);

	if (loadType != CV_LOAD_IMAGE_GRAYSCALE)
	{
		loadType = CV_LOAD_IMAGE_COLOR;
	}

	return loadType;
}