#ifndef IO_H
#define IO_H
#include "network.h"
#include "http_request.h"
#include <errno.h>
#include <sys/epoll.h>
typedef struct _connection connection;
int handle_read(connection* con);
int handle_write(connection * con);














#endif
