#include "io.h"


int handle_read(connection* con){

    while(1){
        // cannot read cos buffer is full
        if(con->read_len == sizeof(con->read_buf)){
            return -1;

        }

        ssize_t n = read(con->fd,con->read_buf+con->read_len,sizeof(con->read_buf)-con->read_len);

        if(n>0){
            con->read_len +=n;
            continue;
            
        }

        if(n==0){
            return 0;
        }

        if(n<0){
            if(errno==EINTR){
                continue;
            }
            else if(errno ==EAGAIN || errno==EWOULDBLOCK){
                break;
            }
            return -1;
        }

       


    }
     return 1;

}

// status 

// 0 means all written
// 1 means 

int handle_write(connection *con)
{
    /*
     * FIRST: send HTTP headers
     */
    while (con->write_offset < con->write_len) {

        ssize_t n = write(
            con->fd,
            con->write_buf + con->write_offset,
            con->write_len - con->write_offset
        );

        if (n > 0) {
            con->write_offset += n;
            continue;
        }

        if (n < 0 && errno == EINTR) {
            continue;
        }

        if (n < 0 &&
            (errno == EAGAIN || errno == EWOULDBLOCK)) {
            return 1;
        }

        return -1;
    }


    /*
     * SECOND: send body
     */
    while (con->body_offset < con->response->content_length) {

        ssize_t n = write(
            con->fd,
            con->response->body + con->body_offset,
            con->response->content_length - con->body_offset
        );

        if (n > 0) {
            con->body_offset += n;
            continue;
        }

        if (n < 0 && errno == EINTR) {
            continue;
        }

        if (n < 0 &&
            (errno == EAGAIN || errno == EWOULDBLOCK)) {
            return 1;
        }

        return -1;
    }


    // Header AND body completely sent
    return 0;
}
