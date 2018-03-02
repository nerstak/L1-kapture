#include <stdio.h>              //TODO: replace the Map[][]=="x" to some fucking strcmp()   (and also fix all the struct refferences so they are consistent with Karsten's)
#include <stdlib.h>

int userinput(){       //WINDOWS VERSION
    int inputed;
    while(1){
    inputed=getch();
    switch(inputed) {
        case 48:
            return 0;
            break;
        case 49:
            return 1;
            break;
        case 50:
            return 2;
            break;
        case 51:
            return 3;
            break;
        case 52:
            return 4;
            break;
        case 53:
            return 5;
            break;
        case 54:
            return 6;
            break;
        case 55:
            return 7;
            break;
        case 56:
            return 8;
            break;
        case 57:
            return 9;
            break;
}
}
}

int posexist(int xpos,int ypos,int TEMPWIDTH,int TEMPHEIGHT){   //checks if a coordinate is within the map's borders
    if(xpos>=0 && ypos>=0 && xpos<TEMPWIDTH && ypos<TEMPHEIGHT){
        return 1;
    }
    return 0;
}

data_values move(int ypos,int xpos,int ydest,int xdest,data_values **Map){   //Moves a unit (TODO: see if Map works and if it does if its not too laggy)
    Map[ydest][xdest].Team=Map[ypos][xpos].Team;
    Map[ydest][xdest].Entity=Map[ypos][xpos].Entity;
    Map[ydest][xdest].Flag=Map[ypos][xpos].Flag;
    Map[ypos][xpos].Team=' ';
    Map[ypos][xpos].Entity=' ';
    Map[ypos][xpos].Flag=0;
    return Map;
}

int main() {
  int inputed,xpos,ypos,xmove,ymove,turnend,select,movend,movepoints,charend,movecost,TEMPWIDTH,TEMPHEIGHT,i,c,XTEMPSPAWN,YTEMPSPAWN;   //all TEMP variables are placeholders for now
    turnend=0;
    while (turnend==0){

        switch(userinput()) {           //PAWN SELECTION
            case 0:
                turnend=1;
                break;
            case 1:
                break;
            default:
                break;
        }


        //TODO: Actually select a pawn, set movepoints, maxmovement and x and y pos.


        while (charend!=1 && movepoints>0 && turnend!=1){
            ymove=0;
            xmove=0;
            switch(Map[ypos][xpos].terrain){
                case "+":
                    movecost=2;
                    break;
                case ".":
                    movecost=1;
                    break;
                case "~":
                    movecost=maxmovement;
                    break;
            }
            switch(userinput()) {       //MOVEMENT
            case 0:
                charend=1;
                break;
            case 1:
                xmove--;
                ymove++;
                break;
            case 2:
                ymove++;
                break;
            case 3:
                xmove++;
                ymove++;
                break;
            case 4:
                xmove--;
                break;
            case 6:
                xmove++;
                break;
            case 7:
                xmove--;
                ymove--;
                break;
            case 8:
                ymove--;
                break;
            case 9:
                xmove++;
                ymove--;
                break;
            default://Fog Of War
                break;
            }
            if(charend!=1){
                if(movepoints>=movecost && posexist((xpos+xmove),(ypos+ymove),TEMPWIDTH,TEMPHEIGHT)==1){
                    if(Map[ypos+ymove][xpos+xmove].Entity==' '){
                        Map=move(ypos,xpos,ypos+ymove,xpos+xmovem,Map)
                        /*
                        Map[ypos+ymove][xpos+xmove].Flag=Map[ypos][xpos].Flag;
                        Map[ypos+ymove][xpos+xmove].Entity=Map[ypos][xpos].Entity;
                        Map[ypos+ymove][xpos+xmove].Team=Map[ypos][xpos].Team;
                        Map[ypos][xpos].Team=""
                        Map[ypos][xpos].Entity=' ';
                        Map[ypos][xpos].Flag=0;
                        */
                        movepoint=movepoints-movecost;
                        ypos=ypos+ymove;
                        xpos=xpos+xmove;

                        //move done here


                        for(i=-1;i=<1;i=+2){
                            for(c=-1;c=<1;c=+2){
                                if(posexist((xpos+i),(ypos+i),TEMPWIDTH,TEMPHEIGHT)==1){

                                    if(Map[ypos][xpos].Team=="red"){        //Fog Of War: 0=unseen, 1=seen by red, 2=seen by blue, 3=seen by both
                                        if(Map[ypos+i][xpos+i].FOW==2){
                                            Map[ypos+i][xpos+i].FOW=3;
                                        }
                                        else{
                                            Map[ypos+i][xpos+i].FOW=1;
                                        }
                                    }
                                    else{
                                        if(Map[ypos+i][xpos+i].FOW==1){
                                            Map[ypos+i][xpos+i].FOW=3;
                                        }
                                        else{
                                            Map[ypos+i][xpos+i].FOW=2;
                                        }
                                    }

                                    if(Map[ypos+i]T[xpos+i].Entity!=' ' && Map[ypos+i][xpos+i].Team!=Map[ypos][xpos].Team){ //FIGHT


                                        switch(Map[ypos][xpos].Entity) {        //TODO: make all combat, do a different combat if flag carrying
                                        case "S":
                                            switch(Map[ypos+i][xpos+i].Entity) {
                                            case "I":


                                                break;
                                            case "T":

                                                break;
                                            case "F":

                                                break;
                                            }
                                            break;


                                        case "I":               //Maybe change I to C or something so it's more unique and different to T | TITITITITIITITIT <-- hard to see?
                                            switch(Map[ypos+i][xpos+i].Entity) {
                                            case "S":

                                                break;
                                            case "I":

                                                break;
                                            case "T":

                                                break;
                                            case "F":

                                                break;
                                            }
                                            break;


                                        case "T":
                                            switch(Map[ypos+i][xpos+i].Entity) {
                                            case "S":

                                                break;
                                            case "I":

                                                break;
                                            case "T":

                                                break;
                                            case "F":

                                                break;
                                            }
                                            break;
                                        }
                                    }
                                }
                            }
                        }
                        else{
                        //INVALID MOVEMENT
                        }
                    }
                    else{
                    //INVALID MOVEMENT
                    }
                }
            }
        }
    }
    return 0;
}
