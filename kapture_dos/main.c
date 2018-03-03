#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "files.c"
void color(int t,int b) //Function to use coloration. First number is text, second background
{
    HANDLE H=GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(H,b*16+t);
}

int main()
{
    data_values **Map;
    int nb_map = 0,color_b;
    Map = getMap(nb_map);
    /*for(int i=0;i<16;i++) //All colors. Similar color are link by (i+n8)%16 w/n={0,1}
    {
        color((i+8)%16,i);
        printf("%d ",i);
    }*/
    if (Map!=NULL)
    {
        for (int i=0;i<9;i++)
        {
            for(int j=0;j<8;j++)
            {
                if(strcmp(Map[i][j].terrain,"grass")==0 || strcmp(Map[i][j].terrain,"tree")==0)
                {
                    color_b = 2;
                }
                if(strcmp(Map[i][j].terrain,"water")==0)
                {
                    color_b = 9;
                }
                if(Map[i][j].entity == ' ')
                {
                    if(strcmp(Map[i][j].terrain,"grass")==0 || strcmp(Map[i][j].terrain,"water")==0)
                    {
                        color(15,color_b);
                        printf(" ");
                    }
                    if(strcmp(Map[i][j].terrain,"tree")==0)
                    {
                        color(4,color_b);
                        printf("+");
                    }
                }
                else
                {
                    if(strcmp(Map[i][j].team,"red")==0)
                        color(12,color_b);
                    else
                        color(9,color_b);
                    printf("%c",Map[i][j].entity);
                }

            }
            printf("\n");
        }
    }
    return 0;
}
