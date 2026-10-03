#ifndef HTTP_RESPONSE_H
#define HTTP_RESPONSE_H
#include <sys/types.h>
#include <stdlib.h>
typedef enum{
    HTTP_OK = 200,
    HTTP_NOT_FOUND = 404
} HTTP_STATUS;


typedef struct _HTTP_RESPONSE
{
    HTTP_STATUS http_status;
    off_t content_length;
    char * content_type;
    char * body;
}http_response;


http_response * create_http_response();


#endif


