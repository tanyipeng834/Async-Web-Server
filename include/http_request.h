
#ifndef HTTP_REQUEST_H
#define HTTP_REQUEST_H
#include "network.h"
#include "string_utils.h"
#include <stdlib.h>
typedef struct _connection connection;
typedef struct _http_request
{
    char * method;
    char * uri;
    char  *version;




} http_request;


int parse_http(connection * con);

http_request * create_http_request();
http_request* parse_request_line(char * start,size_t line_len);

void free_http_request(http_request * http_request);

#endif

