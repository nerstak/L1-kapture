#ifndef FILES_H_INCLUDED
#define FILES_H_INCLUDED

#include <string.h>
typedef struct data_values
    {
        char terrain[15]; //name of the terrain
        char entity; //name of the entity
        char team[6]; //name of team
        int visibility_red;  //+-1 according to entity that can see it, and prompt it if >0
        int visibility_blue;
        int fog; //+1 if red saw it,+2 if blue saw it, 0:none,1:red,2:blue,3:both
        char carrying_flag[6]; //r,b,' '
        int id; //Id of the pawn
    } data_values;

typedef struct data_save
    {
        int line;
        int column;
        int turn; //Number of the turn
        char team[4]; //Color of the team who will play
        int nb_pawn; //Nb of pawn on each team
        int nb_flag; //Positive : RED; Negative: BLUE. When one of them is |2|, team wins
        char pawns[10]; //Pawn config. e.g. 'ISST'
        char mapname[25]; //I'm sorry
    }data_save;

data_values ** getMap(int nb_map,data_save *save,char error[]);

int save_file(data_save *save,data_values **Map,char error[],char save_name[]);

data_values ** loadFile(data_save *save,char error[],char name[]);

int displayDirectory(char directory[]);

void getDirectory(char directory[],int n,data_save *save);

typedef struct spawn_coord
    {
        int by;
        int bx;
        int ry;
        int rx;
    } spawn_coord;

#endif // FILES_H_INCLUDED
