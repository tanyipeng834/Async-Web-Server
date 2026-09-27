#include "event.h"
#include "network.h"




int register_listener(int epfd, int listenfd)
{
    event_handler * handler = malloc(sizeof(event_handler));

    if(handler==NULL){
        perror("NULL");
        return -1;

    }
    handler ->fd = listenfd;
    handler->callback = &listen_callback;
    // this is mainly for connection data
    handler->data = NULL;

    struct epoll_event event ={0};
    // this is for registering to keep track when it is avaialble for read operations
    event.events = EPOLLIN;
    // have a pointer to the handler function
    event.data.ptr = handler;

    if(epoll_ctl(epfd,EPOLL_CTL_ADD,listenfd,&event)==-1)
    {
        perror("epoll_ctl");
        free(handler);
        return -1;


    }


    return 0;



}


void listen_callback(int epfd,uint32_t events, void *event_data)
{
    if(events & (EPOLLERR|EPOLLHUP))
    {
        fprintf(stderr,"error on listening socket");
        return ;





    }
    event_handler * event = (event_handler*) event_data;
    // accept connection with the listenfd and return a
    // non blocking connfd
    int connfd = accept_connection(event->fd);




   

}
