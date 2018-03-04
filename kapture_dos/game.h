#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

typedef struct selection
{
    int n;
    int id;
    int x;
    int y;
} selection;

int move_pawn(int ypos,int xpos,int ydest,int xdest,data_values **Map,data_save save);

int posexist(int xpos,int ypos,int TEMPWIDTH,int TEMPHEIGHT);

char userinput();

int point_deduct(int ypos,int xpos,data_values **Map,int *mov_point);

int cost_terrain(int ypos,int xpos,data_values **Map);

int cursor_new_id(int y, data_save save, data_values **Map);

#endif // GAME_H_INCLUDED
