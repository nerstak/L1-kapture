#include <stdio.h>
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

int posexist(int xpos,int ypos,int TEMPWIDTH,int TEMPHEIGHT){
    if(xpos>=0 && ypos>=0 && xpos<TEMPWIDTH && ypos<TEMPHEIGHT){
        return 1;
    }
    return 0;
}

int main() {
  int inputed,xpos,ypos,xmove,ymove,turnend,select,movend,movepoints,charend,movecost,TEMPWIDTH,TEMPHEIGHT,i,c;
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
            default:
                break;
            }
            if(charend!=1){
                if(movepoints>=movecost && posexist((xpos+xmove),(ypos+ymove),TEMPWIDTH,TEMPHEIGHT)==1){
                    if(Map[ypos+ymove][xpos+xmove].Entity==' '){
                        if(Map[ypos][xpos].Flag=1){
                            Map[ypos+ymove][xpos+xmove].Flag=1;
                        }
                        Map[ypos+ymove][xpos+xmove].Entity=Map[ypos][xpos].Entity;
                        Map[ypos+ymove][xpos+xmove].Team=Map[ypos][xpos].Team;
                        Map[ypos][xpos].Team=0
                        Map[ypos][xpos].Entity=' ';
                        Map[ypos][xpos].Flag=0;
                        movepoint=movepoints-movecost;
                        ypos=ypos+ymove;
                        xpos=xpos+xmove;

                        //move done here


                        for(i=-1;i=<1;i=+2){
                            for(c=-1;c=<1;c=+2){
                                if(posexist((xpos+i),(ypos+i),TEMPWIDTH,TEMPHEIGHT)==1){
                                    Map[ypos+i][xpos+i].FOW=1;                //Fog Of War

                                    if(Map[ypos+i]T[xpos+i].Entity!=' ' && Map[ypos+i][xpos+i].Team!=Map[ypos][xpos].Team){ //FIGHT
                                        goodguy=Map[ypos][xpos].Entity;
                                        badguy=Map[ypos+i][xpos+i].Entity;

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
    return 0;
}
