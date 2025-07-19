#include "clogreader.h"

#include <string.h>

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

    filters[filtersLengs] = BoyerMoore(filter);
    ++filtersLengs;
    return true;
}

bool CLogReader::GetNextLine(char *buf, const int bufsize)
{
   char *txt;
    int n = 0;
    while (get_line(&txt,n))
    {
        for (int i = 0; i < filtersLengs; i++)
        {
            if (std::visit([&](auto&& arg) {
                    return arg.search(txt,n,buf,bufsize);
                }, filters[i]))
            {
                return true;
            }
        }

    }
    return false;
}
