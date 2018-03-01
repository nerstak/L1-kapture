#include "files.h"
#include <errno.h>

data_values ** getMap(int nb_map)
{
    data_values **Map;
    int line, row, i, j,temp;
    char map_path;
    /*sprintf(&map_path,"maps/map%d.txt",nb_map);
    printf("%s\n",&map_path);*/
    FILE * map_file = NULL;
    map_file = fopen("maps/map0.txt","r");
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
    for(i=0;i<line;i++)
    {
        for(j=0;j<row;j++)
        {
            fscanf(map_file,"%c,",&temp);
            if (temp!='.' || temp=='+' || temp=='ø')
            {
                Map[i][j].terrain = temp;
                Map[i][j].entity = ' ';
            }
            else
            {
                Map[i][j].terrain = '.';
                Map[i][j].entity = temp;
            }
            fscanf(map_file,"%\n",NULL); //Jump to the next line
        }
    }
    fclose(map_file);
    return Map;
}
