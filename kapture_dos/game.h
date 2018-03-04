#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

void move_pawn(int ypos,int xpos,int ydest,int xdest,data_values **Map,data_save save);

int posexist(int xpos,int ypos,int TEMPWIDTH,int TEMPHEIGHT);

char userinput();

#endif // GAME_H_INCLUDED
