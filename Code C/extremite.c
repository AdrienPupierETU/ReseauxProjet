


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



void ext_out(){

  int s,n;
  int len,on;
  struct addrinfo *resol;
  struct addrinfo indic={AI_PASSIVE, PF_INET,SOCK_STREAM,0,0,NULL,NULL,NULL};
  struct sockaddr_in client;
  char *port="123";
  int err;

  err=getaddrinfo(NULL,port,&indic,&resol);

  if(err<0){
    fprintf(stderr,"Resolution %s\n",gai_strerror(err));
    exit(1);
  }
  if((s=socket(resol->ai_family,resol->ai_socktype,resol->ai_protocol))<0){ //socket
    perror("Alloc socket");
    exit(1);
  }
  on=1;
  if(setsockopt(s,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on))<0){
    perror("option socket");
    exit(1);
  }
  if(bind(s,resol->ai_addr,sizeof(struct sockaddr_in))<0){ //bind
    perror("bind");
    exit(1);
  }
  freeaddrinfo(resol);
  if(listen(s,SOMAXCONN)<0){ //listen
    perror("listen");
    exit(1);
  }
  while(1){
    len=sizeof(struct sockaddr_in);
    if((n=accept(s,(struct sockaddr *)&client,(socklen_t*)&len))<0){ //accept
      perror("accept");
      exit(1);
    }
    printStandard(n); //traitement
  }

}


void ext_in(){

}

void usage(){
  printf("usage : ./extremite -in <addr>\n");
  printf("usage : ./extremite -out <port> \n");
}

int main (int argc, char** argv){
  printf("salut \n");
  fflush(stdin);
  if(argc<2){
    usage();
    exit(1);
  }
  if(strcmp(argv[1],"-in")==0){
    ext_in();   
  }else if(strcmp(argv[1],"-out")==0){
    printf("salut \n");
    fflush(stdin);
    ext_out();
  }

  return 0;
}
