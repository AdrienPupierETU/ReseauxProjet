#ifndef EXTREMITE
#define EXTREMITE

void ext_in(int tunfd,char *destAddr,char *port);
void ext_out(char * port,int tunfd);
void bidirectional(int tunfd,char * addresse, char * port);
#endif