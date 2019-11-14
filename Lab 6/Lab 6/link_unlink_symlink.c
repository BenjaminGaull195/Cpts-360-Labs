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

//char *t1 = "xwrxwrxwr-------";
//char *t2 = "----------------";




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




int unlink() {
	int pino;
	MINODE *pmip;

	int ino = getino(pathname);
	MINODE *mip = iget(fd, ino);

	char *parent;
	char *child;


	//verify pathname is a file
	if (mip->inode.i_mode == DIR_MODE) {
		printf("pathname is not a file\n");
		return -1;
	}

	//
	mip->inode.i_links_count--;

	//remove if links = 0
	if (mip->inode.i_links_count == 0) {
		truncate(mip);
		idalloc(fd, mip->ino);
	}

	//remove from parent dir
	parent = dirname(pathname);
	child = basename(pathname);

	pino = getino(parent);
	pmip = iget(fd, pino);

	rm_child(pmip, child);
	pmip->dirty = 1;

	iput(mip);
	iput(pmip);
}


int symlink() {
	int nChar;
	int nino;
	MINODE *nmip;
	int oino = getino(pathname);
	MINODE *omip = iget(fd, oino);

	//verify pathname is file
	if (omip->inode.i_mode == DIR_MODE) {
		printf("pathname %s is not a file\n", pathname);
		return -1;
	}

	//create file for newName path
	creat_file(pathname2);

	//change file to LINK_MODE
	nino = getino(pathname2);
	nmip = iget(pathname2);

	nmip->inode.i_mode = LINK_MODE;

	//write oldname to new file
	strcpy((char *)nmip->inode.i_block, pathname2);

	//set file size to num chars in oldname
	nmip->inode.i_size = nChar;

	nmip->dirty = 1;
	//write inode back to disk
	iput(omip);
	iput(nmip);

}


int showlink() {
	char link[128];
	int ino = getino(pathname);
	MINODE *mip = iget(fd, ino);

	//verify link file
	if (mip->inode.i_mode != LINK_MODE) {
		printf("file is not a link file\n");
		return -1;
	}

	//get link info
	strcpy(link, (char *)mip->inode.i_block);

	//print link
	printf("%s\n", link);

	//
	iput(mip);
}


