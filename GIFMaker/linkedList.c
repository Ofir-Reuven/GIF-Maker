#include "linkedList.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "common.h"

#pragma warning(disable : 4996)

/* ___deleteAllFrames___
Deletes entire video
Input: head of the frames list
Output: None
*/
void deleteAllFrames(FrameNode* head)
{
	FrameNode* temp = NULL;
	while (head)
	{
		temp = head->next;
		deleteFrame(head);
		head = temp;
	}
}

/* ___deleteFrame___
deletes a frame node
Input: frame node to delete
Output: None
*/
void deleteFrame(FrameNode* frameNode)
{
	free(frameNode->frame->name);
	free(frameNode->frame->path);
	free(frameNode->frame);
	free(frameNode);
}

/* ___deleteSpecificFrame___
Deletes frame with a specific name entered by user.
Input: head of the frames list, name of frame to delete.
Output: None
*/
int deleteSpecificFrame(FrameNode** head, char* name)
{
	FrameNode* next = NULL;
	FrameNode* prev = UNUSED_ADDRESS;
	FrameNode* frameNode = NULL;
	frameNode = findFrame(*head, name, &prev);
	int result = TRUE;
	if (frameNode)
	{
		if (prev == UNUSED_ADDRESS) // If the asked frame is the head 
		{
			*head = (*head)->next;
		}
		else
		{
			next = prev->next->next;
			prev->next = next;
		}

		deleteFrame(frameNode);
	}
	else
	{
		printf("%sThe frame was not found\n\n%s", RED, WHITE);
		result = FALSE;
	}
	
	return result;
}

/* ___changeFrameDuration___
Changes frame duration.
Input: frame list, name of the frame, new duration.
Output: None
*/
void changeFrameDuration(FrameNode* head, char* name, unsigned int duration)
{
	FrameNode* frameNode = NULL;

	frameNode = findFrame(head, name, &frameNode);
	if (frameNode)
	{
		frameNode->frame->duration = duration;
		printf("%sDuration updated\n%s", GREEN, WHITE);
	}
	else
	{
		printf("%sFrame not found\n%s", RED, WHITE);
	}
}

/* ___findFrame___
Finds frames asked by the user.
Input: head of the frames list, name of the frame asked, 
need previous frame (will be updated. NULL - no, UNUSED_ADDRESS - yes).
Output: found frame, NULL if wasnt found
*/
FrameNode* findFrame(FrameNode* head, char* name, FrameNode** prev)
{
	int found = FALSE;
	FrameNode* foundFrame = NULL;

	if (head)
	{
		// Checks if the frame asked is the head
		if (!strcmp(head->frame->name, name))
		{
			foundFrame = head;
		}

		// Finds the asked frame
		while (head && !foundFrame)
		{
			if (head->next && *prev == UNUSED_ADDRESS && !strcmp(head->next->frame->name, name)) // If previous frame is needed as well
			{
				*prev = head;
			}
			if (!strcmp(head->frame->name, name)) // Frame found
			{
				foundFrame = head;
			}
			head = head->next;
		}

	}

	return foundFrame;
}

/* ___changeFrameIndex___
Changes index of a frame
Input: location of head of frames list, name of frame to move, index to move into.
Output: None
*/
void changeFrameIndex(FrameNode** head, char* name, int index)
{
	FrameNode* curr = *head;
	FrameNode* prev = UNUSED_ADDRESS;
	FrameNode* frameNode = findFrame(*head, name, &prev);
	if (!frameNode)
	{
		printf("%sthis frame does not exist\n%s", RED, WHITE);
	}
	else
	{
		// Detach frame node
		if (frameNode == *head) // want to move head
		{
			*head = (*head)->next;
		}
		else
		{
			prev->next = frameNode->next;
		}

		// Attach frame node to new index
		if (index == 1) // Want to replace head
		{
			frameNode->next = *head;
			*head = frameNode;
		}
		else
		{
			curr = *head;
			// Moving to the chosen index
			while (curr->next && index - PREV_INDEX)
			{
				curr = curr->next;
				index--;
			}

			if (index - PREV_INDEX > 0)
			{
				printf("%sIndex out of range!\n%s", RED, WHITE);
			}
			else
			{
				frameNode->next = curr->next;
				curr->next = frameNode;
			}
		}
	}
}

