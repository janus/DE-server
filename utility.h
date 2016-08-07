#ifndef UTILITY_H_
#define UTILITY_H_

#include <stdio.h>
#include <stdlib.h>

#define MLENGTH 4
#define URLLENGTH 40

struct string_query {
	char *req_name;
	char *req_value;
	struct string_query  *next;
};

struct request_data {
	char method[MLENGTH];
	char url[URLLENGTH];
	//struct string_query *dmessage;
	struct string_query *meta_data;
};

typedef struct request_data requested_data;

typedef struct string_query str_query;

char *MESSASE_TO_CLIENT = "HTTP/1.0 200 OK\r\n"
                  "DE server\r\n"
                  "Content-Type: text\r\n"
                  "\r\n";
                  

#endif
