#ifndef DISPLAY_H_INCLUDED
#define DISPLAY_H_INCLUDED

void color(int t,int b);

void interface_game(int y, data_values **Map, data_save *save, selection *cursor, int *mov_pts);

void rules_display();

void pre_display(data_values **Map, data_save *save);

void deleteWall(char *terrain[15]);
#endif // DISPLAY_H_INCLUDED
