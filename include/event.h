#ifndef EVENT_H
#define EVENT_H

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include<stdio.h>
#include<sys/epoll.h>
#include "network.h"
#include "io.h"
#include "http_request.h"



// this is the event callback function pointer that is used 
// when the fd becomes ready


typedef void(*event_callback)(int epfd,uint32_t events, void * data);
typedef struct 
{
    int fd;
    event_callback callback;
    void * data;

}event_handler;
void listen_callback(int epfd,uint32_t events, void * data);


int register_listener(int epfd,int listenfd);

int register_connection(int epfd,int connfd);

void connection_callback(int epfd,uint32_t events,void* data);


#endif









