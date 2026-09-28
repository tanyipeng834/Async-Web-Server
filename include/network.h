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



typedef struct
{
    // this is the connfd which we retun back to the event loop 
    int fd;
    // read buffer for our conn fd 
    char read_buf[8192];
    // valid bytes in the buffer
    size_t read_len;
    // how many
    size_t read_offset;
    // user land buffer for writing to the conn fd
    char write_buf[8192];
    // how much of the bytes has been handed over to the kernel write()
    size_t write_offset;
    // bytes lefft to write to the socket
    size_t write_cnt;
}connection;


int open_listenfd(char* port);

int accept_connection(int listenfd);



#endif




