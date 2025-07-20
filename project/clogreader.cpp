#include "clogreader.h"

#include <string.h>

#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

bool CLogReader::MoveToNextLine()
{
    line_start = ++line_end;
    while (line_end < data + data_size) {
        if (*line_end == '\n') {
            return true;
        }
        line_end++;
    }
    // Last line
    if (line_end < data + data_size)
        return true;
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

    data = (char*)mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fileDescriptor, 0);
    if (data == MAP_FAILED) {
        perror("mmap");
        close(fileDescriptor);
        return false;
    }

    line_end = data - 1;
    data_size = sb.st_size;

    return true;
}
void  CLogReader::Close()
{
    munmap(data, sb.st_size);
    close(fileDescriptor);
}

void CLogReader::fillFilters(bool startsWithStar, bool endsWithStar, char* subFilters[], int size)
{
    if (size == 1 && !startsWithStar && !endsWithStar)
    {
        filters[filtersLength++] = Equal(subFilters[0]);
        return;
    }
    int index = 0;
    if (!startsWithStar)
    {
        filters[filtersLength++] = SimpleStart(subFilters[index++]);
    }
    if (!endsWithStar)
    {
        filters[filtersLength++] = SimpleEnd(subFilters[--size]);
    }

    for (; index < size; index++)
    {
        filters[filtersLength++] = BoyerMoore(subFilters[index]);
    }


}

bool CLogReader::SetFilter(const char *filter)
{
    if (!filter) return false;

    filtersLength = 0;

    char* filter_copy = strdup(filter);

    bool startsWithStar = (filter[0] == '*');
    bool endsWithStar = (filter[strlen(filter) - 1] == '*');

    char* subFilters[10];
    int index = 0;

    subFilters[index] = strtok (filter_copy,"*");

    while (subFilters[index] != NULL)
    {
        /*
        std::cout << "----------------"  << "\n";
        std::cout << subFilters[index]  << "\n";
        std::cout << "----------------"  << "\n";
        */

        if (++index == 10)
        {
            std::cerr << "Error: filter can not have more than 10 asterisk"  << "\n";
            return false;
        }
        else
            subFilters[index] = strtok (NULL, "*");
    }

    fillFilters(startsWithStar, endsWithStar, subFilters, index);
    return true;
}

bool CLogReader::GetNextLine(char *buf, const int bufsize)
{
     while (MoveToNextLine())
    {
        int lineLength = line_end - line_start;

        int i = 0;
        for (; i < filtersLength; i++)
        {
            if (!std::visit([&](auto&& arg) {
                    return arg.search(line_start,lineLength);
                }, filters[i]))
            {
                break;
            }
        }
        if (i == filtersLength)
        {
            memset(buf, 0, bufsize);
            int min = std::min(bufsize, lineLength);
            strncpy(buf,line_start, min);
            return true;
        }

    }
    return false;
}
