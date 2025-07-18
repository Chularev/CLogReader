#include "clogreader.h"

#include <stdio.h>
#include <string.h>

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
    return false;
}

bool CLogReader::ReadChunk()
{
    if (fgets(data, 1024, fptr) != NULL) {
        return true;
    }
    return false;
}

bool CLogReader::GetNextLine(char *buf,const int bufsize)
{
    return false;
    
}
