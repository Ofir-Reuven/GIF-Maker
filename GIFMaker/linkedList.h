#ifndef LINKEDLISTH
#define LINKEDLISTH

#define FALSE 0
#define TRUE !FALSE

#define PREV_INDEX 2 // Index of the node before the node we want to replace
#define UNUSED_ADDRESS (void*) 1
// Frame struct
typedef struct Frame
{
	char*		name;
	unsigned int	duration;
	char*		path;  
} Frame;


// Link (node) struct
typedef struct FrameNode
{
	Frame* frame;
	struct FrameNode* next;
} FrameNode;


void changeFrameDuration(FrameNode* head, char* name, unsigned int duration);
void deleteAllFrames(FrameNode* head);
void addFrameToEnd(FrameNode** head, FrameNode* newFrame);
FrameNode* findFrame(FrameNode* head, char* name, FrameNode** prev);
void changeFrameIndex(FrameNode** head, char* name, int index);
FrameNode* buildFrame(FrameNode* head);
void deleteFrame(FrameNode* frameNode);
int deleteSpecificFrame(FrameNode** head, char* name);
FrameNode* buildFrame(FrameNode* head);
void changeAllFramesDurations(FrameNode* head, unsigned int duration);
void printFrames(FrameNode* head);
void reverseList(FrameNode** p_head);
FrameNode* createFrame(char* path, unsigned int duration, char* name);
#endif
