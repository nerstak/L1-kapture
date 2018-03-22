#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>

#include "files.h"
#include "game.h"

/*All functions relative to actions directly related to the game*/

char userinput()//Function to get the key pressed by the user
{
    switch(getch())
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
        case 27: //Keys up, down, left, right and other
            return userinput();
            break;
        case 72:
            return '^'; //Key up
            break;
        case 77:
            return '>'; //Key right
            break;
        case 75:
            return '<'; //Key left
            break;
        case 80:
            return '_'; //Key down
            break;
        case 113:
            return 'q';
            break;
        case 115:
            return 's';
            break;
        case 32:
            return ' '; //Key space
            break;
        defaut:
            return userinput();
            break;
    }
}

int posexist(int ypos,int xpos,int TEMPWIDTH,int TEMPHEIGHT) //checks if a coordinate is within the map's borders
{
    if(xpos>=0 && ypos>=0 && xpos<TEMPWIDTH && ypos<TEMPHEIGHT)
    {
        return 1;
    }
    return 0;
}

void visibility_change(char sign,data_values **Map,int y_ori,int x_ori,int y_cell,int x_cell) //Function to change visibility of tile
{
    if(strcmp(Map[y_ori][x_ori].team,"blue")==0)
    {
        if(sign=='+')
            Map[y_cell][x_cell].visibility_blue++;
        else if(sign=='-')
            Map[y_cell][x_cell].visibility_blue--;
    }
    if(strcmp(Map[y_ori][x_ori].team,"red")==0)
    {
        if(sign=='+')
            Map[y_cell][x_cell].visibility_red++;
        else if(sign=='-')
            Map[y_cell][x_cell].visibility_red--;
    }
}

int move_pawn(int ypos,int xpos,int ydest,int xdest,data_values **Map,data_save *save)//Moves a unit
{
    if(posexist(ydest,xdest,save->column,save->line)!=1)
    { //First we check if the movement is possible
        return 1;
    }
    if(Map[ydest][xdest].entity!=' ' || strcmp(Map[ydest][xdest].terrain,"wall")==0)
    {
        return 1;
    }
    for(int k=ypos-1;k<=ypos+1;k++) //Remove the visibility to all cell affected
    {
        for(int l=xpos-1;l<=xpos+1;l++)
        {
            if(posexist(k,l,save->column,save->line)==1)
            {
                visibility_change('-',Map,ypos,xpos,k,l);
            }
        }
    }
    for(int k=ypos-2;k<=ypos+2;k=k+4)
    {
        if (posexist(k,xpos,save->column,save->line)==1)
        {
            visibility_change('-',Map,ypos,xpos,k,xpos);
        }
    }
    for(int l=xpos-2;l<=xpos+2;l=l+4)
    {
        if (posexist(ypos,l,save->column,save->line)==1)
        {
            visibility_change('-',Map,ypos,xpos,ypos,l);
        }
    }
    strcpy(Map[ydest][xdest].team,Map[ypos][xpos].team); //Changing the team attribute
    strcpy(Map[ypos][xpos].team," ");
    Map[ydest][xdest].entity=Map[ypos][xpos].entity; //Moving the entity
    Map[ypos][xpos].entity=' ';
    strcpy(Map[ydest][xdest].carrying_flag,Map[ypos][xpos].carrying_flag); //Moving the carrying flag attribute
    strcpy(Map[ypos][xpos].carrying_flag," ");
    Map[ydest][xdest].id=Map[ypos][xpos].id; //Moving the id
    Map[ypos][xpos].id=0;
    for(int k=ydest-1;k<=ydest+1;k++) //Setting the fog of war and the visibility
    {
        for(int l=xdest-1;l<=xdest+1;l++)
        {
            if(posexist(k,l,save->column,save->line)==1)
            {
                if (Map[k][l].fog!=1 && Map[k][l].fog!=3 && strcmp(Map[ydest][xdest].team,"red")==0)
                {
                    Map[k][l].fog=Map[k][l].fog+1;
                }
                else if (Map[k][l].fog!=2 && Map[k][l].fog!=3 && strcmp(Map[ydest][xdest].team,"blue")==0)
                    Map[k][l].fog=Map[k][l].fog+2;
                visibility_change('+',Map,ydest,xdest,k,l);
            }
        }
    }
    for(int k=ydest-2;k<=ydest+2;k=k+4)//Part only for the visibility
    {
        if (posexist(k,xdest,save->column,save->line)==1)
        {
            visibility_change('+',Map,ydest,xdest,k,xdest);
        }
    }
    for(int l=xdest-2;l<=xdest+2;l=l+4) //Same, part only for the visibility
    {
        if (posexist(ydest,l,save->column,save->line)==1)
        {
            visibility_change('+',Map,ydest,xdest,ydest,l);
        }
    }
    return 0;
}

int point_deduct(int ypos,int xpos,data_values **Map,int *mov_point)//Function calculating points remaining after a movement
{
    return mov_point[Map[ypos][xpos].id]-cost_terrain(ypos,xpos,Map);
}

int cost_terrain(int ypos,int xpos,data_values **Map)//Function to get the cost for a unit to leave a case
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
                case 'A':
                    cost=4;
                    break;
            }
            break;
        default:
            cost=1;
            break;
    }
    return cost;
}

int cursor_new_id(int id, data_save *save, data_values **Map) //This function is usefull to keep the cursor on the pawn we're moving
{
    int cpt=0;
    for(int i=0;i<save->line;i++)
        for(int j=0;j<save->column;j++)
        {
            if(Map[i][j].entity != 'F' && strcmp(Map[i][j].team,save->team)==0)
            {
                cpt++;
                if(Map[i][j].id==id)
                    return cpt;
            }
        }
}

void respawn(int ypos,int xpos,spawn_coord *spawn,data_values **Map,data_save *save) //function to respawn pawns
{
    int ydest,xdest,i,j,h,y,x,end=0;
    char flag[6];
    if(strcmp(Map[ypos][xpos].team,"red")==0)
    {
        ydest=spawn->ry;
        xdest=spawn->rx;
    }
    else
    {
        ydest=spawn->by;
        xdest=spawn->bx;
    }
    strcpy(flag,Map[ypos][xpos].carrying_flag);
    strcpy(Map[ypos][xpos].carrying_flag," ");
    for(h=1;h<4;h++) //Loops to choose an empty cell around the spawn
    {

        for(i=-1;i<2;i++)
        {
            for(j=-1;j<2;j++)
            {
                if(move_pawn(ypos,xpos,ydest+(i*h),xdest+(j*h),Map,save)==0)
                {
                    Map[ydest+(i*h)][xdest+(j*h)].death=1;
                    if(strcmp(flag," ")!=0)
                    {
                        do
                        {
                            y=(rand()%3)-1;
                            x=(rand()%3)-1;
                            if(posexist(ypos+y,xpos+x,save->column,save->line) && Map[ypos+y][xpos+x].entity==' ' && (x!=0 || y!=0))
                            {
                                Map[ypos+y][xpos+x].entity='F';
                                strcpy(Map[ypos+y][xpos+x].team,flag);
                                end++;
                            }
                        }while(end==0);

                    }

                    return;
                }
            }
        }
    }
}

void print_pawn(char pawn,char team[6])
{
    printf("%s ",team);
    switch(pawn)
    {
        case 'T':
            printf("Trooper");
            break;
        case 'I':
            printf("Infantry");
            break;
        case 'S':
            printf("Scout");
            break;
        case 'F':
            printf("Flag");
            break;
        case 'A':
            printf("Archer");
            break;
    }
    return;
}

