#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h> //Remove once project finished
#include <conio.h>
#include <time.h>

#include "files.h"
#include "game.h"
#include "display.h"

int main()
{
    data_values **Map;
    data_save save;
    spawn_coord spawn;
    flag_coord flags;
    char input_user,error[50],save_name[15];
    strcpy(error," ");
    int nb_map,color_b, game, cost,p,i,j,c,d;
    int stay=1; //Var to know when the user want to stop
    while(stay)
    {
        //Menu
        do
        {
            system("cls");
            printf("\n\n\n");
            printf("    1: New Game\n");
            printf("    2: Load Save\n");
            printf("    3: Rules\n");
            printf("    4: Quit\n");
            input_user = userinput();
        }while(input_user!='1' && input_user!='2' && input_user!='3'&& input_user!='4');

        switch(input_user)
        {
        case '1':
            do
            {
                system("cls");
                p=displayDirectory("./maps");
                scanf("%d",&nb_map);
            }while (nb_map>p);
            Map = getMap(nb_map,&save);
            pre_display(Map,&save);
            strcpy(save.team,"red"); //Init of the save in the RAM
            break;
        case '2':
            system("cls");
            p=displayDirectory("./saves");
            printf("\nEnter the file name, without the '.bin':\n");
            scanf("%s",&save_name);
            Map=loadFile(&save,&error,save_name);
            break;
        case '3':
            rules_display();
            break;
        case '4':
            stay = 0;
            break;
        }

        //Game in itself
        if(input_user=='1' || input_user=='2')
        {
            if (Map==NULL)
            {
                printf("An error occured while loading the map.\n");
            }
            else
            {
                selection cursor;
                int *mov_point;
                mov_point = (int *) malloc(save.nb_pawn*sizeof(int));
                int game = 1; //For the while loop
                save.turn = 1; //Same
                int turn,visibility;
                char key;
                do //Loop while the game ain't finished
                {
                    cursor.n = cursor.x = cursor.y =-1;
                    int init_mov_point=1;
                    turn=1;
                    int pawn=0;
                    printf("Init done\n");
                    //Start displaying map
                    do //Loop while the player has not finished to play
                    {
                        system("cls");
                        printf("\n\n");

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
                                        printf("%d,%d:%c;%d\n",i,j,Map[i][j].entity,Map[i][j].id);
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
                                if (Map[i][j].entity=='F')
                                {
                                    if (strcmp(Map[i][j].team,"blue")==0)
                                    {
                                        flags.bx=j;
                                        flags.by=i;
                                    }
                                    else if (strcmp(Map[i][j].team,"red")==0)
                                    {
                                        flags.rx=j;
                                        flags.ry=i;
                                    }
                                }
                                if(init_mov_point==1 && strcmp(Map[i][j].terrain,"spawn_r")==0)
                                {
                                    spawn.ry=i;
                                    spawn.rx=j;
                                }
                                else if(init_mov_point==1 && strcmp(Map[i][j].terrain,"spawn_b")==0)
                                {
                                    spawn.by=i;
                                    spawn.bx=j;
                                }
                            }
                        }

                        init_mov_point=0;
                        printf("Pre-loop done\n");

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
                                        if(strcmp(Map[i][j].carrying_flag," ")!=0)
                                        {
                                            color(13,color_b);
                                        }
                                        else
                                        {
                                            if(strcmp(Map[i][j].team,"red")==0)
                                            {
                                                color(12,color_b);
                                            }
                                            else if(strcmp(Map[i][j].team,"blue")==0)
                                            {
                                                color(9,color_b);
                                            }
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

                        printf("b'%d,%d'\n",flags.by,flags.bx);
                        printf("r'%d,%d'\n",flags.ry,flags.rx);
                        printf("Score:%d\n",save.nb_flag);
                        printf("Display done\n");
                        //End of display!
                        color(7,0);
                        key = userinput(); //Getting input from user
                        printf("Key got\n");
                        if(key=='0')
                        {
                            turn=0;
                            printf("Set 0\n");
                        }
                        else if(key=='q')
                        {
                            turn=0;
                            game=0;
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
                        }
                        printf("Before end of turn\n");
                    }while(turn!=0);
                    printf("After end turn\n");
                    if(game)
                    {
                        if(strcmp(save.team,"red")==0)
                        {
                            strcpy(save.team,"blue");
                        }
                        else if(strcmp(save.team,"blue")==0)
                        {
                            strcpy(save.team,"red");
                            save.turn++;
                        }

                        //COMBAT and capturing flags
                        system("cls");
                        for (i=0;i<save.line;i++)
                        {
                            for(j=0;j<save.column;j++)
                            {
                                if(strcmp(Map[i][j].terrain,"check_for_r")==0 && strcmp(Map[i][j].carrying_flag," ")!=0)
                                {
                                    char temp[6];
                                    strcpy(&temp,Map[i][j].carrying_flag);
                                    respawn(i,j,&spawn,Map,&save);
                                    strcpy(Map[i][j].team,temp);
                                    Map[i][j].entity='F';
                                    save.nb_flag++;
                                    flags.rx=j;
                                    flags.ry=i;
                                }
                                else if(strcmp(Map[i][j].terrain,"check_for_b")==0 && strcmp(Map[i][j].carrying_flag," ")!=0)
                                {
                                    char temp[6];
                                    strcpy(&temp,Map[i][j].carrying_flag);
                                    respawn(i,j,&spawn,Map,&save);
                                    strcpy(Map[i][j].team,temp);
                                    Map[i][j].entity='F';
                                    save.nb_flag--;
                                    flags.bx=j;
                                    flags.by=i;
                                }

                                if(Map[i][j].entity!=' ' && Map[i][j].entity!='F')
                                {

                                    for(c=-1;c<2;c++)
                                    {
                                        for(d=-1;d<2;d++)
                                        {
                                            if(posexist(i+c,j+d,save.line,save.column))
                                            {
                                                if(Map[i+c][j+d].entity!=' ' && (strcmp(Map[i][j].team,Map[i+c][j+d].team)!=0 || Map[i+c][j+d].entity=='F')  && Map[i][j].entity!=' ')
                                                {

                                                    if(strcmp(Map[i][j].carrying_flag," ")==0)
                                                    {
                                                        switch(Map[i][j].entity)
                                                        {
                                                            case 'S':
                                                                if((Map[i+c][j+d].entity=='I' || Map[i+c][j+d].entity=='T') && strcmp(Map[i+c][j+d].carrying_flag," ")==0){
                                                                        printf("%s Scout was defeated by",Map[i][j].team);
                                                                        print_pawn(Map[i+c][j+d].entity,Map[i+c][j+d].team);
                                                                        respawn(i,j,&spawn,Map,&save);
                                                                }
                                                                break;


                                                            case 'I':               //Maybe change I to C or something so it's more unique and different to T | TITITITITIITITIT <-- hard to see?
                                                                printf("%s Infantry ",Map[i][j].team);
                                                                if(strcmp(Map[i+c][j+d].carrying_flag," ")==0)
                                                                {

                                                                    switch(Map[i+c][j+d].entity)
                                                                    {
                                                                        case 'S':
                                                                            printf("defeated %s Scout",Map[i+c][j+d].team);
                                                                            respawn(i+c,j+d,&spawn,Map,&save);
                                                                            break;

                                                                        case 'I':
                                                                            if(rand()%2==0)
                                                                            {
                                                                                printf("was defeated by ");
                                                                                respawn(i,j,&spawn,Map,&save);
                                                                            }
                                                                            else
                                                                            {
                                                                                printf("defeated ");
                                                                                respawn(i+c,j+d,&spawn,Map,&save);
                                                                            }
                                                                            printf("%s Infantry",Map[i+c][j+d].team);
                                                                            break;

                                                                        case 'T':
                                                                            printf("was defeated by %s Trooper",Map[i+c][j+d].team);
                                                                            respawn(i,j,&spawn,Map,&save);
                                                                            break;

                                                                        case 'F':
                                                                            printf("grabbed %s FLAG!",Map[i+c][j+d].team);
                                                                            Map[i+c][j+d].entity=' ';
                                                                            strcpy(Map[i][j].carrying_flag,Map[i+c][j+d].team);
                                                                            strcpy(Map[i+c][j+d].team," ");
                                                                            Map[i+c][j+d].id=NULL;
                                                                            break;
                                                                    }
                                                                }
                                                                else{
                                                                    printf("defeated ");
                                                                    print_pawn(Map[i+c][j+d].entity,Map[i+c][j+d].team);
                                                                    printf(" and made them drop their flag!");
                                                                    respawn(i+c,j+d,&spawn,Map,&save);
                                                                }
                                                                break;

                                                            case 'T':
                                                                printf("%s Trooper ",Map[i][j].team);
                                                                if(strcmp(Map[i+c][j+d].carrying_flag," ")==0){
                                                                    switch(Map[i+c][j+d].entity) {
                                                                        case 'S':
                                                                            printf("defeated %s Scout",Map[i+c][j+d].team);
                                                                            respawn(i+c,j+d,&spawn,Map,&save);
                                                                            break;

                                                                        case 'I':
                                                                            printf("defeated %s Infantry",Map[i+c][j+d].team);
                                                                            respawn(i+c,j+d,&spawn,Map,&save);
                                                                            break;

                                                                        case 'T':
                                                                            printf("fought %s Trooper",Map[i+c][j+d].team);
                                                                            if(move_pawn(i,j,i-c,j-d,Map,&save)==1)
                                                                            {
                                                                                respawn(i,j,&spawn,Map,&save);
                                                                            }
                                                                            if(move_pawn(i+c,j+d,i+c+c,j+d+d,Map,&save)==1)
                                                                            {
                                                                                respawn(i+c,j+d,&spawn,Map,&save);
                                                                            }

                                                                            break;

                                                                        case 'F':
                                                                            printf("grabbed %s FLAG!",Map[i+c][j+d].team);
                                                                            Map[i+c][j+d].entity=' ';
                                                                            strcpy(Map[i][j].carrying_flag,Map[i+c][j+d].team);
                                                                            strcpy(Map[i+c][j+d].team," ");
                                                                            Map[i+c][j+d].id=NULL;
                                                                            break;
                                                                    }
                                                                }
                                                                else{
                                                                    printf("defeated ");
                                                                    print_pawn(Map[i+c][j+d].entity,Map[i+c][j+d].team);
                                                                    printf(" and made them drop their flag!");
                                                                    respawn(i+c,j+d,&spawn,Map,&save);
                                                                }
                                                                break;
                                                        }
                                                    }
                                                    else
                                                    {
                                                        if(Map[i+c][j+d].entity=='I' || Map[i+c][j+d].entity=='T')
                                                        {
                                                            print_pawn(Map[i][j].entity,Map[i][j].team);
                                                            printf(" was defeated by ");
                                                            print_pawn(Map[i+c][j+d].entity,Map[i+c][j+d].team);
                                                            printf(" and dropped their flag!");
                                                            respawn(i,j,&spawn,Map,&save);
                                                        }
                                                    }
                                                    printf("\n");
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }

                        //Flag capture
                        i=flags.ry-1;
                        while (flags.ry!=-1 && i<=flags.ry+1)
                        {
                            j=flags.rx-1;
                            while (flags.rx!=-1 && j<=flags.rx+1)
                            {
                                if(posexist(i,j,save.column,save.line)==1 && (Map[i][j].entity=='I' || Map[i][j].entity=='T')&& strcmp(Map[i][j].carrying_flag," ")==0)
                                {
                                    if(Map[i][j].team[0]!=Map[flags.ry][flags.rx].terrain[10]) //Red team can't take a flag from one of their drop
                                    {
                                        strcpy(Map[i][j].carrying_flag,"red");
                                        strcpy(Map[flags.ry][flags.rx].team," ");
                                        Map[flags.ry][flags.rx].id=NULL;
                                        Map[flags.ry][flags.rx].entity=' ';
                                        if(strcmp(Map[flags.ry][flags.rx].terrain,"check_for_r")==0)
                                        {
                                            save.nb_flag--;
                                        }
                                        flags.ry = flags.rx = -1;
                                    }
                                }
                                j++;
                            }
                            i++;
                        }
                        i=flags.by-1;
                        while (flags.by!=-1 && i<=flags.by+1)
                        {
                            j=flags.bx-1;
                            while (flags.bx!=-1 && j<=flags.bx+1)
                            {
                                if(posexist(i,j,save.column,save.line)==1 && (Map[i][j].entity=='I' || Map[i][j].entity=='T')&& strcmp(Map[i][j].carrying_flag," ")==0)
                                {
                                    if(Map[i][j].team[0]!=Map[flags.by][flags.bx].terrain[10]) //Blue team can't take a flag from one of their drop
                                    {
                                        strcpy(Map[i][j].carrying_flag,"blue");
                                        strcpy(Map[flags.by][flags.bx].team," ");
                                        Map[flags.by][flags.bx].id=NULL;
                                        Map[flags.by][flags.bx].entity=' ';
                                        if(strcmp(Map[flags.by][flags.bx].terrain,"check_for_b")==0)
                                        {
                                            save.nb_flag++;
                                        }
                                        flags.by = flags.bx = -1;
                                    }
                                }
                                j++;
                            }
                            i++;
                        }

                        do //Loop to informe the users to change, and if they want, to save
                        {
                            input_user = ' '; //Reset of the var, because already used before
                            //system("cls");
                            printf("\n\n        OBJECTION. IT IS NOW TIME FOR PLAYER %s TO PLAY.\n\n (if you agreed on every registration term, please press any key)\n",save.team);
                            printf("\n\n\n        Press 'S' to save");
                            input_user = userinput();
                            printf("\n Black input received\n");
                        }while(input_user!=' ' && input_user!='s');
                        printf("Change screen done\n");
                        if(input_user=='s') //Procedure to save
                        {
                            do
                            {
                                printf("\nEnter a name for your save (overwrite existing file)(less than 10 characters)\n");
                                scanf("%s",save_name);
                            }while(strlen(save_name)>10);
                            if(saveFile(&save,Map,error,save_name)!=0)
                            {
                                printf("%s",error);
                                strcpy(error," ");
                            }
                        }
                    }
                    system("cls");
                    if(save.nb_flag==-2)
                    {
                        printf("\n\n\n      Congratulations to the blue who won!");
                        game=0;
                        system("pause");
                    }
                    else if (save.nb_flag==2)
                    {
                        printf("\n\n\n      Congratulations to the red who won!");
                        game=0;
                        system("pause");
                    }
                }while(game); //End of loop while the game ain't finished
                for(int i=0;i<save.line;i++)
                {
                    free(Map[i]);
                }
                free(Map);
                free(mov_point);
            }
        }
    }
    return 0;
}
