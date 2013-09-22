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
	int status=0;

	while(kpr->next != NULL)
	{
		kpr=kpr->next;
		pid=vfork();

		if(pid == 0)
		{
			if(execl(kpr->path,kpr->name,kpr->arg,NULL) == -1)
				status=1;
			_exit(-1);
		}

		else
		{
			if(status)
				kpr->pid=-1;
			else
				kpr->pid=pid;
		}
	}
}

void kpr_run(KPR *kpr)
{
	pid_t pid;
	int status;

	while(1)
	{
		pid=wait(&status);
		if(WIFEXITED(status))
		{
			if(WEXITSTATUS(status) != 0)
				clean_process(kpr,pid);
		}

		if(pid == -1)
			pause();
		else
			process_restart(kpr,pid);
	}
}

void process_restart(KPR *kpr,pid_t pid)
{
	pid_t new_pid;
	int status=0;

	while(kpr->next != NULL)
	{
		kpr=kpr->next;

		if(kpr->pid == pid)
		{
			new_pid=vfork();

			if(new_pid == 0)
			{

				if(execl(kpr->path,kpr->name,kpr->arg,NULL) == -1)
					status=1;
				_exit(-1);
			}
			else
			{
				if(!status)
					kpr->pid=new_pid;
				else
					kpr->pid=-1;
			}

			return;
		}
	}
}

void clean_process(KPR *kpr,pid_t pid)
{
	while(kpr->next != NULL)
	{
		kpr=kpr->next;

		if(kpr->pid == pid)
		{
			kpr->pid=-1;
			return;
		}
	}
}
