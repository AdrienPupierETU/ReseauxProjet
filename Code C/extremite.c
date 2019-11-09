


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
#include <pthread.h>

#include <errno.h>

#include "iftun.h"

typedef struct{
  int tunfd;
  char * addresseDest;
  char * port;
} *data_t;


void printStandard(int socket){
  ssize_t lu;
  int maxlen=80;
  char tampon[maxlen];
  do{
    lu=recv(socket,tampon,maxlen,0);
    printf("Lu value : %lu",lu);
    if(lu>0){
      printf("%s\n",tampon);
    }else{
      break;
    }
  }while(1);
  close(socket); 
}

void recopyFromSocket(int socket, int dest){
  ssize_t lu;
  int maxlen=250;
  char tampon[maxlen];
  do{
   // printf("En attente \n");
    lu=recv(socket,tampon,maxlen,0);
    //printf("Lu value : %lu \n",lu);
    //printf("buffer : %s \n",tampon);
    if(lu==0){
      printf("Connection fermé par le client \n");
      break;
    }
    if(lu<0){
      printf("Erreur recv \n");
      fprintf(stderr, "recv: %s (%d)\n", strerror(errno), errno);
      break;
    }
    //printf("Ecriture \n");
    pWrite(dest, tampon, lu);
    //printf("Ecrit \n");
  }while(1);
  close(socket); 
}

void recopyToSocket(int fd, int socket){
  int buffSize=250;        
    while(1){
        char *buff=malloc(sizeof(char)*buffSize);
        //printf("\n Read src \n");
        pRead(fd, buff, buffSize);
       // printf("send data \n");
        send(socket,buff,buffSize,0);
    }
}


void ext_out(char * port,int tunfd){
    int s, clientfd;
    int reuseaddr = 1;
    struct sockaddr_in6 addr,client;
    int len;
    if(0>(s = socket(AF_INET6, SOCK_STREAM, 0))){
      perror("Socket");
      exit(1);
    }
    setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &reuseaddr, sizeof(reuseaddr));

    addr.sin6_family = AF_INET6;
    addr.sin6_port = htons(atoi(port));
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
    for(;;){
      printf("waiting for client \n");
      len=sizeof(struct sockaddr_in6);
      if(0>(clientfd=accept(s,(struct sockaddr *)&client,(socklen_t*)&len))){
        perror("accept");
        exit(1);
      }
      recopyFromSocket(clientfd,tunfd);
    }
    
}

void ext_in(int tunfd,char *destAddr,char *port){
  int s;
  struct sockaddr_in6 addr;
  if(0>(s=socket(AF_INET6,SOCK_STREAM,0))){
    perror("Socket");
    exit(1);
  }
  addr.sin6_family=AF_INET6;
  addr.sin6_port=htons(atoi(port));
  inet_pton(AF_INET6,destAddr,&addr.sin6_addr);
  /*if(connect(s,(struct sockaddr *)&addr,sizeof(struct sockaddr_in6))<0){
    perror("connect");
    exit(1);
  }*/
  for(;;){
    while(connect(s,(struct sockaddr *)&addr,sizeof(struct sockaddr_in6))<0){
      printf("waiting for distant host to run \n");
      sleep(1);
    }
    printf("Connect done \n");
    recopyToSocket(tunfd,s);
  }
  printf("fin in \n");
}

void * thread_out(void * structt){
  data_t data = (data_t) structt;
  ext_out(data->port,data->tunfd);
  return NULL;
}

void * thread_in(void * structt){
  data_t data = (data_t) structt;
  ext_in(data->tunfd,data->addresseDest,data->port);
  return NULL;
}


void bidirectional(int tunfd,char * addresse, char * port){
  printf("copy data to struct \n");
  data_t data =malloc(sizeof(data_t));
  data->tunfd=tunfd;
  data->addresseDest=addresse;
  data->port=port;

  
  pthread_t threadIn;
  pthread_t threadOut;
  printf("lance thread1 \n");
  if(pthread_create(&threadIn, NULL, thread_in,(void *) data) == -1) {
	    perror("pthread_create");
	    exit(1);
    }
  printf("lance thread2 \n");
  if(pthread_create(&threadOut, NULL, thread_out,(void *) data) == -1) {
	    perror("pthread_create");
	    exit(1);
    }
  if(pthread_join(threadIn,NULL)){
    perror("pthread_join");  
  }

  
}

