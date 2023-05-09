#include <stdio.h>
#include <string.h>
#include "common.h"

/* ___ffgets___
Upgraded fgets to get string from the user.
Input: destination of the string, buffer size.
Output: None
*/
void ffgets(char* dest, int buffer)
{
	fgets(dest, buffer, stdin);
	dest[strcspn(dest, "\n")] = 0;
}

