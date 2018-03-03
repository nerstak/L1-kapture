#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

typedef struct data_save
    {
        int line;
        int column;
        int turn; //Number of the turn
        char team[4]; //Color of the team who will play
    }data_save;

#endif // GAME_H_INCLUDED
