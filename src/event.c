
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

        printf("NEW TCP CONNECTION: fd=%d\n", connfd);
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

   

    if (result == -1) {
        // actual error
        epoll_ctl(epfd, EPOLL_CTL_DEL, con->fd, NULL);
        close(con->fd);

        free(con);
        free(data);

        return;
    }

    // result == 1 means we drained until EAGAIN
    
        // we will keep parsing until line is not fully processed or we will parse until the request has
        // been fully parsed
        result = parse_http(con);
        
        // result is -1 or result ==0 means request not enough data yet
        if(result ==-1){
            return;
        }
    

   
    char * status_string = NULL;

    memset(con->write_buf,0,sizeof(con->write_buf));

    http_response  * response = build_http_response(con->request->uri);
     if(response->http_status ==200){
                    status_string = "Ok";
                }
                else if(response->http_status==404){
                    status_string ="Not Found";
                }

    // write the line straight into 
    int size_written = snprintf(
    con->write_buf,
    sizeof(con->write_buf),
    "HTTP/1.0 %d %s\r\n"
    "Content-Length: %jd\r\n"
    "Content-Type: %s\r\n"
    "\r\n",
    response->http_status,
    status_string,
    (off_t)response->content_length,
    response->content_type
);

memcpy(con->write_buf + size_written,response->body,response->content_length);

    
    
         result = handle_write(con);
        // 
        if(result ==-1){
            return;

        }

        if(result ==1){
            // create an event with 
            struct epoll_event event = {0};
            event.events =  EPOLLOUT;
           // the callback would be the same to connection callback
            
            event.data.ptr = data;



        if (epoll_ctl(epfd,EPOLL_CTL_MOD,con->fd,&event) == -1)
    {
        perror("epoll_ctl MOD");
       return;
    }

    return;

  
}





            
        

    }



    // write the result

    


    else if(events&(EPOLLERR |EPOLLHUP)){
        fprintf(stderr,"error on listening socket");
        return ;
    }

   else if (events & EPOLLOUT) {

    int result = handle_write(con);

    if (result == -1) {
        // actual write error
        epoll_ctl(epfd, EPOLL_CTL_DEL, con->fd, NULL);
        close(con->fd);

        free(con);
        free(data);

        return;
    }

    if (result == 0) {
        /*
         * Finished writing.
         *
         * We don't need EPOLLOUT anymore.
         */
        struct epoll_event event = {0};

        event.events = EPOLLIN;
        event.data.ptr = data;

        if (epoll_ctl(
                epfd,
                EPOLL_CTL_MOD,
                con->fd,
                &event) == -1) {

            perror("epoll_ctl MOD");
        }

        return;
    }

    /*
     * result == 1:
     * write() hit EAGAIN again.
     *
     * EPOLLOUT is already enabled.
     * Just return to epoll_wait().
     */
    return;
}




}

