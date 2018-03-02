#include "files.h"
#include <string.h>
#include <errno.h>

data_values ** getMap(int nb_map)
{
    data_values **Map;
    int line, row, i, j;
    char map_path,temp[10];
    sprintf(&map_path,"maps/map%d.txt",nb_map);
    FILE * map_file = NULL;
    map_file = fopen(&map_path,"r");
    if (map_file==NULL)
    {
        printf("NULL");
        return NULL;
    }
    fscanf(map_file,"ROW= %d,LINE= %d\n",&row,&line);
    Map=(data_values **) malloc(line*sizeof(data_values *));
    for(i=0;i<line;i++)
    {
        Map[i]=(data_values *) malloc(row*sizeof(data_values));
    }
    long bl = ftell(map_file);
    for(i=0;i<line;i++)
    {
        for(j=0;j<row;j++)
        {
            fscanf(map_file,"%[^,]",&temp);//Get chars until comma
            fseek(map_file,1,SEEK_CUR);
            if (strcmp(temp,"grass")==0 || strcmp(temp,"tree")==0 || strcmp(temp,"water")==0)
            {
                strcpy(Map[i][j].terrain,temp);
                strcpy(Map[i][j].entity," ");
            }
            else
            {
                strcpy(Map[i][j].terrain,"grass");
                strcpy(Map[i][j].entity,temp);
            }
        }
        fscanf(map_file,"%\n",NULL); //Jump to the next line
    }
    fclose(map_file);
    return Map;
}
