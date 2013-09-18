#include "kprd.h"

int main(int argc,char **argv)
{
	mode_t mask;
	KPR *kpr;
	char *path;

	if(argv[1] == NULL)
		path="/";
	else
		path=argv[1];
	if(argv[2] != NULL)
		sscanf(argv[2],"%o",&mask);
	else
		mask=0666;

	init_daemon(path,mask);

	kpr=kpr_open_config();
	if(kpr == NULL)
		_exit(-1);
	if(kpr_read_config(kpr) == -1)
		_exit(-1);

	start_process(kpr);
	kpr_run(kpr);

	kpr_close_config(kpr);
	return 0;

}
