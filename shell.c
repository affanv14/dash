#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<termios.h>
#include "utils.h"

#define MAX_PATHNAME_SIZE 1000
#define MAX_PROCESSES 10000
struct filed
{
    int fd;
    
    int nfd;
};

int exec_command(char**);
void init(char**,char***);
void pwd(void);
void cd(char*);
void exec_bins(char**,struct filed *);
struct filed *checkio(char**);
int push(pid_t);
pid_t pop(void);
void save_process(int);
void restart_process(void);
void kill_all_processes(void);
void wait_and_continue(pid_t);

pid_t shell_pgid;
struct termios shell_tmodes;
int shell_terminal;
int shell_is_interactive;
int pstack[MAX_PROCESSES];
int stackptr;

void main()
{
    char *line;
    char **tokens;
    int status=1;
    init(&line,&tokens);
    while(status)
    {
        printf("shell:");
        readline(line);
        parseline(line,tokens);
        if(tokens[0]==NULL)
        {
            status=1;
            continue;
        }
        status=exec_command(tokens);
    }
    free(tokens);
    free(line);

}

void init(char **line,char ***tokens)
{
    shell_terminal=STDIN_FILENO;
    shell_is_interactive=isatty(shell_terminal);
    if(shell_is_interactive)
    {
        while(tcgetpgrp(shell_terminal)!=(shell_pgid =getpgrp()))
            kill(-shell_pgid,SIGTTIN);

        change_all_signals(SIG_IGN);
        shell_pgid=getpid();
    
        if(setpgid(shell_pgid,shell_pgid)<0)
        {
            perror("Could no put shell in its process group\n");
            exit(0);
        }
        tcsetpgrp(shell_terminal,shell_pgid);
        tcgetattr(shell_terminal,&shell_tmodes);
    }
    stackptr=-1;
    *line=(char*)malloc(sizeof(char)*BUFFER_SIZE);
    *tokens=(char **)malloc(sizeof(char*)*NUM_TOKENS);
}
int exec_command(char **tokens)
{
    struct filed *file_descriptor=checkio(tokens);
    if(strcmp(tokens[0],"exit")==0)
    {
        return 0;
    }
    else if(strcmp(tokens[0],"pwd")==0)
    {
        pwd();
        return 1;
    }
    else if(strcmp(tokens[0],"fg")==0)
    {
        restart_process();
        return 1;
    }
    else if(strcmp(tokens[0],"cd")==0)
    {
        cd(tokens[1]);
        return 1;
    }
    else
    {
        exec_bins(tokens,file_descriptor);
        return 1;
    
    }
}

void exec_bins(char **tokens,struct filed* file_descriptor)
{
    int pid=fork();
    if(pid==0)
    {
        change_all_signals(SIG_DFL);
        setpgid(0,0);
        if(file_descriptor!=NULL)
        {
            
            dup2(file_descriptor->fd,file_descriptor->nfd);
            close(file_descriptor->fd);
        }
        if(execvp(tokens[0],tokens)==-1)
            perror(tokens[0]);
    }
    else if(pid < 0)
    {
        fprintf(stderr,"Error occured while forking\n");
    }
    else
    {
        setpgid(pid,pid);
        wait_and_continue(pid);
        if(file_descriptor!=NULL)
            free(file_descriptor);
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

struct filed *checkio(char **tokens)
{
    int i=0;
    struct filed *cfd=malloc(sizeof(struct filed));
    while(tokens[i]!=NULL)
    {
        if(strcmp(tokens[i],">")==0)
        {
            cfd->fd=open(tokens[i+1],O_CREAT|O_TRUNC|O_WRONLY,0644);
            if(cfd->fd<0)
                perror("open");
            cfd->nfd=STDOUT_FILENO;
            tokens[i]=NULL;
            return cfd;
        }
        if(strcmp(tokens[i],"<")==0)
        {
            cfd->fd=open(tokens[i+1],O_RDONLY,0644);
            if(cfd->fd<0)
                perror("open");
            cfd->nfd=STDIN_FILENO;
            tokens[i]=NULL;
            return cfd;
        }
        i++;
    }
    return NULL;
}

int push(pid_t pid)
{
    if(stackptr>=MAX_PROCESSES-1)
        return -1;
    pstack[++stackptr]=pid;
    return 0;
}

pid_t pop()
{
    if(stackptr<0)
        return -1;
    else
        return pstack[stackptr--];
}

void save_process(int x)
{
    push(x);
}

void restart_process()
{
    pid_t pid=pop();
    if(pid<0)
        fprintf(stderr,"No stopped processes\n");
    kill(pid,SIGCONT);
    wait_and_continue(pid);
}

void kill_all_processes()
{
    pid_t pid=pop();
    while(pid>=0)
    {
        kill(pid,SIGQUIT);
    }
}

void wait_and_continue(pid_t pid)
{
    int wstatus;
    tcsetpgrp(shell_terminal,pid);
    waitpid(pid,&wstatus,WUNTRACED);
    if(WIFSTOPPED(wstatus))
    {
        save_process(pid);
    }
    tcsetpgrp(shell_terminal,getpid());
}
