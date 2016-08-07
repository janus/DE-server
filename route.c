
#include "utility.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "derror.c"


str_query *head = NULL;
str_query *tail = NULL;

str_query *header; 
str_query *tailer; 

str_query *ntemp;

int stag_set = 0;
void add_message(str_query *message) {
	if(!head){
		head = message;
		tail = message;
	} else {
		tail->next = message;
		tail = tail->next;
	}
	return;
}

void add_header_message(str_query *message) {
	if(!header){
		header = message;
		tailer = message;
		stag_set = 1;
	} else {
		tailer->next = message;
		tailer = tailer->next;
	}
	return;
}	

void create_message(const char *name, const char *value, int nlen, int vlen, int choice){
	ntemp = (str_query *)malloc(sizeof(str_query));
	ntemp->req_name = (char *)malloc(sizeof(char) * nlen);
	ntemp->req_value = (char *)malloc(sizeof(char) * vlen);
	strncpy(ntemp->req_name, name, nlen);
	ntemp->req_name[nlen] = '\0';
	strncpy(ntemp->req_value, value, vlen);
	ntemp->req_value[vlen] = '\0';
	if(!choice){
		add_header_message(ntemp);
	} else{
		add_header_message(ntemp);
	}
	 
	return;
	
}

/*
 * @params: string and its size (integer)
 * @return: a pointer to struct
 * Resolves request header and url
*/


requested_data *router(char *message, int siz){
	char *temp;
	int j = 0;
	char c; 
	header = NULL;
	tailer = NULL;
	str_query *query;
	int method_size = 3;
	requested_data *ndata = (requested_data *)malloc(sizeof(requested_data));
	//ndata->dmessage = NULL;
	ndata->meta_data = NULL;
	int i = 0;
	while((c = *message ) != '\0'){
		while(isspace(*message )) { message++;}
		if(strncasecmp(message, "GET", method_size) == 0){
			strncpy(ndata->method, "GET", method_size);
			ndata->method[method_size] = '\0';
			message = message + 3;
			i = i + 3;
			continue;
		}
		if(strncasecmp(message, "PUT", method_size) == 0) {
			strncpy(ndata->method, "PUT", method_size);
			ndata->method[method_size] = '\0';
			message = message + 3;
			i = i + 3;
			continue;
		}
		if(c == '/') {
			temp = (char *)malloc(sizeof(char)*200);
			temp[j++] = c;
			i++;
			message++;
			while((c = *message) != ' '){
				if(isalnum(c)) {
					temp[j++] = c;
				} else if (c == '?'){
					strncpy(ndata->url, temp, j);
					ndata->url[j] = '\0';
					j = 0;
					break;
				}
				message++;
				i++;
			}
			if(j != 0){
				strncpy(ndata->url, temp, j);
				ndata->url[j] = '\0';
				j = 0;
				continue;
			}
		}
		if(c == '?'){
			temp = (char *)malloc(sizeof(char)* 100);
			i++;
			int state_check = 0;
			message++;
			j = 0;
			query = (str_query *)malloc(sizeof(str_query));
			while((c = *message) != ' '){
				if(isalnum(c)) {
					temp[j++] = c;
				} else if(c == '='){
					query->req_name = (char *)malloc(sizeof(char) * j);
					strncpy(query->req_name, temp, j);
					query->req_name[j] = '\0';
					temp = (char *)malloc(sizeof(char)* 200);
					j = 0;
					state_check = 1;
				} else if(c == '&' && state_check){
					query->req_value = (char *)malloc(sizeof(char) * j);
					strncpy(query->req_value, temp, j);
					query->req_value[j] = '\0';
					temp = (char *)malloc(sizeof(char)* 200);
					add_message(query);
					state_check = 0;
					j = 0;
					query = (str_query *)malloc(sizeof(str_query));
					
				}
				message++;
				i++;
				
			}
			if(j != 0){
				query->req_value = (char *)malloc(sizeof(char) * j);
				strncpy(query->req_value, temp, j);
				query->req_value[j] = '\0';
				add_message(query);
				j = 0;
				continue;
			}
			
		} 
		
		if(strncasecmp(message, "HTTP", 4) == 0){
			temp = (char *)malloc(sizeof(char) * 100);
			strncpy(temp, "HTTP", 4);
			message = message + 4;
			c = *message;
			int n = 4;
			while(c != '\n'){
				temp[n++] = c;
				c = *message++;
			}
			create_message("proto", temp, 5, n, 0);
			free(temp);
		}
		if(strncmp(message, "Host", 4) == 0){
			temp = (char *)malloc(sizeof(char) * 100);
			message = message + 4;
			c = *message++;
			int n = 0;
			if (c == ':'){
				c = *message++;
				while(c == ' ') {c = *message++;}
				while(!isspace(c)){
					temp[n++] = c;
					c = *message++;
				}
				create_message("Host", temp, 4, n, 0);
				free(temp);
			}
		}
		if(strncasecmp(message, "User-Agent", 10) == 0){
			temp = (char *)malloc(sizeof(char)* 100);
			message = message + 10;
			c = *message++;
			j = 0;
			if(c == ':'){
				c = *message++;
				while(isspace(c)){ c = *message++;}
				while(!isspace(c)){
					temp[j++] = c;
					c = *message++;
				}
			}
			create_message("User-Agent", temp, 10, j, 0);
			j = 0;
			free(temp);
			
		}
		if(strncasecmp(message, "Accept:", 7) == 0){
			message = message + 7;
			i = i + 7;
			c = *message++;
			while(c != '\n'){
				c = *message++;
				i++;
			}
		}
		if(strncasecmp(message, "Content-Length:", 15) == 0) {
			message = message + 15;
			i = i + 15;
			c = *message++;
			while(c != '\n'){
				c = *message++;
				i++;
			}
		}
		if(strncasecmp(message, "Content-Type:", 13) == 0) {
			message = message + 13;
			i = i + 13;
			c = *message++;
			while(c != '\n'){
				c = *message++;
				i++;
			}
		}
		if(isalnum(c)){
			char *nntp = (char *)malloc(sizeof(char) * 100);
			j = 0;
			i++;
			nntp[j++] = c;
			int d = 0;
			int check_chge = 0;
			message++;
			while(!isspace(*message) && *message != '\0'){
				c = *message;
				if(c == '&') {
					create_message(nntp, temp, d, j, 1);
					check_chge = 0;
					j = 0;
					free(nntp);
					free(temp);
					nntp = (char *)malloc(sizeof(char) * 100);
				} else if(c == '='){
					d = j;
					j = 0;
					check_chge = 1;
					temp = (char *)malloc(sizeof(char) * 100);
				} else if(c != '=' && !check_chge){
					nntp[j++] = c;
				} else if(check_chge) {
					
					
					if(strncasecmp(message, "%20", 3) == 0){
						temp[j++] = ' ';
						message = message + 3;
						continue;
					} else {
						temp[j++] = c;
					}
				}
				message++;
				i++;
			}
			if(j != 0){
				create_message(nntp, temp, d, j, 1);
				free(temp);
				free(nntp);
				j = 0;
				d = 0;
			}
		}				
			
        if(isspace(c)){
			while(isspace(c)){ c = *message++;}
			message--;
		} else {
			message++;
		}
		i++;
					
	}
	printf("\nFirst second\n");
	ndata->meta_data = header;

	return ndata;
	
}

		

