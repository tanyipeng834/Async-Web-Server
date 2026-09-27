
#include<stdlib.h>
#include<stdio.h>
#include<sys/epoll.h>
#include "network.h"
#include "event.h"
#include "errno.h"
#define MAX_EVENTS 1024


int main(int argc, char*argv[]){
    if(argc!=2){
        printf("Usage: ./server <port_number>");
        exit(1);
    }
    char * port_num = argv[1];
    int listen_fd =open_listenfd(port_num);
    printf("listening ...\n");
    if(listen_fd<1){
        perror("Failed to listen to port");
        exit(1);
    }
    
    struct epoll_event events[MAX_EVENTS];
    int epfd = epoll_create1(0);
    if(register_listener(epfd,listen_fd)==-1){
        exit(1);
    }
    if(epfd==-1)
    {
        perror("epoll_create1");
    }

    while(1)
    {   
        // create an epoll instance
        
        // epfd is -1 means a system call error
      
        int nready = epoll_wait(epfd,events,MAX_EVENTS,-1);
        if(nready==-1)
        {   
            if(errno==EINTR)continue;

            perror("epoll_wait");
            break;
        }

        for(int i =0; i<nready;i++)
        {
            event_handler * handler = events[i].data.ptr;
            handler->callback(
                epfd, events[i].events,handler
            );



        }



    }



    



}

