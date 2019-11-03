#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h> 
#include <sys/stat.h>
#include <sys/ioctl.h>

#include <unistd.h>
#include <fcntl.h>
#include <linux/if.h>
#include <linux/if_tun.h>

#include "tunalloc.h"


int pRead(int fd,char *buff,int buffSize){
    int err;    
    if((err=read(fd,buff,buffSize)<0)){
        perror("Reading data");
        exit(1);
    }
    return err;
}

int pWrite(int fd, char* buff,int buffSize){
    int err;    
    if((err=write(fd,buff,buffSize)<0)){
        perror("writing data");
        exit(1);
    }
    return err;
}

void recopy(int src,int dest){
    int buffSize=250;        
    fflush(stdout);
    while(1){
        char *buff=malloc(sizeof(char)*buffSize);
        //printf("\n Read TUN \n");
        pRead(src, buff, buffSize);
        //printf("Write dst \n");        
        pWrite(dest, buff, buffSize);
    }    
    
}

int openFile(char *path ){
    int fd;
    if((fd=open(path,O_RDWR))<0){
        perror("open FILE DST ");
        exit(-1);
    }
    return fd;
}

int createInterface(char * name){
  int tunfd;
  printf("Création de %s\n",name);

  tunfd = tun_alloc(name);
  
  printf("Faire la configuration de %s...\n",name);
  fflush(stdin);
  printf("Appuyez sur une touche pour continuer\n");
  getchar();
  printf("Interface %s Configurée:\n",name);
  system("ip addr");
  return tunfd;
}
