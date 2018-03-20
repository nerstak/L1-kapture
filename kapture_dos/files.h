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
        int death; //1 if the pawn has died in the last turn
    } data_values;

typedef struct data_save
    {
        int line; //Width of current map
        int column; //Height of current map
        int turn; //Number of the turn
        char team[6]; //Color of the team who will play
        int nb_pawn; //Nb of pawn on each team
        char pawns[10]; //Pawn config. e.g. 'ISST'
        char mapname[25]; //Name of the current map
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
