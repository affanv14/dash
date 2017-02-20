#include<stdio.h>
#include "utils.h"

int exec_command(char**);
void init(char**,char***);
void main()
{
    char *line;
    char **tokens;
    int status=1;
    init(&line,&tokens);
    while(status)
    {
        printf("shell:");
        line=readline(line);
        tokens=parseline(line,tokens);
        if(tokens==NULL)
            continue;
        status=exec_command(tokens);
    }
    free(tokens);
    free(line);

}

void init(char **line,char ***tokens)
{

    *line=(char*)malloc(sizeof(char)*BUFFER_SIZE);
    *tokens=(char **)malloc(sizeof(char*)*NUM_TOKENS);
}
int exec_command(char **tokens)
{
    if(strcmp(tokens[0],"exit")==0)
            return 0;
    else
    {
            printf("command not found\n");
            return 1;
    
    }
}
