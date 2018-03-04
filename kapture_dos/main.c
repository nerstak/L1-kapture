#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <string.h>
#include <errno.h>
#include <conio.h>

#include "files.h"
#include "game.h"


#include <unistd.h>

void clearScreen()
{
  const char *CLEAR_SCREEN_ANSI = "\e[1;1H\e[2J";
  write(STDOUT_FILENO, CLEAR_SCREEN_ANSI, 12);
}
void color(int t,int b) //Function to use coloration. First number is text, second background
{
    HANDLE H=GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(H,b*16+t);
}

int main()
{
    data_values **Map;
    data_save save;
    int nb_map = 0,color_b, game;
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
        typedef struct selection{
            int n;
            int x;
            int y;
        } selection;
        selection cursor;
        int game = 1; //For the while loop
        strcpy(save.team,"red"); //Init of the save in the RAM
        save.turn = 1; //Same
        int turnend=0,pawn=0;
        char key;
        do
        {
            cursor.n = cursor.x = cursor.y =-1;

            do
            {
                system("cls");
                for (int i=0;i<save.line;i++) //Display of the map
                {
                    for(int j=0;j<save.column;j++)
                    {
                        if(Map[i][j].fog==3 || (Map[i][j].fog==1 && strcmp(save.team,"red")==0) || (Map[i][j].fog==2 && strcmp(save.team,"blue")==0)) //First we set the color of the back
                        {
                            if(strcmp(Map[i][j].terrain,"grass")==0 || strcmp(Map[i][j].terrain,"tree")==0|| strcmp(Map[i][j].terrain,"spawn_r")==0 || strcmp(Map[i][j].terrain,"spawn_b")==0 ) //Giving the color green for grass and tree
                            {
                                color_b = 2;
                            }
                            if(strcmp(Map[i][j].terrain,"water")==0) //Color blue for water
                            {
                                color_b = 11;
                            }
                            if(strcmp(Map[i][j].terrain,"check_for_b")==0 || strcmp(Map[i][j].terrain,"check_for_r")==0) //Color for zone of drop
                            {
                                color_b = 14;
                            }
                        }
                        else //Fog of war
                        {
                            color_b = 8;
                        }
                        if(Map[i][j].entity == ' ') //Display terrain without entity
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
                        else //Display entity and their terrain
                        {
                            if(color_b != 8)
                            {
                                pawn++;
                                if(strcmp(Map[i][j].team,"red")==0)
                                {
                                    color(12,color_b);
                                }
                                else if(strcmp(Map[i][j].team,"blue")==0)
                                {
                                    color(9,color_b);
                                }
                                if(cursor.n==-1)
                                {
                                    cursor.n=1;
                                    cursor.x = j;
                                    cursor.y = i;
                                }
                                if(pawn==cursor.n)
                                {
                                    cursor.x = j;
                                    cursor.y = i;
                                    color(15,color_b);
                                }
                                printf("%c",Map[i][j].entity);
                            }
                            else //Fog of war display
                            {
                                color(15,color_b);
                                printf(" ");
                            }
                        }

                    }
                    color(0,0);
                    printf("\n");
                }

                key = userinput(); //Getting input from user
                if(key=='0')
                {
                    turnend=1;
                }
                else
                {
                    if(key=='<')
                    {
                        if(cursor.n!=1)
                            cursor.n--;
                    }
                    if(key=='>')
                    {
                        if(cursor.n!=save.nb_pawn)
                            cursor.n++;
                    }
                    pawn=0;
                }
            }while(turnend==0);

            game = 0;
        }while(game);
    }
    return 0;
}
