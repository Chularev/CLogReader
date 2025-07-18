#include "clogreader.h"

#include <string.h>

#include <iostream>

#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

bool CLogReader::get_line(char **line, int& lineLength) {

    while (current_position < start_position + data_size) {
        if (*current_position == '\n') {
            lineLength = current_position - line_start;
            *line = line_start;
            line_start = current_position + 1;
            current_position++;
            return true;
        }
        current_position++;
    }

    // Process last line if no trailing newline
    if (line_start < current_position) {
        lineLength = current_position - line_start;
        *line = line_start;
        line_start = current_position + 1;
        current_position++;
        return true;
        //size_t line_length = current_position - line_start;
        // printf("%.*s\n", (int)line_length, line_start);
    }
    return false;
}

CLogReader::CLogReader()
{
}

CLogReader::~CLogReader()
{

}

bool CLogReader::Open(const char *filePath)
{
    fileDescriptor = open(filePath, O_RDONLY);
    if (fileDescriptor == -1) {
        perror("open");
        return false;
    }

    if (fstat(fileDescriptor, &sb) == -1) {
        perror("fstat");
        close(fileDescriptor);
        return false;
    }

    start_position = (char*)mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fileDescriptor, 0);
    if (start_position == MAP_FAILED) {
        perror("mmap");
        close(fileDescriptor);
        return false;
    }

    line_start = start_position;
    current_position = start_position;
    data_size = sb.st_size;

    return true;
}
void  CLogReader::Close()
{
    munmap(start_position, sb.st_size);
    close(fileDescriptor);
}

bool CLogReader::SetFilter(const char *filter)
{
    strcpy(this->filter,filter);
    filterLength = strlen(filter);
    badCharHeuristic(filter, strlen(filter));
    return true;
}

bool CLogReader::GetNextLine(char *buf, const int bufsize)
{
   // int m = pat.size();
    int n = 1024;
   char *txt;
   get_line(&txt,n);

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

            strncpy(buf,txt, bufsize);
            return true;

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
