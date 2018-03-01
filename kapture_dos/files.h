#ifndef FILES_H_INCLUDED
#define FILES_H_INCLUDED

typedef struct data_values
    {
        signed char terrain;
        signed char entity;
        signed char team;
    } data_values;
data_values ** getMap(int nb_map);


#endif // FILES_H_INCLUDED
