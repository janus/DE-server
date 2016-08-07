#include <stdio.h>
#include <stdlib.h>

void error_message(char *message){
	printf("%s", message);
	abort();
}
