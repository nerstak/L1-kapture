#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

typedef struct selection
{
    int n;
    int id;
    int x;
    int y;
} selection;

void visibility_change(char sign,data_values **Map,int y_ori,int x_ori,int y_cell,int x_cell);

int move_pawn(int ypos,int xpos,int ydest,int xdest,data_values **Map,data_save save);

int posexist(int ypos,int xpos,int TEMPWIDTH,int TEMPHEIGHT);

char userinput();

int point_deduct(int ypos,int xpos,data_values **Map,int *mov_point);

int cost_terrain(int ypos,int xpos,data_values **Map);

int cursor_new_id(int y, data_save save, data_values **Map);

#endif // GAME_H_INCLUDED
