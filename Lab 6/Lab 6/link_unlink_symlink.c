#include "type.h"
#include "string.h"
#include <libgen.h>

extern int fd, dev;
extern int ninodes, nblocks;
extern int bmap, imap, inode_start, iblock;

extern char line[256], cmd[32], pathname[256], pathname2[256];

extern char gpath[256];
extern char *name;
extern int n;

extern GD    *gp;
extern SUPER *sp;
extern INODE *ip;
extern DIR   *dp;

extern MINODE minode[NMINODE];
extern MINODE *root;
extern PROC   proc[NPROC], *running;

char *t1 = "xwrxwrxwr-------";
char *t2 = "----------------";




int link() {
	int nino, pino;
	MINODE *pmip;
	char *parent, *child;
	int oino = getino(pathname);
	MINODE *omip = iget(fd, oino);
	

	if (omip->inode.i_mode == DIR_MODE) {
		printf("can't link to a DIR\n");
		return -1;
	}

	nino = getino(pathname2);
	if (nino != 0) {
		printf("%s already exists\n", pathname2);
	}

	parent = dirname(pathname2);
	child = basename(pathname2);
	pino = getino(parent);
	pmip = iget(fd, pino);

	enter_name(pmip, oino, child);
	omip->inode.i_links_count++;
	omip->dirty = 1;
	iput(omip);
	iput(pmip);

}



