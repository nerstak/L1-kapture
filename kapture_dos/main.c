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
        for (int i=0;i<9;i++)
        {
            for(int j=0;j<8;j++)
            {
                if(Map[i][j].entity[0] == ' ')
                {
                    if(strcmp(Map[i][j].terrain,"grass")==0)
                    {
                        printf(". ");
                    }
                    if(strcmp(Map[i][j].terrain,"water")==0)
                    {
                        printf("~ ");
                    }
                    if(strcmp(Map[i][j].terrain,"tree")==0)
                    {
                        printf("+ ");
                    }
                }
                else
                    printf("%s ",Map[i][j].entity);

            }
            printf("\n");
        }
    }
    return 0;
}
