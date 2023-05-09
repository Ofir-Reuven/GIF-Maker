/*********************************
* Class: MAGSHIMIM Final Project *
* Play function				 	 *
**********************************/

#include "view.h"
/**
play the movie!!
display the images each for the duration of the frame one by one and close the window
input: a linked list of frames to display
output: none
**/
void play(FrameNode* list, int loadType)
{
	cvNamedWindow("Display window", CV_WINDOW_AUTOSIZE); //create a window
	FrameNode* head = list;
	int imgNum = 1, playCount = 0;
	IplImage* image;
	while (playCount < GIF_REPEAT)
	{
		while (list != 0)
		{ 
			image = cvLoadImage(list->frame->path, loadType);
			IplImage* pGrayImg = 0;
			pGrayImg = cvCreateImage(cvSize(image->width, image->height), image->depth, 1);
			if (!image) //The image is empty - shouldn't happen since we checked already.
			{
				printf("Could not open or find image number %d", imgNum);
			}
			else
			{
				cvShowImage("Display window", image); //display the image
				cvWaitKey(list->frame->duration); //wait
				list = list->next;
				cvReleaseImage(&image);
			}
			imgNum++;
		}
		list = head; // rewind
		playCount++;
	}
	cvDestroyWindow("Display window");
	return;
}

/* ___playReversed___
Plays movies in reversed order.
Input: frames list, image load type - grayscale, color, etc.
Output: None
*/
void playReversed(FrameNode* list, int loadType)
{
	reverseList(&list);
	play(list, loadType);
	reverseList(&list);
}

