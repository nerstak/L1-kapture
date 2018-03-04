#ifndef CHARACTER_MOVEMENT_H_INCLUDED
#define CHARACTER_MOVEMENT_H_INCLUDED

char userinput(); //Return the char corresponding to the key pressed

int posexist(int xpos,int ypos,int TEMPWIDTH,int TEMPHEIGHT);

void move(int ypos,int xpos,int ydest,int xdest,data_values **Map);

void move(int ypos,int xpos,int ydest,int xdest,data_values **Map,data_save save);



#endif