#include <windows.h>
#include <stdlib.h>

#include "files.h"
#include "game.h"
#include "display.h"

/*All functions relative to the display*/

void color(int t,int b) //Function to use coloration. First number is text, second background
{
    HANDLE H=GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(H,b*16+t);
}

void interface_game(int y,data_values **Map,data_save *save,selection *cursor, int *mov_pts)
{
    color(8,0);
    printf(" ");
    if (y==0)
    {
        printf("Player: %s  Turn: %d",save->team,save->turn);
    }
    else if(y==2)
    {
        printf("Remaining movement points:");
    }
    else if(y==3)
    {
        switch(Map[cursor->y][cursor->x].entity)
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
        printf("%d",mov_pts[cursor->id]);
    }
    else if(y==4)
    {
        printf("Cost to leave: %d",cost_terrain(cursor->y,cursor->x,Map));
    }
    else if(y==save->line-2)
    {
        printf("Press Q to leave game");
    }
    else if(y==save->line-1)
    {
        printf("Press 0 to end turn");
    }
}

void rules_display()
{
    system("cls");
    printf("     -=BASICS=-\n\n Each team has the same number of INFANTRY, SCOUTS, and TROOPERS. \n They also each have a FLAG they must protect while trying to capture the enemy FLAG.\n\n");
    printf("     -=MOVEMENT=-\n\n Every turn a player can select the pawns they want to move with the ARROW KEYS, move them using the NUMPAD.\n To end their turn players must press the 0 key.\n Each player can move their pawns until they run out of MOVEMENT POINTS.\n MOVEMENT POINTS are distributed as follows: \n  - 5 for SCOUTS\n  - 3 for INFANTRY\n  - 2 for TROOPERS\n\n Every time a pawn moves, it loses movement points for the tile it is leaving as follows:\n  - 1 point to leave a GRASS tile\n  - 2 points to leave a FORREST tile\n  - ALL points to leave a RIVER tile, a pawn can only leave a RIVER tile if it has all its movement points\n\n at the beggining of the game, there is a FOG OF WAR on the entire terrain.\n Players reveal terrain by moving their pawns, revealing the 8 tiles surrounding each pawn.\n Additionally a player can only see an enemy pawn if it is in revealed terrain and in a small radius around a friendly pawn.\n\n ");
    printf("     -=COMBAT=-\n\n When two pawns are adjacent, combat begins.\n Each pawn reacts differently to combat:\n  - SCOUTS are the weakest and are sent back to spawn when facing INFANTRY or TROOPERS\n  - INFANTRY sends SCOUTS back to spawn and are sent back to spawn by TROOPERS\n    When two INFANTRY face off, one is chosen randomly and is sent back to spawn.\n  - TROOPERS are the strongest and send both SCOUTS and INFANTRY back to spawn\n    When two TROOPERS face off they push each other and if something blocks them from moving back, they are sent back to spawn\n\n");
    printf("     -=OBJECTIVE=-\n\n The win the game, a player must grab the enemy FLAG with a pawn and take it back to his flag to capture it.\n To grab the FLAG, a TROOPER or INFANTRY pawn must be adjacent to the enemy FLAG pawn.\n While holding the FLAG, pawns keep the same movement points but behave like SCOUTS in combat.\n If a pawn carrying the FLAG is defeated in combat, it is sent back to spawn and leaves the flag where he died, to be picked up by another pawn.\n If a player manages to carry the enemy FLAG all the way to their FLAG in their base, The flag is captured and the player wins the game!\n\n         HAVE FUN!");
    userinput();
}
