#ifndef NETWORK_H
#define NETWORK_H

#define LISTENQ 1024    
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include<arpa/inet.h>
#include<netdb.h>
#include<fcntl.h>
#include "errno.h"


int open_listenfd(char* port);

int accept_connection(int listenfd);



#endif




