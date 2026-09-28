#include "io.h"


int handle_read(connection* con){

    while(1){
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
