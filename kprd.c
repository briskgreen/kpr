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
