#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>

#include "files.h"

/*All functions concerning reading or writing a file, or data saved in RAM*/

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
    int line,column,id_red,id_blue;
    id_red=id_blue=0;
    fscanf(map_file,"COLUMN= %d,LINE= %d\n",&column,&line);
    save->column = column;
    save->line = line;
    Map=(data_values **) malloc(save->line*sizeof(data_values *));
    for(i=0;i<save->line;i++)
    {
        Map[i]=(data_values *) malloc(save->column*sizeof(data_values));
    }
    for(i=0;i<save->line;i++)
    {
        for(j=0;j<save->column;j++)
        {
            fscanf(map_file,"%[^,]",&temp);//Get chars until comma
            fseek(map_file,1,SEEK_CUR); //Moving the cursor to the next position
            if (strcmp(temp,"grass")==0 || strcmp(temp,"tree")==0 || strcmp(temp,"water")==0 || strcmp(temp,"check_for_b")==0 || strcmp(temp,"check_for_r")==0 || strcmp(temp,"spawn_r")==0 || strcmp(temp,"spawn_b")==0)
            {
                strcpy(Map[i][j].terrain,temp);
                Map[i][j].entity = ' ';
                strcpy(Map[i][j].team," ");
                strcpy(Map[i][j].carrying_flag," ");
                if(Map[i][j].fog!=1 && Map[i][j].fog!=2 && Map[i][j].fog!=3)
                    Map[i][j].fog=0;
                if(Map[i][j].visibility_blue>100)
                    Map[i][j].visibility_blue=0;
                if(Map[i][j].visibility_red>100)
                    Map[i][j].visibility_red=0;
            }
            else
            {
                strcpy(Map[i][j].terrain,"grass");
                if(temp[0]=='F' || temp[0]=='I' || temp[0]=='S' || temp[0]=='T')
                {
                    Map[i][j].entity = temp[0];
                    strcpy(Map[i][j].carrying_flag," ");
                    if (temp[1]=='R')
                    {
                        strcpy(Map[i][j].team,"red");
                        if(temp[0]!='F')
                        {
                            Map[i][j].id=id_red;
                            nb_pawn_r++;
                            id_red++;
                        }
                        else
                        {
                            Map[i][j].id=-1;
                        }
                        for(int k=i-1;k<=i+1;k++)
                            for(int l=j-1;l<=j+1;l++)
                            {
                                if (posexist(k,l,save->column,save->line)==1)
                                {
                                    if(Map[k][l].fog!=1 && Map[k][l].fog!=3)
                                    {
                                        if(Map[k][l].fog==2)
                                            Map[k][l].fog=3;
                                        else
                                           Map[k][l].fog=1;
                                    }
                                    if(Map[k][l].visibility_red>100)
                                        Map[k][l].visibility_red=0;
                                    Map[k][l].visibility_red++;
                                }
                            }
                        for(int k=i-2;k<=i+2;k=k+4)
                        {
                            if (posexist(k,j,save->column,save->line)==1)
                            {
                                if(Map[k][j].visibility_red>100)
                                    Map[k][j].visibility_red=0;
                                Map[k][j].visibility_red++;
                            }
                        }
                        for(int l=j-2;l<=j+2;l=l+4)
                        {
                            if (posexist(i,l,save->column,save->line)==1)
                            {
                                if(Map[i][l].visibility_red>100)
                                    Map[i][l].visibility_red=0;
                                Map[i][l].visibility_red++;
                            }
                        }
                    }
                    else if (temp[1]=='B')
                    {
                        strcpy(Map[i][j].team,"blue");
                        if(temp[0]!='F')
                        {
                            Map[i][j].id=id_blue;
                            nb_pawn_b++;
                            id_blue++;
                        }
                        else
                        {
                            Map[i][j].id=-1;
                        }
                        for(int k=i-1;k<=i+1;k++)
                            for(int l=j-1;l<=j+1;l++)
                            {
                                if (posexist(k,l,save->column,save->line)==1)
                                {
                                    if (Map[k][l].fog!=2 && Map[k][l].fog!=3)
                                    {
                                        if(Map[k][l].fog==1)
                                            Map[k][l].fog=3;
                                        else
                                           Map[k][l].fog=2;
                                    }
                                    if(Map[k][l].visibility_blue>100)
                                        Map[k][l].visibility_blue=0;
                                    Map[k][l].visibility_blue++;
                                }
                            }
                        for(int k=i-2;k<=i+2;k=k+4)
                        {
                            if (posexist(k,j,save->column,save->line)==1)
                            {
                                if(Map[k][j].visibility_blue>100)
                                    Map[k][j].visibility_blue=0;
                                Map[k][j].visibility_blue++;
                            }
                        }
                        for(int l=j-2;l<=j+2;l=l+4)
                        {
                            if (posexist(i,l,save->column,save->line)==1)
                            {
                                if(Map[i][l].visibility_blue>100)
                                    Map[i][l].visibility_blue=0;
                                Map[i][l].visibility_blue++;
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
        save->nb_pawn = nb_pawn_b;
    else
        return NULL;
    save->nb_flag=0; //Setting score
    fclose(map_file);
    return Map;
}

int saveFile(data_save *save,data_values **Map,char error[],char name[])
{
    FILE * file_save;
    char save_name[20];
    sprintf(&save_name,"saves/%s.bin",name);
    file_save = fopen(save_name , "wb+"); //Create or reset save file
    if (file_save==NULL)
    {
        error="Error during the creation of the save";
        return 1;
    }
    fwrite(save,sizeof(data_save),1,file_save);
    for(int i=0;i<save->line;i++)
    {
        fwrite(Map[i],sizeof(data_values),save->column,file_save);
    }
    fclose(file_save);
    return 0;
}

data_values ** loadFile(data_save *save,char error[],char name[])
{
    FILE * file_save;
    data_values **Map;
    char save_name[20];
    sprintf(&save_name,"saves/%s.bin",name);
    file_save = fopen(save_name , "rb");
    if (file_save==NULL)
    {
        error="Error during the loading of the save";
        return 1;
    }
    fread(save,sizeof(data_save),1,file_save);
    Map=(data_values **) malloc(save->line*sizeof(data_values *));
    for(int i=0;i<save->line;i++)
    {
        Map[i]=(data_values *) malloc(save->column*sizeof(data_values));
        fread(Map[i],sizeof(data_values),save->column,file_save);
    }
    fclose(file_save);
    return Map;
}

int displayDirectory(char directory[])
{
    DIR *d;
    struct dirent *dir;
    d = opendir(directory);
    int i = -1;
    if(d) //If directory exists
    {
        while((dir = readdir(d))!=NULL) //While there is unlisted files, it lists files
        {
            if(dir->d_name[0]!='.')
            {
                i++;
                printf("N%d: %s\n",i, dir->d_name);
            }
        }
    closedir(d);
    }
    return i;
}
