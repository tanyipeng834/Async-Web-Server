#include "file_utils.h"



char * build_file_path(char* uri)
{
    // this is to produce the memory required for the file path
    size_t file_path_len = strlen(uri)+strlen(SERVER_STATIC)+1;
    char* file_path = malloc(file_path_len);
    snprintf(file_path,file_path_len,"%s%s",SERVER_STATIC,uri);


    return file_path;






}

// function returns -1 which would indicate
// that the static file does not exist
off_t get_file_size(char * system_file_string)
{   
    int srcfd;
    struct stat file_stat;
    srcfd = open(system_file_string,O_RDONLY,0);
    if(srcfd==-1){
        perror("Open Error");
        return -1;
    }

    if(fstat(srcfd,&file_stat)==-1)
    {
        perror("Error with stat");
        return -1;

    }
    close(srcfd);


    return file_stat.st_size;







}


char * serve_static_file(char * system_file_string)
{
    int srcfd;
    char * srcp;
    size_t file_size = get_file_size(system_file_string);

    
    
    srcfd = open(system_file_string,O_RDONLY,0);
    // check the status of the system call
    if(srcfd ==-1){
        return NULL;
    }

    srcp = malloc(file_size);
    ssize_t read_bytes = read(srcfd,srcp,file_size);
    if(read_bytes==-1){
        perror("Read Error in serve static file\n");
    }
    close(srcfd);

  
    return srcp;

}


// data can be stored in .rodata section
const char * parse_file_extension(char * uri)
{
    char * file_ext = strrchr(uri,'.');

    if(file_ext ==NULL){
        return "application/octet-stream";
    }
    if(strcmp(file_ext,".html") ==0 || strcmp(file_ext,".htm")==0){
        return "text/html";
    }

    else if(strcmp(file_ext,".jpg")==0 || strcmp(file_ext,".jpeg")==0){
        return "image/jpeg";
    }

    return NULL;



}

 

