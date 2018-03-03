#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <string.h>
#include <errno.h>

#include "game.c"
#include "files.c"


void color(int t,int b) //Function to use coloration. First number is text, second background
{
    HANDLE H=GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(H,b*16+t);
}

int main()
{
    data_values **Map;
    data_save save;
    int nb_map = 0,color_b;
    Map = getMap(nb_map,&save);
    /*for(int i=0;i<16;i++) //All colors. Similar color are link by (i+n8)%16 w/n={0,1}
    {
        color((i+8)%16,i);
        printf("%d ",i);
    }*/
    if (Map==NULL)
    {
        printf("An error occured while loading the map.\n");
    }
    else
    {
        int game = 1;
        strcpy(save.team,"red");
        save.turn = 1;
        while(game)
        {

            for (int i=0;i<save.line;i++)
            {
                for(int j=0;j<save.column;j++)
                {
                    if(Map[i][j].fog==3 || (Map[i][j].fog==1 && strcmp(save.team,"red")==0) || (Map[i][j].fog==2 && strcmp(save.team,"blue")==0)) //First we set the color of the back
                    {
                        if(strcmp(Map[i][j].terrain,"grass")==0 || strcmp(Map[i][j].terrain,"tree")==0)
                        {
                            color_b = 2;
                        }
                        if(strcmp(Map[i][j].terrain,"water")==0)
                        {
                            color_b = 9;
                        }
                        if(strcmp(Map[i][j].terrain,"check_for_b")==0 || strcmp(Map[i][j].terrain,"check_for_r")==0)
                        {
                            color_b = 14;
                        }
                    }
                    else
                    {
                       color_b = 8;
                    }
                    if(Map[i][j].entity == ' ')
                    {
                        if(color_b == 8 || strcmp(Map[i][j].terrain,"grass")==0 || strcmp(Map[i][j].terrain,"water")==0 || strcmp(Map[i][j].terrain,"check_for_b")==0 || strcmp(Map[i][j].terrain,"check_for_r")==0)
                        {
                            color(15,color_b);
                            printf(" ");
                        }
                        else if(strcmp(Map[i][j].terrain,"tree")==0)
                        {
                            color(4,color_b);
                            printf("+");
                        }
                    }
                    else
                    {
                        //Séparer cette fonction en deux if. Un pour fog, et autre couleur player
                        if(color_b != 8)
                        {
                            if(strcmp(Map[i][j].team,"red")==0)
                            {
                                color(12,color_b);
                            }
                            else if(strcmp(Map[i][j].team,"blue"))
                            {
                               color(9,color_b);
                            }
                            printf("%c",Map[i][j].entity);
                        }
                        else
                        {
                            color(15,color_b);
                            printf(" ");
                        }
                    }

                }
                printf("\n");
            }
            game = 0;
        }
    }
    return 0;
}
