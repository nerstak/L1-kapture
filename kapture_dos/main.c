#include <stdio.h>
#include <stdlib.h>
#include "files.c"
#if defined(_WIN32)
    #define PLATFORM_NAME "windows" // Windows
#elif defined(_WIN64)
    #define PLATFORM_NAME "windows" // Windows
#elif defined(__CYGWIN__) && !defined(_WIN32)
    #define PLATFORM_NAME "windows" // Windows (Cygwin POSIX under Microsoft Window)
#elif defined(__linux__)
    #define PLATFORM_NAME "linux"
#endif // define

int main()
{
    data_values **Map;
    int nb_map = 0;
    Map = getMap(nb_map);
    if (Map!=NULL)
    {
        for (int i=0;i<8;i++)
        {
            for(int j=0;j<8;j++)
            {
                if(Map[i][j].entity == ' ')
                    printf("%c ",Map[i][j].terrain);
                else
                    printf("%c ",Map[i][j].entity);

            }
            printf("\n");
        }
    }
    return 0;
}
