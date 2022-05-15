#include <iostream>
#include <string>
#include <string.h>
#include <csignal>
#include <ostream>
#include <unistd.h>
#include <stdlib.h>
#include <cstdio>
#include <errno.h>
#include <dirent.h>

using namespace std;

volatile sig_atomic_t breakCount = 0;
void signal_handler(int sig){
}
void prompt();
int searchInternalCommand(char *cmd);
void runInternalCommand(int index,char *cmd);
void runExternalCommand(char *cmd);

void pwd();
void ls();
void changep(char *switchs);
void rm(char *cmd);
void clearScreen();

char commands[][255] = {"exit","ls","pwd","rm","changep","clscr"};
char promptTextUser[128] = "naizari";
char promptTextGroup[128]="ubuntu";
int commandsLength = 6;


int main()
{
    signal(SIGINT, signal_handler);

    do{
        char cmd[256] = {0};
        prompt();
        cin.clear();
        gets(cmd);

        int index = searchInternalCommand(cmd);
        if(index>-1){
            runInternalCommand(index,cmd);
        }else{
            runExternalCommand(cmd);
        }

    }while(1);
    return 0;
}

void prompt(){
    char cwd[1024];
    getcwd(cwd,sizeof(cwd));
    cout<<promptTextUser<<"@"<<promptTextGroup<<":"<<cwd<<"~$ ";
}
void clearScreen(){
    cout<<"\x1b[2J\x1b[1;1H"<<flush;
}

void pwd(){
    char cwd[1024];
    getcwd(cwd,sizeof(cwd));
    cout <<cwd<<endl;
}
void ls(){
    DIR *d;
    struct dirent *dir;
    d = opendir(".");
    if(d){
        while((dir=readdir(d))!=NULL){
            cout<<dir->d_name<<"\n";
        }
        closedir(d);
    }
}
void changep(char *switchs){
    char *loc,*loc2;
    int locnumber2,locnumber,endOfSwitch,endOfCommand,i,promptTextUserLength,mainParameterLength,promptTextGroupLength;
    loc = strchr(switchs,'-');
    loc2 = strrchr(switchs,' ');
    if(loc){
        locnumber = loc-switchs;
    }else{
        cout<<"You can't Use This Command Without switch"<<endl;
        return;
    }
    if(loc2){
        locnumber2 = loc2-switchs;
    }
    if(locnumber2<locnumber){
        cout<<"You can't Use This Command Without Parameter"<<endl;
        return;
    }
    endOfSwitch = loc2-switchs;
    endOfCommand = strlen(switchs);

    char *mainSwitch = new char[endOfSwitch-locnumber-1];

    for(int i=locnumber+1,j=0;i<endOfSwitch;i++,j++){
        mainSwitch[j] = switchs[i];
    }

    char *mainParameter = new char[endOfCommand-endOfSwitch-1];
    for(int i=endOfSwitch+1,j=0;i<endOfCommand;i++,j++){
        mainParameter[j] = switchs[i];
    }
    mainParameterLength = strlen(mainParameter);
    promptTextUserLength = strlen(promptTextUser);
    promptTextGroupLength = strlen(promptTextGroup);

    switch(mainSwitch[0]){
        //change user
        case 'u':
            //meghdare ghablie promptTextUser ra pak mikonad
            for(i=0;i<promptTextUserLength;i++){
                promptTextUser[i] = '\0';
            }
            //promptTextUser ra meghdar dahi mikonad
            for(i=0;i<mainParameterLength;i++){
                promptTextUser[i] = mainParameter[i];
            }
            //promptTextUser = mainParameter;
            break;

        //change group
        case 'g':
            for(i=0;i<promptTextGroupLength;i++){
                promptTextGroup[i] = '\0';
            }
            for(i=0;i<mainParameterLength;i++){
                promptTextGroup[i] = mainParameter[i];
            }
            break;
    }
}
void rm(char *cmd){
    char *loc;
    int locnumber,endOfCommand;
    loc = strchr(cmd,' ');
    if(loc){
        locnumber = loc-cmd;
    }else{
        cout<<"You can't Use This Command Without Parameter"<<endl;
        return;
    }
    endOfCommand = strlen(cmd);
    char *fileName = new char[endOfCommand-locnumber-1];

    for(int i=locnumber+1,j=0;i<endOfCommand;i++,j++){
        fileName[j] = cmd[i];
    }
    if(remove(fileName)){
        cout<<"no such file find to delete"<<endl;
    }
}


int searchInternalCommand(char *cmd){
    char *loc;
    loc = strchr(cmd,' ');
    int locnumber;
    if(loc){
        locnumber = loc-cmd;
    }else{
        locnumber = strlen(cmd);
    }
    char *maincmd = new char[locnumber];

    for(int i=0;i<locnumber;i++){
        maincmd[i] = cmd[i];
    }

    for(int i=0;i<commandsLength;i++){
        if(strcmp(commands[i],maincmd)==0){
            return i;
        }
    }
    return -1;
}
void runInternalCommand(int index,char *cmd){
    switch(index){
        case 0:
            exit(0);
            break;

        case 1:
            ls();
            break;

        case 2:
            pwd();
            break;

        case 3:
            rm(cmd);
            break;

        case 4:
            changep(cmd);
            break;
        case 5:
            clearScreen();
            break;
        default:
            break;
    }
}
void runExternalCommand(char *cmd){
     system(cmd);
}
