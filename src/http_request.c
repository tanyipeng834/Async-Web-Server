#include "http_request.h"




int parse_http(connection *con)
{
    while (con->read_offset < con->read_len) {

        char *start =
            con->read_buf + con->read_offset;

        size_t remaining =
            con->read_len - con->read_offset;

        char *line_end =
            find_crlf(start, remaining);
        // not complete request
        if(line_end==NULL){
            return 1;
        }

       
        

        size_t line_len = line_end - start;


       
        if (con->http_state == PARSE_REQUEST_LINE) {

            if (line_len == 0) {
                return -1;  // malformed request
            }

            con->request =
                parse_request_line(start, line_len);

            if (con->request == NULL) {
                return -1;
            }

            con->read_offset += line_len + 2;

            con->http_state = PARSE_HEADERS;

            continue;
        }

        if (con->http_state == PARSE_HEADERS) {

            
           
            if (line_len == 0) {

                con->read_offset += 2;

                con->http_state = PARSE_DONE;

                return 0;
            }

           
            con->read_offset += line_len + 2;

            continue;
        }
    }

    return 1;
}


http_request* create_http_request()
{
    http_request * request = calloc(1,sizeof(http_request));
    return request;

}


http_request* parse_request_line(char * start,size_t line_len){
    http_request * request = create_http_request();
    //adding 1 to include the null terminator
    char request_line[line_len+1];
    memcpy(request_line,start,line_len);
    // add the null terminator
    request_line[line_len]='\0';
    char ** http_request_params = malloc(sizeof(char*)*3);
    split_string(request_line,http_request_params,' ');

    

    if(strcmp(http_request_params[1],"/")==0){
        http_request_params[1] ="/index.html";
    }

    request->method = http_request_params[0];
    request ->uri = http_request_params[1];
    request ->version = http_request_params[2];




    return request;











}

void free_http_request(http_request * http_request){



    free(http_request->method);
    free(http_request->uri);
    free(http_request->version);
    free(http_request);

}
