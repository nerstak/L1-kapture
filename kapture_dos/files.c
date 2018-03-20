#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>

#include "files.h"

/*All functions concerning reading or writing a file, or data saved in RAM*/

data_values ** getMap(int nb_map, data_save *save,char error[])
{
    data_values **Map;
    int i, j;
    char map_path[30],temp[15],pawns[10];
    getDirectory("./maps",nb_map,save);
    sprintf(&map_path,"maps/%s",save->mapname);
    FILE * map_file = NULL;
    map_file = fopen(map_path,"r");
    if (map_file==NULL)
    {
        strcpy(error,"Unable to open the selected file");
        return NULL;
    }
    int line,column;
    fscanf(map_file,"COLUMN= %d,LINE= %d \n",&column,&line);
    if(line>=50 || line<=10 || column>=80 || column<=5)
    {
        strcpy(error,"Incorrect dimension");
        return NULL;
    }
    save->column = column;
    save->line = line;
    Map=(data_values **) malloc(save->line*sizeof(data_values *));
    for(i=0;i<save->line;i++)
    {
        Map[i]=(data_values *) malloc(save->column*sizeof(data_values));
    }
    fscanf(map_file,"%[^,]",&pawns);//Get chars until comma
    fseek(map_file,1,SEEK_CUR); //Moving the cursor to the next position
    fscanf(map_file,"%\n",NULL); //Jump to the next line
    strcpy(save->pawns,pawns);
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
                Map[i][j].fog=0;
                Map[i][j].visibility_blue=0;
                Map[i][j].visibility_red=0;
                Map[i][j].death=0;
            }
            else
            {
                strcpy(error,"Map corrupted");
                return NULL;
            }
        }
        fscanf(map_file,"%\n",NULL); //Jump to the next line
    }
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
        strcpy(error,"Error during the creation of the save");
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
        strcpy(error,"Error during the loading of the save");
        return NULL;
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

void getDirectory(char directory[],int n,data_save *save)
{
    DIR *d;
    char result[30],please[30];
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
                if(i==n)
                {
                    strcpy(result,dir->d_name);
                }
            }
        }
    closedir(d);
    }
    strcpy(save->mapname,result);
    return;
}
