#include <windows.h>
#include <stdlib.h>
#include <time.h>

#include "files.h"
#include "game.h"
#include "display.h"

/*All functions relative to the display*/

void color(int t,int b) //Function to use coloration. First number is text, second background
{
    HANDLE H=GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(H,b*16+t);
}

void interface_game(int y,data_values **Map,data_save *save,selection *cursor, int *mov_pts) //Interface while playing
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
        case 'A':
            printf(" Archer: ");
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
    printf("     -=BASICS=-\n\n Each team has the same number of INFANTRY, SCOUTS, and TROOPERS. \n They also each have a FLAG they must protect while trying to capture the enemy FLAG.\n at the start of the game players can decide where to place their FLAG, base and spawn or can let the game chose it randomly\n\n");
    printf("     -=MOVEMENT=-\n\n Every turn a player can select the pawns they want to move with the ARROW KEYS & move them using the NUMPAD.\n To end their turn players must press the 0 key.\n Each player can move their pawns until they run out of MOVEMENT POINTS.\n MOVEMENT POINTS are distributed as follows: \n  - 5 for SCOUTS (S)\n  - 4 for ARCHERS (A)\n  - 3 for INFANTRY (I)\n  - 2 for TROOPERS (T)\n\n Every time a pawn moves, it loses movement points for the tile it is leaving as follows:\n  - 1 point to leave a GRASS (green) tile\n  - 2 points to leave a FORREST (+) tile\n  - ALL points to leave a RIVER (blue) tile, a pawn can only leave a RIVER tile if it has all its movement points\n  - WALL (#) tiles are custom tiles unique to our game. Pawns cannot move to WALL tiles\n\n At the beginning of the game, there is a FOG OF WAR on the entire terrain.\n Players reveal terrain by moving their pawns, revealing the 8 tiles surrounding each pawn.\n Additionally a player can only see an enemy pawn if it is in revealed terrain and in a small radius around a friendly pawn.\n\n ");
    printf("     -=COMBAT=-\n\n When two pawns are adjacent, combat begins.\n Each pawn reacts differently to combat:\n  - SCOUTS are the weakest and are sent back to spawn when facing INFANTRY or TROOPERS\n  - INFANTRY sends SCOUTS and ARCHERS back to spawn and are sent back to spawn by TROOPERS\n    When two INFANTRY face off, one is chosen randomly and is sent back to spawn.\n  - TROOPERS are the strongest and send both SCOUTS, ARCHERS and INFANTRY back to spawn\n    When two TROOPERS face off they push each other and if something blocks them from moving back, they are sent back to spawn\n  - ARCHERS are a special custom pawn we made. ARCHERS are beaten by TROOPERS and INFANTRY if they are in surrounding tiles\n    HOWEVER, ACHERS can shoot at enemy pawns that are two tiles away in cardinal directions as long as these pawns are not in fog of war\n    When shot by an archer, INFANTRY and SCOUTS are sent back to spawn. TROOPERS however are pushed two tiles back and if something blocks their movement, they are also sent back to spawn.\n    ARCHERS cannot shoot over walls.\n\n After a pawn loses combat and is sent back to spawn, it is penalized and is not able to move for one turn\n\n");
    printf("     -=OBJECTIVE=-\n\n To win the game, a player must grab the enemy FLAG with a pawn and take it back to his flag to capture it.\n To grab the FLAG, a TROOPER, INFANTRY or ARCHER pawn must be adjacent to the enemy FLAG pawn.\n While holding the FLAG, pawns keep the same movement points but behave like SCOUTS in combat.\n If a pawn carrying the FLAG is defeated in combat, it is sent back to spawn and leaves the flag on a tile adjacent to where he was defeated, to be picked up by another pawn.\n If a player manages to carry the enemy FLAG all the way to their FLAG in their base, The flag is captured and the player wins the game!\n\n         HAVE FUN!");
    userinput();
}

