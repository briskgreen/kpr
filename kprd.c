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

	while(kpr->next != NULL)
	{
		kpr=kpr->next;
		pid=fork();
		if(pid == 0)
		{
			if(fork() == 0)
				execl(kpr->name,kpr->path,kpr->arg,NULL);
			wait(NULL);
			_exit(0);
		}
		else
			kpr->pid=pid;
	}
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

	while(kpr->next != NULL)
	{
		kpr=kpr->next;

		if(kpr->pid == pid)
		{
			new_pid=fork();

			if(new_pid == 0)
				execl(kpr->path,kpr->name,kpr->arg,NULL);
			else
				kpr->pid=new_pid;
			return;
		}
	}
}
