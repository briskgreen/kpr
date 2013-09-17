#include "kprd.h"

int main(int argc,char **argv)
{
	mode_t mask;
	KPR *kpr;
	char *path;

	if(argv[1] == NULL)
		path="/";
	if(argv[2] != NULL)
		sscanf(argv[2],"%o",&mask);

}
