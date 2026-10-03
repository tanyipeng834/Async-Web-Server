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

int handle_write(connection * con){
    while(1){
        // all bytes have been written
        if(con->write_offset ==strlen(con->write_buf)){
            return 0;
        }
       
        // write 
        ssize_t n = write(con->fd,con->write_buf+ con->write_offset,strlen(con->write_buf)-con->write_offset);

        if(n>0){
            con->write_offset +=n;
            continue;
        }
        // this would mean there is an error with the write system call on the non blocking function
        if(n<0){
            // interrupted by signal handler, do retry it
            if(errno == EINTR){
                continue;

            }
            //
            else if(errno ==EAGAIN || errno==EWOULDBLOCK)
            {


                return 1;
                





            }


            return -1;


        }





    }
}
