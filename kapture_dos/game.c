#include <stdio.h>//TODO: replace the Map[][]=="x" to some fucking strcmp()   (and also fix all the struct refferences so they are consistent with Karsten's)
#include <stdlib.h>
#include <time.h>

#include "files.h"


char userinput()
{
    int inputed=getch();
    switch(inputed)
    {
        case 48:
            return '0'; //Key 0
            break;
        case 49:
            return '1'; //Key 1
            break;
        case 50:
            return '2'; //Key 2
            break;
        case 51:
            return '3'; //Key 3
            break;
        case 52:
            return '4'; //Key 4
            break;
        case 53:
            return '5'; //Key 5
            break;
        case 54:
            return '6'; //Key 6
            break;
        case 55:
            return '7'; //Key 7
            break;
        case 56:
            return '8'; //Key 8
            break;
        case 57:
            return '9'; //Key 9
            break;
        default: //Keys up, down, left, right and other
            return userinput();
            break;
        case 77:
            return '>'; //Key right
            break;
        case 75:
            return '<'; //Key left
            break;
    }
}

int posexist(int xpos,int ypos,int TEMPWIDTH,int TEMPHEIGHT) //checks if a coordinate is within the map's borders
{
    if(xpos>=0 && ypos>=0 && xpos<TEMPWIDTH && ypos<TEMPHEIGHT)
    {
        return 1;
    }
    return 0;
}

void move_pawn(int ypos,int xpos,int ydest,int xdest,data_values **Map,data_save save)//Moves a unit (TODO: see if Map works and if it does if its not too laggy)
{
    strcpy(Map[ydest][xdest].team,Map[ypos][xpos].team);
    Map[ydest][xdest].entity=Map[ypos][xpos].entity;
    Map[ydest][xdest].carrying_flag=Map[ypos][xpos].carrying_flag;
    strcpy(Map[ypos][xpos].team," ");
    Map[ypos][xpos].entity=' ';
    Map[ypos][xpos].carrying_flag=0;
    for(int k=ydest-1;k<=ydest+1;k++)
    {
        for(int l=xdest-1;l<=xdest+1;l++)
        {
            if (k>=0 && k<&save.line && l>=0 && l<&save.column && Map[k][l].fog!=1 && Map[k][l].fog!=3 && strcmp(Map[ydest][xdest].team,"red")==0)
            {
                Map[k][l].fog=Map[k][l].fog+1;
            }
            else if (k>=0 && k<&save.line && l>=0 && l<&save.column && Map[k][l].fog!=1 && Map[k][l].fog!=3 && strcmp(Map[ydest][xdest].team,"blue")==0)
                Map[k][l].fog=Map[k][l].fog+2;
        }
    }
}
