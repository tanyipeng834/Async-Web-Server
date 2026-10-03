
#include "event.h"

int register_listener(int epfd, int listenfd)
{
    event_handler * handler = malloc(sizeof(event_handler));

    if(handler==NULL){
        perror("malloc");
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


void listen_callback(int epfd,uint32_t events, void *data)
{
    if(events & (EPOLLERR|EPOLLHUP))
    {
        fprintf(stderr,"error on listening socket");
        return ;





    }
    event_handler * event = (event_handler*) data;
    // accept connection with the listenfd and return a
    // non blocking connfd

    while(1){

        int connfd = accept_connection(event->fd);
        // means there could be an error
        if(connfd >=0){

        if(register_connection(epfd,connfd)==-1){
        fprintf(stderr,"Not able to register connection to epoll instance");
        close(connfd);
        return;
        }
        // accept another connection
        continue;
        }

        else{
            // this means there are no more connections
            if(errno ==EAGAIN ||errno ==EWOULDBLOCK)break;



        }
        
        




        



    }
    




   

}


int register_connection(int epfd, int connfd)
{

   

    event_handler * handler = malloc(sizeof(event_handler));

    if(handler ==NULL){
        perror("malloc");
        return -1;
    }
    connection * con = calloc(1,sizeof(connection));
    con->fd = connfd;
    con->http_state=PARSE_REQUEST_LINE;
    

    handler->fd = connfd;
    // callback 
    handler->callback = &connection_callback;
    // create an connection object
    handler->data = con;
    // since the tcp connection is full duplex where we can write and read from both ends

    // null initailize the epoll_event struct
    struct epoll_event event ={0};
    event.events = EPOLLIN;
    event.data.ptr = handler;


     if(epoll_ctl(epfd,EPOLL_CTL_ADD,connfd,&event)==-1)
    {
        perror("epoll_ctl");
        free(handler);
        return -1;


    }


    return 0;

    






}

// this is for the connection callback 
void connection_callback(int epfd,uint32_t events,void* data)
{

    connection* con = ((event_handler*)data)->data;

    if(events&EPOLLIN){

    int result = handle_read(con);

    if (result == 0) {
        // EOF: peer closed its sending side
        epoll_ctl(epfd, EPOLL_CTL_DEL, con->fd, NULL);
        close(con->fd);

        free(con);
        free(data);   // data is your event_handler *

        return;
    }

    if (result == -1) {
        // actual error
        epoll_ctl(epfd, EPOLL_CTL_DEL, con->fd, NULL);
        close(con->fd);

        free(con);
        free(data);

        return;
    }

    // result == 1 means we drained until EAGAIN
    while(1){
        // we will keep parsing until line is not fully processed or we will parse until the request has
        // been fully parsed
        int result = parse_http(con);
         if (result == 1) {
    printf("HTTP request parsed!\n");

    printf("Method:  %s\n", con->request->method);
    printf("URI:     %s\n", con->request->uri);
    printf("Version: %s\n", con->request->version);
}
       
        if(result==0 || result ==-1 ){
            break;
        }
    }

    const char *response =
    "HTTP/1.1 200 OK\r\n"
    "Content-Type: text/plain\r\n"
    "Content-Length: 5\r\n"
    "\r\n"
    "Hello";
    // copy the response to the write buffer
    memcpy(con->write_buf,response,strlen(response));
    // write the result until this two condition happens
    // one when the condition has been successful
    while(1){
        int result = handle_write(con);
        // 
        if(result ==0 || result ==-1){
            break;

        }

        if(result ==1){
            // create an event with 
            struct epoll_event event = {0};
            event.events = events| EPOLLOUT;
           // the callback would be the same to connection callback
            
            event.data.ptr = data;



            if (epoll_ctl(epfd,EPOLL_CTL_MOD,con->fd,&event) == -1)
    {
        perror("epoll_ctl MOD");
       return;
    }

  
}





            
        }

    }



    // write the result

    


    else if(events&(EPOLLERR |EPOLLHUP)){
        fprintf(stderr,"error on listening socket");
        return ;
    }




}

