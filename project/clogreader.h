#include <sys/stat.h>
#include <stdio.h>

#include "search.h"

class CLogReader
{
    public:
        CLogReader();
        ~CLogReader();

       bool    Open(const char *filePath);                       // открытие файла, false - ошибка
       void    Close();                         // закрытие файла

       bool    SetFilter(const char *filter);   // установка фильтра строк, false - ошибка
       bool    GetNextLine(char *buf,           // запрос очередной найденной строки,
                           const int bufsize);  // buf - буфер, bufsize - максимальная длина
                                                // false - конец файла или ошибка
   protected:
       Search filters [10];
       int filtersLength = 0;
   private:
       bool get_line(char **line, int& lineLength);
       void fillFilters(bool startsWithStar, bool endsWithStar, char *subFilters[], int size);

       // File
       char* start_position;
       int fileDescriptor;
       struct stat sb;
       char* line_start;
       char* current_position;
       size_t data_size;

};
