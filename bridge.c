#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>



#define die(e) do { fprintf(stderr, "%s\n", e); exit(EXIT_FAILURE); } while (0)
#define SZ 4096

int init_val = 4096;
char *ret_data;
char *ch_result;
int in_grow = 0;

/*
 * @return string
 * executes clisp and lisp file and return the result
*/

 
char *pmain() {
  int link[2];
  pid_t pid;
  if (pipe(link)==-1)
    die("pipe");

  if ((pid = fork()) == -1)
    die("fork");

  if(pid == 0) {
    fflush(stdout);
    dup2 (link[1], STDOUT_FILENO);
    close(link[0]);
    close(link[1]);
    execl("/usr/bin/clisp", "clisp", "./CAPTCHA/cd.lisp",NULL);
    die("execl");
    
    wait(NULL);


  } else {
    close(link[1]);
    ret_data = (char *)malloc(sizeof(char) * SZ);
    int nbytes = read(link[0], ret_data, init_val);
    in_grow = in_grow + nbytes;
    while(nbytes){
		nbytes = read(link[0], ret_data + in_grow, init_val - in_grow);
		in_grow = in_grow + nbytes;
		
	}
	ch_result = (char *)malloc(sizeof(char) *(in_grow + 1));
	strncpy(ch_result , ret_data, in_grow);
	ch_result[in_grow] = '\0';
	in_grow = 0;
    free(ret_data);
    wait(NULL);
    close(link[0]);

  }
  return ch_result;
}
