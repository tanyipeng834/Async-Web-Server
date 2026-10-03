#include "http_response.h"




http_response * create_http_response(){
    http_response * response = calloc(1, sizeof(http_response));
    return response;
}
