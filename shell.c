#include<stdio.h>
#include<unistd.h>
#include "utils.h"

#define MAX_PATHNAME_SIZE 1000

int exec_command(char**);
void init(char**,char***);
void pwd(void);
void cd(char*);
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
    else if(strcmp(tokens[0],"pwd")==0)
    {
            pwd();
            return 1;
    }
    else if(strcmp(tokens[0],"cd")==0)
    {
        cd(tokens[1]);
        return 1;
    }
    else
    {
            printf("command not found\n");
            return 1;
    
    }
}

void pwd()
{
    char *name=getcwd(NULL,MAX_PATHNAME_SIZE);
    printf("%s\n",name);
}

void cd(char* path)
{
    int status;
    status=chdir(path);
    if(status==-1)
        perror("cd");
}
