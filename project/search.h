#ifndef SEARCH_H
#define SEARCH_H

#include <variant>
#include <string.h>
#include <iostream>

class Equal
{
    char *filter;
    int filterLength;
public:
    Equal() = default;
    Equal(const char *filter)
    {
        strcpy(this->filter,filter);
        filterLength = strlen(filter);
    }
    bool search(const char* line, int lineLength, char* buf, int bufsize)
    {
        if (lineLength != filterLength)
            return false;
        for (int i = 0; i < filterLength; i++)
        {
            if (filter[i] == '?')
                continue;

            if (filter[i] != line[i])
                return false;
        }
        return true;
    }
};


class SimpleStart
{
    char *filter;
    int filterLength;
public:
    SimpleStart() = default;

    SimpleStart(const char *filter)
    {
        strcpy(this->filter,filter);
        filterLength = strlen(filter);
    }
    bool search(const char* line, int lineLength, char* buf, int bufsize)
    {
        if (lineLength < filterLength)
            return false;

        for (int i = 0; i < filterLength; i++)
        {
            if (filter[i] == '?')
                continue;

            if (filter[i] != line[i])
                return false;
        }
        return true;
    }
};

class SimpleEnd
{
    char *filter;
    int filterLength;
public:
    SimpleEnd() = default;

    SimpleEnd(const char *filter)
    {
        strcpy(this->filter,filter);
        filterLength = strlen(filter);
    }
    bool search(const char* line, int lineLength, char* buf, int bufsize)
    {
        if (lineLength < filterLength)
            return false;

        for (int i = 0; i < filterLength; i++)
        {
            if (filter[i] == '?')
                continue;

            if (filter[i] != line[lineLength - filterLength + i])
                return false;
        }
        return true;
    }

};

class BoyerMoore
{
    static const int NO_OF_CHARS = 256;
    int badchar[NO_OF_CHARS];
    char filter[100];
    int  filterLength = -1;

public:
    BoyerMoore() = default;
    BoyerMoore(const char *filter)
    {
        strcpy(this->filter,filter);
        filterLength = strlen(filter);

        int i;

        // Initialize all occurrences as -1
        for (i = 0; i < NO_OF_CHARS; i++)
            badchar[i] = -1;

        // Fill the actual value of last occurrence
        // of a character
        for (i = 0; i < filterLength; i++)
        {
            if (filter[i] == '?')
                continue;

            badchar[(int)filter[i]] = i;
        }
    }

    bool search(const char* line, int lineLength, char* buf, int bufsize)
    {
        int s = 0; // s is shift of the pattern with
        // respect to text
        while (s <= (lineLength - filterLength)) {
            int j = filterLength - 1;

            /* Keep reducing index j of pattern while
            characters of pattern and text are
            matching at this shift s */
            while (j >= 0 && (filter[j] == line[s + j] || filter[j] == '?'))
                j--;

            /* If the pattern is present at current
            shift, then index j will become -1 after
            the above loop */
            if (j < 0) {
                std::cout << "pattern occurs at shift = " << s
                          << std::endl;

                strncpy(buf,line, bufsize);
                return true;
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
                s += std::max(1, j - badchar[line[s + j]]);
        }
        return false;
    }

};

using Search = std::variant<Equal, SimpleStart, SimpleEnd, BoyerMoore>;

#endif // SEARCH_H
