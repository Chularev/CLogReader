#ifndef SEARCH_H
#define SEARCH_H

#include <variant>
#include <string.h>
#include <iostream>

class Equal
{
public:
    Equal() = default;
    Equal(const char *filter)
    {

    }
    bool search(const char* txt, int n, char* buf, int bufsize)
    {
        return false;
    }
};


class SimpleStart
{
public:
    SimpleStart() = default;

    SimpleStart(const char *filter)
    {

    }
    bool search(const char* txt, int n, char* buf, int bufsize)
    {
        return false;
    }
};

class SimpleEnd
{
public:
    SimpleEnd() = default;

    SimpleEnd(const char *filter)
    {

    }
    bool search(const char* txt, int n, char* buf, int bufsize)
    {
        return false;
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
            badchar[(int)filter[i]] = i;
    }

    bool search(const char* txt, int n, char* buf, int bufsize)
    {
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

};

using Search = std::variant<Equal, SimpleStart, SimpleEnd, BoyerMoore>;

#endif // SEARCH_H
