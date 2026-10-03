#ifndef FILE_UTILS
#define FILE_UTILS

#define SERVER_STATIC "./www"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#define PATH_MAX 4096

#define CRLF "/r/n";
char * build_file_path(char* uri);

char * serve_static_file(char* system_file_string);


const char * parse_file_extension(char * system_file_string);

off_t get_file_size(char * system_file_string);







#endif