void pre_display(data_values **Map, data_save *save)//Function to choose the spawns, and the display relative to it
{
    int pcount,pdown=0,color_b,done=1,previous_x=-1,previous_y=-1,possible;
    char team[6]="red", temp[15];
    char choice=' ',key_input;
    selection cursor;
    pcount=strlen(save->pawns);
    cursor.x=cursor.y=2;
    do
    {
        printf("Do you want to choose the spawn point, or let it be random? (c/r) ");
        scanf(" %c",&choice);
    }while(choice!='r' && choice!='c');
    do
    {
        if (choice=='c')
        {
            system("cls");
            possible=1;
            for (int i=0;i<save->line;i++) //Display of the map
            {
                for(int j=0;j<save->column;j++)
                {
                    if(strcmp(Map[i][j].terrain,"grass")==0 || strcmp(Map[i][j].terrain,"tree")==0|| strcmp(Map[i][j].terrain,"wall")==0 || strcmp(Map[i][j].terrain,"spawn_r")==0 || strcmp(Map[i][j].terrain,"spawn_b")==0 ) //Giving the color green for grass and tree
                    {
                        color_b = 10;//Green
                                                            }
                    if(strcmp(Map[i][j].terrain,"water")==0) //Color blue for water
                    {
                        color_b = 11;//Cyan
                    }
                    if(strcmp(Map[i][j].terrain,"check_for_b")==0 || strcmp(Map[i][j].terrain,"check_for_r")==0) //Color for zone of drop
                    {
                        color_b = 10;//Green
                    }
                    if(i==cursor.y && j==cursor.x)
                    {
                        color_b=15;
                        if((cursor.y>save->line-3) || (cursor.y<2) || (cursor.x>save->column-3) || (cursor.x<2) || strcmp(Map[i][j].terrain,"wall")==0)
                        {
                            color_b=12;
                            possible=0;
                        }
                        if(previous_x!=-1 && (cursor.x<=previous_x+2+2 && cursor.x>=previous_x-2-2 && cursor.y<=previous_y+2+2 && cursor.y>=previous_y-2-2))
                        {
                            color_b=12;
                            possible=0;
                        }
                    }

                    if(strcmp(Map[i][j].terrain,"grass")==0 || strcmp(Map[i][j].terrain,"water")==0 || strcmp(Map[i][j].terrain,"check_for_b")==0 || strcmp(Map[i][j].terrain,"check_for_r")==0 || strcmp(Map[i][j].terrain,"spawn_b")==0 || strcmp(Map[i][j].terrain,"spawn_r")==0)
                    {
                        color(15,color_b);
                        printf(" ");
                    }
                    else if(strcmp(Map[i][j].terrain,"tree")==0)
                    {
                        color(4,color_b);
                        printf("+");
                    }
                    else if(strcmp(Map[i][j].terrain,"wall")==0)
                    {
                        color(0,color_b);
                        printf("#");
                    }

}
                color(15,0);
                switch(i)
                {
                    case 0:
                        printf(" Currently choosing: %s",team);
                        break;
                    case 2:
                        printf(" Move the cursor with the arrow keys");
                        break;
                    case 3:
                        printf(" When the cursor is white, affirm your choice");
                        break;
                    case 5:
                        printf("  by pressing the space bar");
                        break;
                }
                color(0,0);
                printf("\n");
            }
            key_input=userinput();
        }
        else
        {
            if(previous_x==-1) //Set red spawn in random mode
            {
                srand(time(NULL));
                int band=rand()%4;
                if(band==0)
                    cursor.y=2;
                else if(band==2)
                    cursor.y=save->line-3;
                else if(band==1)
                    cursor.x=2;
                else if(band==3)
                    cursor.x=save->column-3;
                if(band==0 || band==2)
                {
                    do
                    {
                        cursor.x=rand()%(save->column-2)+2;
                    }while(cursor.x<2 || cursor.x>save->column-3);
                }
                else if(band==1 || band==3)
                {
                    do
                    {
                        cursor.y=rand()%(save->line-2)+2;
                    }while(cursor.y<2 || cursor.y>save->line-3);
                }
            }
            else //In random, the position of the 2 spawns are symetric
            {
                cursor.x=save->column-previous_x-1;
                cursor.y=save->line-previous_y-1;
            }

            key_input=' ';
        }
        switch(key_input)
        {
            case '<':
                if(cursor.x!=0)
                    cursor.x--;
                break;
            case '>':
                if(cursor.x!=save->column-1)
                    cursor.x++;
                break;
            case '^':
                if(cursor.y!=0)
                    cursor.y--;
                break;
            case '_':
                if(cursor.y!=save->line-1)
                    cursor.y++;
                break;
            case ' ':
                if(possible) //When the user want to choose its spawn
                {
                    if(strcmp(team,"blue")==0)
                        done=0;
                    sprintf(&temp,"check_for_%c",team[0]); //Generation of checks
                    if(strcmp(team,"red")==0)
                    {
                        deleteWall(Map[cursor.y-2][cursor.x-2].terrain);
                        Map[cursor.y-2][cursor.x-2].entity='F';
                        Map[cursor.y-2][cursor.x-2].id=-1;
                        strcpy(Map[cursor.y-2][cursor.x-2].team,team);
                        strcpy(Map[cursor.y-2][cursor.x-2].terrain,temp);
                    }
                    else
                    {
                        deleteWall(Map[cursor.y+2][cursor.x+2].terrain);
                        Map[cursor.y+2][cursor.x+2].entity='F';
                        Map[cursor.y+2][cursor.x+2].id=-1;
                        strcpy(Map[cursor.y+2][cursor.x+2].team,team);
                        strcpy(Map[cursor.y+2][cursor.x+2].terrain,temp);
                    }
                    sprintf(&temp,"spawn_%c",team[0]);
                    strcpy(Map[cursor.y][cursor.x].terrain,temp); //Generation of spawn

                    int i,j;//Generation of pawns

                    int pdone=0,a=0,b=0;
                    while(pdone<pcount)
                    {
                            i = cursor.y-1+b;
                            j = cursor.x-1+a;
                            deleteWall(Map[i][j].terrain);
                            Map[i][j].entity=save->pawns[pdone];
                            Map[i][j].id=pdone;
                            int bl=Map[i][j].id;
                            strcpy(Map[i][j].team,team);
                            for(int k=i-1;k<=i+1;k++)
                                for(int l=j-1;l<=j+1;l++)
                                {
                                    if (posexist(k,l,save->column,save->line)==1)
                                    {
                                        if(strcmp(team,"red")==0 && Map[k][l].fog!=1 && Map[k][l].fog!=3)
                                        {
                                            if(Map[k][l].fog==2)
                                                Map[k][l].fog=3;
                                            else
                                               Map[k][l].fog=1;
                                        }
                                        if (strcmp(team,"blue")==0 && Map[k][l].fog!=2 && Map[k][l].fog!=3)
                                        {
                                            if(Map[k][l].fog==1)
                                                Map[k][l].fog=3;
                                            else
                                               Map[k][l].fog=2;
                                        }
                                        visibility_change('+',Map,i,j,k,l);
                                    }
                                }
                            for(int k=i-2;k<=i+2;k=k+4)
                            {
                                if (posexist(k,j,save->column,save->line)==1)
                                {
                                    visibility_change('+',Map,i,j,k,j);
                                }
                            }
                            for(int l=j-2;l<=j+2;l=l+4)
                            {
                                if (posexist(i,l,save->column,save->line)==1)
                                {
                                    visibility_change('+',Map,i,j,i,l);
                                }
                            }
                            pdone++;
                            a++;
                            if(a>2)
                            {
                                a-=3;
                                b++;
                            }
                    }
                    if(choice=='c')
                    {
                        system("cls");
                        color(15,0);
                        printf("\n\n\n      Placement of the team %s done",team);
                        getch();
                    }
                    strcpy(&team,"blue");
                    previous_x=cursor.x;
                    previous_y=cursor.y;
                    cursor.x=cursor.y=2;
                }
                break;
        }
    }while(done);
    save->nb_pawn=pcount;
}

void deleteWall(char * terrain[15])
{
    if(strcmp(terrain,"wall")==0)
    {
        strcpy(terrain,"grass");
    }
}