/* ___addFrameToEnd___
Adds frame to the end of the frames list.
Input: location of frames list, frame to add.
Output: None
*/
void addFrameToEnd(FrameNode** head, FrameNode* newFrame)
{
	FrameNode* curr = *head;
	if (newFrame)
	{
		if (!*head) // If this is the first frame in list
		{
			*head = newFrame;
		}
		else
		{
			// Adds to the end
			while (curr->next)
			{
				curr = curr->next;
			}
			curr->next = newFrame;
		}
	}
	
}

/* ___printFrames___
Prints all frames.
Input: frames list
Output: None
*/
void printFrames(FrameNode* head)
{
	printf("			%sName		%sDuration		%sPath\n", WHITE, GREEN, CYAN);
	while (head)
	{
		printf("			%s%s		%s%u ms			%s%s\n", WHITE, head->frame->name, GREEN, head->frame->duration, CYAN, head->frame->path);
		head = head->next;
	}
}

/* ___changeAllFramesDurations___
Changes durations of all frames to a specific duration.
Input: frames list, duration.
Output: None
*/
void changeAllFramesDurations(FrameNode* head, unsigned int duration)
{
	while (head)
	{
		head->frame->duration = duration;
		head = head->next;
	}
}


/* ___buildFrame___
Builds a new frame.
Input: frames list.
Output: new frame node
*/
FrameNode* buildFrame(FrameNode* head)
{
	FrameNode* frameNode = NULL;
	Frame* frame = NULL;
	char path[STR_LEN] = { 0 };
	char name[STR_LEN] = { 0 };
	FILE* checkFile = NULL;
	unsigned int duration = 0;
	FrameNode* prev = NULL;

	printf("%sPlease insert frame path:\n%s", YELLOW, WHITE);
	ffgets(path, STR_LEN);
	printf("%sPlease insert frame duration(in miliseconds):\n%s", YELLOW, WHITE);
	scanf("%u", &duration);
	getchar();
	printf("%sPlease choose a name for that frame:\n%s", YELLOW, WHITE);
	ffgets(name, STR_LEN);
	
	// While name is already assigned to a frame
	while (findFrame(head, name, &prev))
	{
		printf("%sName already exists! choose another name\n%s", RED, WHITE);
		ffgets(name, STR_LEN);
	}

	checkFile = fopen(path, "r");
	if (!checkFile)
	{
		printf("%sCan't find file! Frame will not be added\n\n%s", RED, WHITE);
	}
	else // Entered path is fine
	{
		fclose(checkFile);

		frameNode = createFrame(path, duration, name);

	}

	return frameNode;
}

/* ___createFrame___
Creates frame node.
Input: path, name and duration of the frame.
Output: created frame node.
*/
FrameNode* createFrame(char* path, unsigned int duration, char* name)
{

	FrameNode* frameNode = NULL;
	Frame* frame = NULL;
	// Allocates memory for the frame node
	frameNode = (FrameNode*)malloc(sizeof(FrameNode));
	if (!frameNode)
	{
		printf("Error allocating memory");
		return NULL;
	}
	frameNode->frame = (Frame*)malloc(sizeof(Frame));
	frame = frameNode->frame; // For convenience
	if (!frame)
	{
		printf("Error allocating memory");
		return NULL;
	}
	frame->path = (char*)malloc(strlen(path) + 1);
	frame->name = (char*)malloc(strlen(name) + 1);
	if (!frame->name || !frame->path)
	{
		printf("Error allocating memory");
		return NULL;
	}

	// Inserts values into frame node
	strcpy(frame->path, path);
	frame->duration = duration;
	strcpy(frame->name, name);
	frameNode->next = NULL;

	return frameNode;
}



/* ___reverseList___
Reverses list
Input: location of list head.
Output: None
*/
void reverseList(FrameNode** p_head)
{
	FrameNode* prev = NULL;
	FrameNode* curr = *p_head;
	FrameNode* next = NULL;

	while (curr) {
		next = curr->next;
		curr->next = prev;
		// Continues to next
		prev = curr;
		curr = next;
	}

	*p_head = prev;
}