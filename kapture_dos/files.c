#include <stdlib.h>
#include <stdio.h>
#include "files.h"

data_values ** getMap(int nb_map, data_save *save)
{
    data_values **Map;
    int i, j,nb_pawn_b=0,nb_pawn_r=0;
    char map_path[30],temp[15];
    sprintf(&map_path,"maps/map%d.txt",nb_map);
    FILE * map_file = NULL;
    map_file = fopen(map_path,"r");
    if (map_file==NULL)
    {
        printf("NULL_");
        return NULL;
    }
    int line,column;
    fscanf(map_file,"COLUMN= %d,LINE= %d\n",&column,&line);
    (*save).column = column;
    (*save).line = line;
    Map=(data_values **) malloc((*save).line*sizeof(data_values *));
    for(i=0;i<(*save).line;i++)
    {
        Map[i]=(data_values *) malloc((*save).column*sizeof(data_values));
    }
    for(i=0;i<(*save).line;i++)
    {
        for(j=0;j<(*save).column;j++)
        {
            fscanf(map_file,"%[^,]",&temp);//Get chars until comma
            fseek(map_file,1,SEEK_CUR); //Moving the cursor to the next position
            if (strcmp(temp,"grass")==0 || strcmp(temp,"tree")==0 || strcmp(temp,"water")==0 || strcmp(temp,"check_for_b")==0 || strcmp(temp,"check_for_r")==0 || strcmp(temp,"spawn_r")==0 || strcmp(temp,"spawn_b")==0)
            {
                strcpy(Map[i][j].terrain,temp);
                Map[i][j].entity = ' ';
                strcpy(Map[i][j].team," ");
                if(Map[i][j].fog!=1 && Map[i][j].fog!=2 && Map[i][j].fog!=3)
                    Map[i][j].fog=0;
            }
            else
            {
                strcpy(Map[i][j].terrain,"grass");
                if(temp[0]=='F' || temp[0]=='I' || temp[0]=='S' || temp[0]=='T')
                {
                    Map[i][j].entity = temp[0];
                    if (temp[1]=='R')
                    {
                        strcpy(Map[i][j].team,"red");
                        if(temp[0]!='F')
                            nb_pawn_r++;
                        for(int k=i-1;k<=i+1;k++)
                            for(int l=j-1;l<=j+1;l++)
                            {
                                if (k>=0 && k< (*save).line && l>=0 && l< (*save).column && (Map[k][l].fog!=1 && Map[k][l].fog!=3))
                                {
                                    if(Map[k][l].fog==2)
                                        Map[k][l].fog=3;
                                    else
                                       Map[k][l].fog=1;
                                }
                            }
                    }
                    else if (temp[1]=='B')
                    {
                        strcpy(Map[i][j].team,"blue");
                        if(temp[0]!='F')
                        {
                            nb_pawn_b++;
                        }
                        for(int k=i-1;k<=i+1;k++)
                            for(int l=j-1;l<=j+1;l++)
                            {
                                if (k>=0 && k< (*save).line && l>=0 && l<= (*save).column && (Map[k][l].fog!=2 && Map[k][l].fog!=3))
                                {
                                    if(Map[k][l].fog==1)
                                        Map[k][l].fog=3;
                                    else
                                       Map[k][l].fog=2;
                                }
                            }
                    }
                    else
                        Map[i][j].entity = ' ';
                }
                else
                {
                    Map[i][j].entity = ' ';
                }
            }
        }
        fscanf(map_file,"%\n",NULL); //Jump to the next line
    }
    if(nb_pawn_b==nb_pawn_r) //Check that both team have same nb of pawns (no matter theirs types)
        (*save).nb_pawn = nb_pawn_b;
    else
        return NULL;
    fclose(map_file);
    return Map;
}
