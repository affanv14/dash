#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include "utils.h"
#define DELIM " "
//TODO:increase buffer size if needed
char **parseline(char *args,char **tokens)
{
    int i=0;
    tokens[0]=strtok(args,DELIM);
    if(tokens[0]==NULL)
        return NULL;
    do
    {
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
