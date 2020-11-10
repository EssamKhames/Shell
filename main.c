#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#define BUFSIZE 1024
#define TOK_BUFSIZE 64
#define TOK_DELIM " \t\r\n\a"
FILE *fp;
// methods declaration
void signal_handler();
void loop(void) ;
char *readLine(void) ;
int execute(char **args) ;
int checkSymbol ( char ** args) ;

// write to log file whenever process terminates.
void signal_handler() {
    fp = fopen("log.txt", "w");
    fprintf(fp, "Child process was terminated  \n");
}

int main(int argc, char **argv)
{
  // open files
    fp = fopen("log.txt", "w");
    fclose(fp);

  // Run command loop.
   loop();

  // Perform any shutdown/cleanup.

  return 1 ;
}
void loop(void)
{
  int status = 1 ;


  while (status) {


  char * line = malloc(BUFSIZE * sizeof(char));
  char **args = malloc(BUFSIZE * sizeof(char*));
  int bufsize = TOK_BUFSIZE, position = 0;
  char *token = malloc(BUFSIZE * sizeof(char));
    printf(">>> ");

   //read line
    line = readLine();

   // split line
  if (!args) {
    fprintf(stderr, "allocation error\n");
    exit(0);
  }
  token = strdup(strtok(line ,TOK_DELIM));
  while (token != NULL) {
    args[position] = token;
    position++;
// avoid segmention error or core dumped
    if (position >= bufsize) {
      bufsize += TOK_BUFSIZE;
      args = realloc(args, bufsize * sizeof(char*));
      if (!args) {
        fprintf(stderr, "allocation error\n");
        exit(0);
      }
    }

    token = strtok(NULL, TOK_DELIM );
  }
  args[position] = NULL;

// excute line
   status = execute(args);

    free(line);
    free(args);
  }

}


char *readLine(void)
{
  int bufsize = BUFSIZE;
  int position = 0;
  char *buffer = malloc(sizeof(char) * bufsize);
  int c;

  if (!buffer) {
    perror("allocation error\n");
    exit(0);
  }

  while (1) {
    // Read a character
    c = getchar();

    // If we hit EOF replace it with a null character and return
    if (c == EOF || c == '\n') {
      buffer[position] = '\0';
      return buffer;
    } else {
      buffer[position] = c;
    }
    position++;

    // If we have exceeded the buffer reallocate avoiding core dumped :(
    if (position >= bufsize) {
      bufsize += BUFSIZE;
      buffer = realloc(buffer, bufsize);
      if (!buffer) {
        perror( "allocation error\n");
        exit(0);
      }
    }
  }
    return buffer ;
}



int execute(char **args)
{
//handler signal
  signal(SIGCHLD,signal_handler);

  pid_t pid, wpid;
  int status = 0  , n = 0 ,flag = 0;

  if (args[0] == NULL) {
    // empty command was entered.
    return 1;
  }

  if (strcmp( args[0], "exit" ) == 0){
     return 0 ;
  }

  // check if we have & symbol
  if(args[1] != NULL){
  char * Buffer = args[1] ;
  while ( Buffer[n] != '\0')
  {
    if( Buffer[n] == '&' ){
        flag = 1 ;
        Buffer[n] = NULL ;
        args[1] = Buffer ;
    }
    n++ ;
  }}

  pid = fork();
  if (pid == 0 ) {
    // Child process
    if (execvp(args[0], args) == -1) {
		printf("wrong command .. \n");
        exit(0) ;
    }
  } else if (pid < 0) {
    // Error forking
		perror("cannot execute command \n");
  }
  else {
    if(flag == 1){

    }else{
            do{
                wpid = waitpid(pid, &status, WUNTRACED);
            }while (!WIFEXITED(status) && !WIFSIGNALED(status));
        }
    }

  return 1;
}

