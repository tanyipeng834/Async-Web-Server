#include "network.h"

int open_listenfd(char* port)
{
    struct addrinfo hints,*listp,*p;
    int listenfd,optval=1;
    memset(&hints,0,sizeof(struct addrinfo));
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE | AI_ADDRCONFIG;
    hints.ai_flags |= AI_NUMERICSERV;
    getaddrinfo(NULL,port,&hints,&listp);

    for(p =listp; p; p=p->ai_next){
        if ((listenfd = socket(p->ai_family, p->ai_socktype,
                           p->ai_protocol)) < 0)
        continue;  

        setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR,
               (const void *)&optval , sizeof(int));
               if (bind(listenfd, p->ai_addr, p->ai_addrlen) == 0)break;
    close(listenfd);

    }
    freeaddrinfo(listp);
    if (!p) /* No address worked */
    return -1;
    /* Make it a listening socket ready to accept conn. requests */
    if (listen(listenfd, LISTENQ) < 0) {
        close(listenfd);
        return -1; }

    int flags = fcntl(listenfd,F_GETFL,0);
    if(flags==-1){
        perror("fctnl F_GETFL");
        return -1;
    }
    if(fcntl(listenfd,F_SETFL,flags|O_NONBLOCK)){
        perror("fcntl F_SETFL");
        return -1;
    }
    return listenfd;


    
}


int accept_connection(int listenfd)
{
    char strbuf[INET_ADDRSTRLEN];
    struct sockaddr_in client_connection;

    socklen_t client_len = sizeof(struct sockaddr_in);

    memset(
        &client_connection,
        0,
        sizeof(struct sockaddr_in)
    );

    int connfd =-1;
    while(1){

     connfd = accept(
        listenfd,
        (struct sockaddr *)&client_connection,
        &client_len
    );

    if (connfd < 0) {
        if(errno ==EINTR){
            continue;

        }
       return -1;
        
    }
    break;
}

int flags = fcntl(connfd,F_GETFL,0);
    if(flags==-1){
        perror("fctnl F_GETFL");
        return -1;
    }
    if(fcntl(connfd,F_SETFL,flags|O_NONBLOCK)){
        perror("fcntl F_SETFL");
        return -1;
    }

    inet_ntop(
        AF_INET,
        &(client_connection.sin_addr),
        strbuf,
        sizeof(strbuf)
    );

    printf("Client Ip Addr: %s\n", strbuf);

    return connfd;
}

void close_connection(connection *con,
                      event_handler *ev,
                      int epfd)
{
    if (con == NULL)
        return;

    if (epoll_ctl(epfd, EPOLL_CTL_DEL, con->fd, NULL) == -1)
        perror("epoll_ctl delete");

    close(con->fd);

    free_http_request(con->request);
    free_http_response(con->response);

    free(con);
    free(ev);
}


