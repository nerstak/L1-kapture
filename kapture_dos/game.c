#include <stdio.h>//TODO: replace the Map[][]=="x" to some fucking strcmp()   (and also fix all the struct refferences so they are consistent with Karsten's)
#include <stdlib.h>
#include <time.h>

#include "files.h"

/*All functions relative to actions directly related to the game*/

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

int move_pawn(int ypos,int xpos,int ydest,int xdest,data_values **Map,data_save save)//Moves a unit
{
    if(posexist(ydest,xdest,save.line,save.column)!=1 || Map[ydest][xdest].entity!=' ')
        return 1;
    strcpy(Map[ydest][xdest].team,Map[ypos][xpos].team);
    Map[ydest][xdest].entity=Map[ypos][xpos].entity;
    Map[ydest][xdest].carrying_flag=Map[ypos][xpos].carrying_flag;
    Map[ydest][xdest].id=Map[ypos][xpos].id;
    Map[ypos][xpos].id=0;
    strcpy(Map[ypos][xpos].team," ");
    Map[ypos][xpos].entity=' ';
    Map[ypos][xpos].carrying_flag=0;
    for(int k=ydest-1;k<=ydest+1;k++)
    {
        for(int l=xdest-1;l<=xdest+1;l++)
        {
            if (posexist(l,k,save.column,save.line)==1 && Map[k][l].fog!=1 && Map[k][l].fog!=3 && strcmp(Map[ydest][xdest].team,"red")==0)
            {
                Map[k][l].fog=Map[k][l].fog+1;
            }
            else if (posexist(l,k,save.column,save.line)==1 && Map[k][l].fog!=2 && Map[k][l].fog!=3 && strcmp(Map[ydest][xdest].team,"blue")==0)
                Map[k][l].fog=Map[k][l].fog+2;
        }
    }
    return 0;
}

int point_deduct(int ypos,int xpos,data_values **Map,int *mov_point)
{
    return mov_point[Map[ypos][xpos].id]-cost_terrain(ypos,xpos,Map);
}

int cost_terrain(int ypos,int xpos,data_values **Map)
{
    int cost;
    switch(Map[ypos][xpos].terrain[0]){
        case 't':
            cost=2;
            break;
        case 'w':
            switch(Map[ypos][xpos].entity)
            {
                case 'T':
                    cost=2;
                    break;
                case 'I':
                    cost=3;
                    break;
                case 'S':
                    cost=5;
                    break;
            }
            break;
        default:
            cost=1;
            break;
    }
    return cost;
}

int cursor_new_id(int id, data_save save, data_values **Map) //This function is usefull to keep the cursor on the pawn we're moving
{
    int cpt=0;
    for(int i=0;i<save.line;i++)
        for(int j=0;j<save.column;j++)
        {
            if(Map[i][j].entity != 'F' && strcmp(Map[i][j].team,save.team)==0)
            {
                cpt++;
                if(Map[i][j].id==id)
                    return cpt;
            }
        }
}
