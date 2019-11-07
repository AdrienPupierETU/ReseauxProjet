#include "extremite.h"
#include "iftun.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void usage(){
  printf("usage : ./extremite <tunname> -in <addr> <port>\n");
  printf("usage : ./extremite <tunname> -out <port> \n");
  printf("usage : ./extremite <tunname> -bi <addr> <port> \n");
}

int main (int argc, char** argv){
  if(argc<4){
    usage();
    exit(1);
  }
  int tunfd;
  char *name=argv[1];
  tunfd=createInterface(name);
  if(strcmp(argv[2],"-in")==0){
    char* extrTunnel=argv[3];
    ext_in(tunfd,extrTunnel,argv[4]);   
  }else if(strcmp(argv[2],"-out")==0){
    ext_out(argv[3],tunfd);
  }else if(strcmp(argv[2],"-bi")==0){
    bidirectional(tunfd,argv[3],argv[4]);
  }
  printf("Appuyez sur une touche pour terminer\n");
  getchar();
  printf("FIN\n");
  return 0;
}
