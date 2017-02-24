#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#define BUFFER_SIZE 2000
#define NUM_TOKENS 100

typedef void (*sighandler_t)(int);

char **parseline(char*,char**);
char *readline(char*);
void change_all_signals(sighandler_t);
