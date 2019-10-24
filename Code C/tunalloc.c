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




int tun_alloc(char *dev)
{
	struct ifreq ifr;
	int fd, err;

	if( (fd = open("/dev/net/tun", O_RDWR)) < 0 ){
		perror("alloc tun");
		exit(-1);
	}

  memset(&ifr, 0, sizeof(ifr));

  /* Flags: IFF_TUN   - TUN device (no Ethernet headers) 
   *        IFF_TAP   - TAP device  
   *
   *        IFF_NO_PI - Do not provide packet information  
   */ 
  ifr.ifr_flags = IFF_TUN |IFF_NO_PI; 
  if( *dev )
    strncpy(ifr.ifr_name, dev, IFNAMSIZ);

  if( (err = ioctl(fd, TUNSETIFF, (void *) &ifr)) < 0 ){
    close(fd);
    return err;
  }
  strcpy(dev, ifr.ifr_name);
  return fd;
}

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
    int buffSize=64;        
    fflush(stdout);
    while(1){
        char *buff=malloc(sizeof(char)*buffSize);
        printf("\n Read TUN \n");
        pRead(src, buff, buffSize);
        printf("Write dst \n");        
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

void createInterface(char * name,int dstfd ){
  int tunfd;
  fd_set rd_set;
  
  printf("Création de %s\n",name);


  tunfd = tun_alloc(name);
  

  printf("Faire la configuration de %s...\n",name);
  printf("Appuyez sur une touche pour continuer\n");
  getchar();
  printf("Interface %s Configurée:\n",name);
  system("ip addr");

  FD_ZERO(&rd_set);
  FD_SET(tunfd, &rd_set);
  if(FD_ISSET(tunfd, &rd_set)){
     recopy(tunfd,dstfd);
  }

  printf("Appuyez sur une touche pour terminer\n");
  getchar();
  printf("FIN\n");  
}

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
  
  createInterface(tunName,dstfd);
  return 0;
}
