


#include <sys/stat.h>
#include <sys/ioctl.h>

#include <fcntl.h>
#include <linux/if.h>
#include <linux/if_tun.h>

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <netdb.h>

#include <arpa/inet.h>

#include "iftun.h"

#define SERVER_PORT     3005
#define BUFFER_LENGTH    250



void printStandard(int fd){
  ssize_t lu;
  char tampon[80+1];
  do{
    lu=recv(fd,tampon,80,0);
    if(lu>0){
      printf("%s\n",tampon);
    }else{
      break;
    }
  }while(1);
  close(fd); 
}

void ext_out(char * port){
    int s, c;
    int reuseaddr = 1;
    struct sockaddr_in6 addr;

    if((s = socket(AF_INET6, SOCK_STREAM, 0))<0){
      perror("Socket");
      exit(1);
    }
    setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &reuseaddr, sizeof(reuseaddr));

    addr.sin6_family = AF_INET6;
    addr.sin6_port = htons(123);
    addr.sin6_addr = in6addr_any;

    if(bind(s, (struct sockaddr *)&addr, sizeof(addr))<0){
      perror("bind");
      exit(1);
    }
    if(listen(s, SOMAXCONN)){
      perror("listen");
      exit(1);
    }
    printf("Serveur Up without any problems \n");
    while(1){
      printf("waiting for client \n");
      if((c=accept(s,NULL,NULL)<0)){ //accept
        perror("accept");
        exit(1);
      }
      printStandard(c); //traitement
    }
  
}

void ext_in(int tunfd,char *destAddr){
  int s;
  //char * port;

  struct sockaddr_in6 addr;
  if((s=socket(AF_INET6,SOCK_STREAM,0))<0){
    perror("Socket");
    exit(1);
  }
  addr.sin6_family=AF_INET6;
  addr.sin6_port=htons(123);
  inet_pton(AF_INET6,destAddr,&addr.sin6_addr);
  if(connect(s,(struct sockaddr *)&addr,sizeof(addr))<0){
    perror("connect");
    exit(1);
  }
  fd_set rd_set;
  FD_ZERO(&rd_set);
  FD_SET(tunfd, &rd_set);
  if(FD_ISSET(tunfd, &rd_set)){
    recopy(tunfd,s);
  }
}


void usage(){
  printf("usage : ./extremite <tunname> -in <addr> <port>\n");
  printf("usage : ./extremite <tunname> -out <port> \n");
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
    ext_in(tunfd,extrTunnel);   
  }else if(strcmp(argv[2],"-out")==0){
    ext_out(argv[3]);
  }
  printf("Appuyez sur une touche pour terminer\n");
  getchar();
  printf("FIN\n");
  return 0;
}
