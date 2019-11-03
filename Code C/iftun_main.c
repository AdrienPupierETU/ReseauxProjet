
#include <string.h>
#include <stdio.h>

#include <unistd.h>
#include <fcntl.h>
#include <linux/if.h>
#include <linux/if_tun.h>

#include "iftun.h"

void usage(){
  printf("Usage : ./iftun <name> -std : Recopie ce qui est lu sur l'interface TUN sur la sortie standard \n");
  printf("Usage : ./iftun <name> -p <path> : Recopie ce qui est lu sur l'interface TUN sur le fichier situé à <path> \n");
}

int main (int argc, char** argv){
  if(argc<3){
    usage();
    return 1;
  }
  char * tunName=argv[1];
  int dstfd;
  if(strcmp(argv[2],"-std")==0){
    dstfd=1;   
  }else if(strcmp(argv[2],"-p")==0){
    char* destName=argv[3];
    dstfd=openFile(destName);
  }
  int tunfd;
  tunfd=createInterface(tunName);
  fd_set rd_set;
  FD_ZERO(&rd_set);
  FD_SET(tunfd, &rd_set);
  if(FD_ISSET(tunfd, &rd_set)){
     recopy(tunfd,dstfd);
  }
  printf("Appuyez sur une touche pour terminer\n");
  getchar();
  printf("FIN\n");  
  return 0;
}