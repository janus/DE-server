
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 */

#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include "utility.h"
#include "route.c"
#include "tokenize.c"
#include "bridge.c"




#define PORT 4775
#define ADDRESS "127.0.0.1"
#define START  "start"
#define  STOP  "stop"
#define LISTENQ 4

typedef struct sockaddr SA;
//#define INET6_ADDRSTRLEN 3000
requested_data *re_data;

int switch_ch;
struct sockaddr_in serveraddr;

int start_server(int port){
	int listenid;
	int optval=1;
	typedef struct sockaddr SA;
	if((listenid = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		error_message("Error in startServer <Failed to create socket>");
	}
	if (setsockopt(listenid, SOL_SOCKET, SO_REUSEADDR,
                   (const void *)&optval , sizeof(int)) < 0)
            return -1;
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons((unsigned short)port);
	if(bind(listenid, (SA*)&serveraddr, sizeof(serveraddr)) < 0) {
		error_message("Error in startServer <Failed to bind socket>");
	}
	if(listen(listenid, LISTENQ) < 0) {
		error_message("Error in startServer <Failed to listen>");
	}
	
	
	return listenid;
}
	

void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

void process_server(int listenid){
	char remoteIP[INET6_ADDRSTRLEN];
	int fd;
	int read_size;
	char *client_message = (char *)malloc(sizeof(char) * 1000);
	struct sockaddr_storage remoteaddr; // client address
	socklen_t addrlen;
	addrlen = sizeof remoteaddr;
	char *gen;
	int gen_int = 0;
	int supplied_answer;
    while((fd = accept(listenid, (struct sockaddr *)&remoteaddr, &addrlen))) {
		while((read_size = recv(fd, client_message, 1000, 0)) > 0) {
			printf("%s", client_message);
            re_data = router(client_message, read_size);
            printf("Method %s --url::%s=+++\n", re_data->method, re_data->url);
            switch_ch = 1;
            break;
				
		}
		printf("DE server: new connection from %s on "
                            "socket %d\n",
                            inet_ntop(remoteaddr.ss_family,
                                get_in_addr((struct sockaddr*)&remoteaddr),
                                remoteIP, INET6_ADDRSTRLEN),fd);
		str_query *rem_data;
		str_query *header = re_data->meta_data;
	     while(header){
			if(strncasecmp(re_data->method, "PUT", 3) == 0){
				if(strncasecmp(re_data->url, "/answer", 7) == 0){
					if(strlen(re_data->url) == 7){
						if(strncasecmp(header->req_name, "question", 8) == 0){
							
							int res_int = parse_input(header->req_value);
							
							if(gen_int == res_int && supplied_answer == res_int){
								send(fd, "HTTP status 200 OK" , 18, 0);
							} else {
								send(fd, "400 Bad Request" , 15, 0);
							}
							
							printf("\n Resulted from computation  of %s  result :: %d\n", header->req_name, res_int);
							
						}
					}
					
				}
			}
			if(switch_ch){
				if(strncasecmp(re_data->url, "/question" , 9) == 0){
					if(strncasecmp(re_data->method, "GET", 3) == 0){
						gen = pmain();
						send(fd, gen, strlen(gen), 0);
						gen_int = parse_input(gen);
						switch_ch = 0;
						printf("\n Resulted from generated computation  of %s  result :: %d\n", header->req_name, gen_int);
						free(gen);
					}
				}
			}
			if(strncasecmp(header->req_name, "answer", 8) == 0){
				supplied_answer = atoi(header->req_value);
				
			}
			
            printf("\nname %s    value %s\n", header->req_name, header->req_value);
			rem_data = header;
			header = header->next;
			free(rem_data);
		 
	    }		
        free(re_data);
        close(fd);
	}
	return ;
	
}


int main(int argc, char **argv) {
	int port, pid, server_conn = 1;
	int internet_address_len = 15;
	char serAdd[internet_address_len];

	char strAction[7];
	if(argc == 1) {
		port = PORT;
		strcpy(serAdd, ADDRESS);
	} else if(argc == 2){
		port = atoi(argv[1]);
		strcpy(serAdd, ADDRESS);
	} else if(argc == 3) {
		port = atoi(argv[1]);
		int n = strlen(argv[2]);
		strncpy(serAdd, argv[2], n);
		if(n >= internet_address_len){
			error_message("Internet address is incorrect");
		}
		serAdd[n] = '\0';
	}
	printf("\n\tDE is a basic server built to carry out simple things\n"
	        "\t-----	-----\n"
	        "\t|    |	|\n"
	        "\t|    |	-----\n"
	        "\t|    |	|\n"
	        "\t-----	-----\n"
	       "\tMethods implemented [Get & Put]\n"
	       "\tTo start type start\n"
	        "\tTo stop type stop\nDE]");
	scanf("%s", strAction);
	if (strcmp(strAction, START) == 0){
		printf("Server %sed\n", START);
		int listenid = start_server(port);
		printf("listen on port %d & listening is %d\nDE]", port,listenid);
		signal(SIGPIPE, SIG_IGN);
		while(server_conn) {
			pid =fork();
			if(pid == 0) {
				process_server(listenid);
				break;				
			}
			scanf("%s", strAction);
			if (strcmp(strAction, STOP) == 0) {
				close(listenid);
				return 0;
			}

		}
		
	}

	return 0;
	
}
	
