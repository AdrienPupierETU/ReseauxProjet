#ifndef IFTUN_H
#define IFTUN_H

int createInterface(char * name);
int openFile(char *path );
void recopy(int src,int dest);
#endif