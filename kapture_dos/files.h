#ifndef FILES_H_INCLUDED
#define FILES_H_INCLUDED

typedef struct data_values
    {
        char terrain[15]; //name of the terrain
        char entity; //name of the entity
        char team[4]; //name of team
        int visibility_red;  //+-1 according to entity that can see it, and prompt it if >0
        int visibility_blue;
        int fog; //+1 if red saw it,+2 if blue saw it, 0:none,1:red,2:blue,3:both
        int carrying_flag; //0 if no, 1 if yes
    } data_values;
data_values ** getMap(int nb_map,data_save *save);


#endif // FILES_H_INCLUDED
