#ifndef IFTUN_H
#define IFTUN_H

int createInterface(char* name);
int createInterfaceAutoConfig(char * name, char * addresse);
int openFile(char* path );
void recopy(int src,int dest);
int pWrite(int fd,char* buff,int buffSize);
int pRead(int fd,char* buff,int buffSize);
#endif