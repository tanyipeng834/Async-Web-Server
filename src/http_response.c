#include "http_response.h"




http_response * create_http_response(){
    http_response * response = calloc(1, sizeof(http_response));
    return response;
}

http_response* build_http_response(char *uri){

    
    http_response * response = create_http_response();
    char * file_string = build_file_path(uri);

    ssize_t file_size = get_file_size(file_string);
    HTTP_STATUS http_status;

    if(file_size==-1){
        http_status = HTTP_NOT_FOUND;
        response->http_status =http_status;

    }

    else{
        
        
        http_status = HTTP_OK;
        response->http_status = http_status;
        const char * content_type = parse_file_extension(uri);
        response->content_type = strdup(content_type);
        char* body = serve_static_file(file_string);
        response->body = body;
        response->content_length= get_file_size(file_string);


    }



    // free the memory for the file_path
    free(file_string);
    return response;

}


void free_http_response(http_response * response)
{
    
    free(response->content_type);
    free(response->body);
    free(response);



}
