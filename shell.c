#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/wait.h>
#include "utils.h"

#define MAX_PATHNAME_SIZE 1000

int exec_command(char**);
void init(char**,char***);
void pwd(void);
void cd(char*);
void exec_bins(char**);

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
        exec_bins(tokens);
        return 1;
    
    }
}

void exec_bins(char **tokens)
{
    int wstatus;
    int pid=fork();
    if(pid==0)
    {
        if(execvp(tokens[0],tokens)==-1)
        perror(tokens[0]);
    }
    else if(pid < 0)
    {
        fprintf(stderr,"Error occured while forking\n");
    }
    else
    {
        wait(&wstatus);
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
