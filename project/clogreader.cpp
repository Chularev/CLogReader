#include "clogreader.h"

#include <stdio.h>
#include <string.h>

#include <algorithm>
#include <iostream>

CLogReader::CLogReader()
{
}

CLogReader::~CLogReader()
{

}

bool CLogReader::Open(const char *filePath)
{
    // Opening the file in read mode
    fptr = fopen("filename.txt", "r");

    // checking if the file is
    // opened successfully
    if (fptr == NULL) {
        printf("The file is not opened.");
        return false;
    }
    return true;
}
void  CLogReader::Close()
{
    fclose(fptr);
}

bool CLogReader::SetFilter(const char *filter)
{
    strcpy(this->filter,filter);
    filterLength = strlen(filter);
    badCharHeuristic(filter, strlen(filter));
    return true;
}

bool CLogReader::ReadChunk()
{
    if (fgets(txt, 1024, fptr) != NULL) {
        return true;
    }
    return false;
}

bool CLogReader::GetNextLine(char *buf,const int bufsize)
{
   // int m = pat.size();
    int n = 1024;

    int s = 0; // s is shift of the pattern with
        // respect to text
    while (s <= (n - filterLength)) {
        int j = filterLength - 1;

        /* Keep reducing index j of pattern while
        characters of pattern and text are
        matching at this shift s */
        while (j >= 0 && filter[j] == txt[s + j])
            j--;

        /* If the pattern is present at current
        shift, then index j will become -1 after
        the above loop */
        if (j < 0) {
            std::cout << "pattern occurs at shift = " << s
                 << std::endl;

            /* Shift the pattern so that the next
            character in text aligns with the last
            occurrence of it in pattern.
            The condition s+m < n is necessary for
            the case when pattern occurs at the end
            of text */
            s += (s + filterLength < n) ? filterLength - badchar[txt[s + filterLength]] : 1;
        }

        else
            /* Shift the pattern so that the bad character
            in text aligns with the last occurrence of
            it in pattern. The max function is used to
            make sure that we get a positive shift.
            We may get a negative shift if the last
            occurrence of bad character in pattern
            is on the right side of the current
            character. */
            s += std::max(1, j - badchar[txt[s + j]]);
    }
    return false;
    
}

// Boyer Moore Algorithm
void CLogReader::badCharHeuristic(const char *filter, int size)
{
    int i;

    // Initialize all occurrences as -1
    for (i = 0; i < NO_OF_CHARS; i++)
        badchar[i] = -1;

    // Fill the actual value of last occurrence
    // of a character
    for (i = 0; i < size; i++)
        badchar[(int)filter[i]] = i;
}
