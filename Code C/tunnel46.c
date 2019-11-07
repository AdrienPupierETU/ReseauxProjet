#include "extremite.h"
#include "iftun.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>



#define MAXCHAR 250

void usage(){
  printf("usage : ./tunnel46 <tunname>\n");
}

int main (int argc, char** argv){
   /* if(argc!=2){
        usage();
        exit(1);
    }*/

    FILE *file;
    char str[MAXCHAR];
    char *filename = "/vagrant/configTunnel";

    char *tunName;
    char *inip;
    char *inport;
    char *options;
    char *outip;
    char *outport;

    file =fopen(filename,"r");
    char delim[]="=";
    if(file ==NULL){
        perror("open config file");
        exit(1);
    }
   
    while (fgets(str, MAXCHAR, file) != NULL){
        if(strncmp(str,"#",1)==0){ // si la ligne commence par # on l'ignore
            continue;
        }
        int len= strlen(str);     
        if( str[len-1] == '\n' )
            str[len-1] = 0;
        //printf("%s \n", str);
        char *ptr= strtok(str,delim);
        while(ptr!=NULL){
            //printf("%s \n",ptr);
            if(strcmp(ptr,"tun") == 0){
                ptr = strtok(NULL, delim);
                tunName=strdup(ptr);
            }
            if(strcmp(ptr,"inip")==0){
                ptr = strtok(NULL, delim);
                inip=strdup(ptr);

            }
            if(strcmp(ptr,"inport")==0){
                ptr = strtok(NULL, delim);
                inport=strdup(ptr);
            }
            if(strcmp(ptr,"options")==0){
                ptr = strtok(NULL, delim);
                options=strdup(ptr);
            }
            if(strcmp(ptr,"outip")==0){
                ptr = strtok(NULL, delim);
                outip=strdup(ptr);
            }
            if(strcmp(ptr,"outport")==0){
                ptr = strtok(NULL, delim);
                outport=strdup(ptr);
            }
            ptr = strtok(NULL, delim);
        }
    }

    printf("TunName : %s \n",tunName);
    printf("inip : %s \n",inip);
    printf("inport : %s \n",inport);
    //printf("options : %s \n",options);
    printf("outip : %s \n",outip);
    printf("outport : %s \n",outport);
    fclose(file);
    int tunfd=createInterfaceAutoConfig(tunName);
    printf("bidirectional \n");
    bidirectional(tunfd,outip,outport);
    printf("Appuyez sur une touche pour terminer\n");
    getchar();
    printf("FIN\n");
    return 0;
}