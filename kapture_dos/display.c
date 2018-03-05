#include <windows.h>

#include "files.h"
#include "game.h"
#include "display.h"

/*All functions relative to the display*/

void color(int t,int b) //Function to use coloration. First number is text, second background
{
    HANDLE H=GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(H,b*16+t);
}

void interface_game(int y,data_values **Map,data_save save,selection cursor, int *mov_pts)
{
    color(8,0);
    printf(" ");
    if (y==0)
    {
        printf("Player: %s  Turn: %d",save.team,save.turn);
    }
    else if(y==2)
    {
        printf("Remaining movement points:");
    }
    else if(y==3)
    {
        switch(Map[cursor.y][cursor.x].entity)
        {
        case 'T':
            printf(" Shock Troop: ");
            break;
        case 'S':
            printf(" Scout: ");
            break;
        case 'I':
            printf(" Infantryman: ");
            break;
        }
        printf("%d",mov_pts[cursor.id]);
    }
    else if(y==4)
    {
        printf("Cost to leave: %d",cost_terrain(cursor.y,cursor.x,Map));
    }
}
