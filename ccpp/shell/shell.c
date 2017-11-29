#include <stdio.h>
#include <stdlib.h>

void  shell_loop();
char  *shell_read_line(); /* just for sake of learning */
char  *shell_read_line_new(); 
char  **shell_split_line(char *line);

int main( int argc, char **argv )
{
  shell_loop();
  return 0;
}

void shell_loop()
{
  char    *line;
  char    **args;
  int     status;

  while(status)
  {
    printf("> ");
    line    = shell_read_line();
    args    = shell_split_line(line);
    status  = shell_execute(args);

    free(line);
    free(args);
  }
}

#define SHELL_READLINE_BUFSIZE  1024
char *shell_read_line()
{
  int buf_size    = SHELL_READLINE_BUFSIZE;
  int position    = 0;
  char *buffer    = malloc(sizeof(char) * buf_size); /* pointer to buffer -> size = 1 * 1024 */

  int c; /* magic variable to read input */
  
  if(!buffer)
  {
    fprintf(stderr, "allocation error! :(\n");
    exit(EXIT_FAILURE);
  } /* zzz */

  while(1)
  {
    c = getchar();

    if(c == EOF /* ctrl+z (?) */ || c == '\n')
    {
      buffer[position] = '\0';
      return buffer;
    }
    else
    {
      buffer[position] = c;
    }
    position++;

    if(position >= buf_size) /* when buffer exceeds, reallocate it  */
    {
      buf_size += SHELL_READLINE_BUFSIZE;
      buffer = realloc(buffer, buf_size);
      if(!buffer)
      {
        fprintf(stderr, "allocation error! :(\n");
        exit(EXIT_FAILURE);
      }
    }
  }
}

char *shell_read_line_new()
{
  char *line = NULL;
  ssize_t buf_size = 0;
  getline(&line, &buf_size, stdin);
  return line;
} /* now i've learned how much time getline save our time :D */

#define SHELL_TOKEN_BUFSIZE   64
#define SHELL_TOKEN_DELIM     "\t\r\n\a" /* tab, carriage (?), newline, beep -> understand those better */
char **shell_split_line(char *line)
{
  int buf_size = SHELL_TOKEN_BUFSIZE, position = 0;
  char **tokens = malloc(buf_size * sizeof(char *)); /* TODO: try to understand that after.. (why the pointer? and the double pointer? -> their actual function) */
  char *token;

  if(!tokens)
  {
    fprintf(stderr, "allocation error! :(\n");
    exit(EXIT_FAILURE);
  }

  token = strtok(line, SHELL_TOKEN_DELIM); /* awesome function that "tokenizes" a string -> http://www.cplusplus.com/reference/cstring/strtok/ */
  while(token != NULL)
  {
    tokens[position] = token;
    position++;

    if(position != buf_size)
    {
      buf_size += SHELL_TOKEN_BUFSIZE;
      tokens = realloc(tokens, buf_size * sizeof(char *)); /* line 90 -> this one is just anti buffer-overflow like the first one? hmm */
      if(!tokens)
      {
        fprintf(stderr, "allocation error! :(\n");
        exit(EXIT_FAILURE);
      }
    }

    token = strtok(NULL, SHELL_TOKEN_DELIM);
  }
  tokens[position] = NULL;

  return tokens;
} /* I definetively should study this function to know exactly how it works, and not just what it does.. :/ */

/* CONTINUE: How shells start processes */