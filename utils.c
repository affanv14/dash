#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include "utils.h"
#include<signal.h>
#define DELIM " "

char **parseline(char *args,char **tokens)
{
    int i=0;
    tokens[0]=strtok(args,DELIM);
    if(tokens[0]==NULL)
        return NULL;
    do
    {
        if(i>=(sizeof(tokens)/sizeof(char*))-1)
            tokens=realloc(tokens,sizeof(char*)*((sizeof(tokens)/sizeof(char*)+NUM_TOKENS)));
        tokens[++i]=strtok(NULL,DELIM);
    }while(tokens[i]!=NULL);
    tokens[i+1]==NULL;
    return tokens;

}

char *readline(char *line)
{
    fgets(line,BUFFER_SIZE,stdin);
    line[strcspn(line,"\n")]='\0';
    return line;
}

void change_all_signals(sighandler_t handler)
{
    signal(SIGINT,handler);
    signal(SIGQUIT,handler);
    signal(SIGTSTP,handler);
    signal(SIGINT,handler);
    signal(SIGTTIN,handler);
    signal(SIGTTOU,handler);
    signal(SIGCHLD,handler);
}
