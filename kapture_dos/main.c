#include <stdlib.h>
#include <stdio.h>
//#include <string.h>
#include <errno.h> //Remove once project finished
//#include <conio.h>

#include "files.h"
#include "game.h"
#include "display.h"

int main()
{
    data_values **Map;
    data_save save;
    spawn_coord spawn;
    int nb_map = 0,color_b, game, cost;
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
        selection cursor;
        int game = 1; //For the while loop
        strcpy(save.team,"red"); //Init of the save in the RAM
        save.turn = 1; //Same
        int turnend,pawn=0,visibility;
        char key;


        do
        {
            cursor.n = cursor.x = cursor.y =-1;
            int *mov_point=(int *)malloc(save.nb_pawn*sizeof(int));
            int init_mov_point=1;
            turnend=0;
            do
            {
                system("cls");
                for(int i=0;i<save.line;i++) //Getting right position of the cursor, and init the number of movement points
                {
                    for(int j=0;j<save.column;j++)
                    {
                        if(Map[i][j].entity != 'F' && strcmp(Map[i][j].team,save.team)==0) //Process for cursor, who only work for pawns of the team playing
                        {
                            pawn++;
                            if(cursor.n==-1) //If cursor not set
                            {
                                cursor.n=1;
                                cursor.id=Map[i][j].id;
                                cursor.x = j;
                                cursor.y = i;
                            }
                            if(pawn==cursor.n) //Changing the position of the cursor
                            {
                                cursor.x = j;
                                cursor.y = i;
                                cursor.id=Map[i][j].id;
                            }
                            if(init_mov_point==1) //We set up the array containing the number of mov's point remaining to each pawn (only when the player change)
                            {
                                switch(Map[i][j].entity)
                                {
                                    case 'T':
                                        mov_point[Map[i][j].id]=2;
                                        break;
                                    case 'I':
                                        mov_point[Map[i][j].id]=3;
                                        break;
                                    case 'S':
                                        mov_point[Map[i][j].id]=5;
                                        break;
                                }
                            }
                        }
                    }
                }
                for (int i=0;i<save.line;i++) //Display of the map
                {
                    for(int j=0;j<save.column;j++)
                    {
                        if(Map[i][j].visibility_blue>0 && strcmp(save.team,"blue")==0 || Map[i][j].visibility_red>0 && strcmp(save.team,"red")==0)
                            visibility=1;
                        else
                            visibility=0;
                        if(Map[i][j].fog==3 || (Map[i][j].fog==1 && strcmp(save.team,"red")==0) || (Map[i][j].fog==2 && strcmp(save.team,"blue")==0)) //First we set the color of the back
                        {
                            if(strcmp(Map[i][j].terrain,"grass")==0 || strcmp(Map[i][j].terrain,"tree")==0|| strcmp(Map[i][j].terrain,"spawn_r")==0 || strcmp(Map[i][j].terrain,"spawn_b")==0 ) //Giving the color green for grass and tree
                            {
                                color_b = 2+visibility*8;//Green
                                if(strcmp(Map[i][j].terrain,"spawn_r")==0)
                                {
                                    spawn.ry=i;
                                    spawn.rx=j;
                                }
                                else
                                {
                                    if(strcmp(Map[i][j].terrain,"spawn_b")==0)
                                    {
                                        spawn.by=i;
                                        spawn.bx=j;
                                    }
                                }
                            }
                            if(strcmp(Map[i][j].terrain,"water")==0) //Color blue for water
                            {
                                color_b = 3+visibility*8;//Cyan
                            }
                            if(strcmp(Map[i][j].terrain,"check_for_b")==0 || strcmp(Map[i][j].terrain,"check_for_r")==0) //Color for zone of drop
                            {
                                color_b = 6+visibility*8;//Yellow
                            }
                        }
                        else //Fog of war
                        {
                            color_b = 8;
                        }
                        if(Map[i][j].entity == ' ') //Display terrain without entity
                        {
                            if(color_b == 8 || strcmp(Map[i][j].terrain,"grass")==0 || strcmp(Map[i][j].terrain,"water")==0 || strcmp(Map[i][j].terrain,"check_for_b")==0 || strcmp(Map[i][j].terrain,"check_for_r")==0 || strcmp(Map[i][j].terrain,"spawn_b")==0 || strcmp(Map[i][j].terrain,"spawn_r")==0)
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
                                if(strcmp(Map[i][j].team,"red")==0)
                                {
                                    color(12,color_b);
                                }
                                else if(strcmp(Map[i][j].team,"blue")==0)
                                {
                                    color(9,color_b);
                                }
                                if(cursor.id==Map[i][j].id && strcmp(Map[i][j].team,save.team)==0)
                                    color(15,color_b);
                                if(visibility==1)
                                    printf("%c",Map[i][j].entity);
                                else
                                    printf(" ");
                            }
                            else //Fog of war display
                            {
                                color(15,color_b);
                                printf(" ");
                            }
                        }

                    }
                    interface_game(i,Map,&save,&cursor,mov_point);
                    color(0,0);
                    printf("\n");
                }
                color(7,0);
                key = userinput(); //Getting input from user
                if(key=='0')
                {
                    turnend=1;
                }
                else
                {
                    cost=point_deduct(cursor.y,cursor.x,Map,mov_point);
                    switch(key){
                    case '<':
                        if(cursor.n!=1)
                            cursor.n--;
                        break;
                    case '>':
                        if(cursor.n!=save.nb_pawn)
                            cursor.n++;
                        break;
                    case '9':
                        if(cost>=0){
                            if(move_pawn(cursor.y,cursor.x,cursor.y-1,cursor.x+1,Map,&save)==0)
                            {
                                cursor.y--;
                                cursor.x++;
                                cursor.n=cursor_new_id(cursor.id,&save,Map);
                                mov_point[Map[cursor.y][cursor.x].id]=cost;
                            }
                        }
                        break;
                    case '8':
                        if(cost>=0){
                            if(move_pawn(cursor.y,cursor.x,cursor.y-1,cursor.x,Map,&save)==0)
                            {
                                cursor.y--;
                                cursor.n=cursor_new_id(cursor.id,&save,Map);
                                mov_point[Map[cursor.y][cursor.x].id]=cost;
                            }
                        }
                        break;
                    case '7':
                        if(cost>=0){
                            if(move_pawn(cursor.y,cursor.x,cursor.y-1,cursor.x-1,Map,&save)==0)
                            {
                                cursor.y--;
                                cursor.x--;
                                cursor.n=cursor_new_id(cursor.id,&save,Map);
                                mov_point[Map[cursor.y][cursor.x].id]=cost;
                            }
                        }
                        break;
                    case '6':
                        if(cost>=0){
                            if(move_pawn(cursor.y,cursor.x,cursor.y,cursor.x+1,Map,&save)==0)
                            {
                                cursor.x++;
                                cursor.n=cursor_new_id(cursor.id,&save,Map);
                                mov_point[Map[cursor.y][cursor.x].id]=cost;
                            }
                        }
                        break;
                    case '4':
                        if(cost>=0){
                            //respawn(cursor.y,cursor.x,spawn,Map,&save);
                            if(move_pawn(cursor.y,cursor.x,cursor.y,cursor.x-1,Map,&save)==0)
                            {
                                cursor.x--;
                                cursor.n=cursor_new_id(cursor.id,&save,Map);
                                mov_point[Map[cursor.y][cursor.x].id]=cost;
                            }
                        }
                        break;
                    case '3':
                        if(cost>=0){
                            if(move_pawn(cursor.y,cursor.x,cursor.y+1,cursor.x+1,Map,&save)==0)
                            {
                                cursor.y++;
                                cursor.x++;
                                cursor.n=cursor_new_id(cursor.id,&save,Map);
                                mov_point[Map[cursor.y][cursor.x].id]=cost;
                            }
                        }
                        break;
                    case '2':
                        if(cost>=0){
                            if(move_pawn(cursor.y,cursor.x,cursor.y+1,cursor.x,Map,&save)==0)
                            {
                                cursor.y++;
                                cursor.n=cursor_new_id(cursor.id,&save,Map);
                                mov_point[Map[cursor.y][cursor.x].id]=cost;
                            }
                        }
                        break;
                    case '1':
                        if(cost>=0){
                            if(move_pawn(cursor.y,cursor.x,cursor.y+1,cursor.x-1,Map,&save)==0)
                            {
                                cursor.y++;
                                cursor.x--;
                                cursor.n=cursor_new_id(cursor.id,&save,Map);
                                mov_point[Map[cursor.y][cursor.x].id]=cost;
                            }
                        }
                        break;
                    }
                    pawn=0;
                    init_mov_point=0;
                }
            }while(turnend==0);
            if(strcmp(save.team,"red")==0)
            {
                strcpy(save.team,"blue");
            }
            else if(strcmp(save.team,"blue")==0)
            {
                strcpy(save.team,"red");
                save.turn++;
            }
            if(save.turn==100)
                game=0;

        }while(game);
    }
    return 0;
}
