#include <unistd.h>
#include <iostream>
#include "fcntl.h"

int main(int argc, char ** argv)
{
    FILE * F = fopen("map.txt", "w");
    
    if (F != NULL)
    {
        int n = atoi(argv[1]);
        int c;
        for (int i = 0; i < n; i++)
        {
            if (rand() % 6 == 0)
                c = '#';
            else
                c = '.';
            fputc(c, F);
        }
        fclose(F);
    }
    return 0;
}