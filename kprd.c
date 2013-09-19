#include "kprd.h"

void init_daemon(const char *path,mode_t mask)
{
	if(fork() > 0)
		_exit(0);
	if(fork() > 0)
		_exit(0);

	setsid();
	chdir(path);
	umask(mask);
	
	close(STDOUT_FILENO);
	close(STDERR_FILENO);
	close(STDIN_FILENO);
}

void start_process(KPR *kpr)
{
	pid_t pid;
	char status='\0';
	int pipefd[2];
	fd_set reads;
	struct timeval timeout;

	timeout.tv_sec=0;
	timeout.tv_usec=1000;
	pipe(pipefd);
	while(kpr->next != NULL)
	{
		kpr=kpr->next;
		pid=fork();
		if(pid == 0)
		{
			if(fork() == 0)
				if(execl(kpr->path,kpr->name,kpr->arg,NULL) == -1)
				{
					char status='\1';

					write(pipefd[1],&status,sizeof(char));
				}
			wait(NULL);
			_exit(0);
		}
		else
		{
			FD_ZERO(&reads);
			FD_SET(pipefd[0],&reads);
			if(select(pipefd[0]+1,&reads,NULL,NULL,&timeout) > 0)
				read(pipefd[0],&status,sizeof(char));

			if(!status)
				kpr->pid=pid;
			else
				kpr->pid=-1;
		}
	}

	close(pipefd[1]);
	close(pipefd[2]);
}

void kpr_run(KPR *kpr)
{
	pid_t pid;

	while(1)
	{
		pid=wait(NULL);

		if(pid == -1)
			pause();
		else
			process_restart(kpr,pid);
	}
}

void process_restart(KPR *kpr,pid_t pid)
{
	pid_t new_pid;
	char status='\0';
	fd_set reads;
	int pipefd[2];
	struct timeval timeout;

	timeout.tv_sec=0;
	timeout.tv_usec=1000;
	pipe(pipefd);
	while(kpr->next != NULL)
	{
		kpr=kpr->next;

		if(kpr->pid == pid)
		{
			new_pid=fork();

			if(new_pid == 0)
			{
				char status='\1';

				if(execl(kpr->path,kpr->name,kpr->arg,NULL) == -1)
					write(pipefd[1],&status,sizeof(char));
			}
			else
			{
				FD_ZERO(&reads);
				FD_SET(pipefd[0],&reads);
				if(select(pipefd[0]+1,&reads,NULL,NULL,&timeout) > 0)
					read(pipefd[0],&status,sizeof(char));

				if(!status)
					kpr->pid=new_pid;
				else
					kpr->pid=-1;
			}

			close(pipefd[1]);
			close(pipefd[0]);
			return;
		}
	}

	close(pipefd[1]);
	close(pipefd[0]);
}
