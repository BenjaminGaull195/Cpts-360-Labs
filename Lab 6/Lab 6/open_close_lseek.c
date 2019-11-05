#include "type.h"
#include <string.h>
#include <time.h>

/**** globals defined in main.c file ****/
extern MINODE minode[NMINODE];
extern MINODE *root;
extern PROC   proc[NPROC], *running;

extern char   gpath[256];
extern char   *name[64];
extern int    n;

extern int    fd, dev;
extern int    nblocks, ninodes, bmap, imap, inode_start;
extern char   line[256], cmd[32], pathname[256], pathname2[256];

extern GD    *gp;
extern SUPER *sp;
extern INODE *ip;
extern DIR   *dp;



int open_file(char *pathname, int mode) {
	OFT *oftp;
	MINODE *mip;
	int ino;
	//get ino
	ino = getino(pathname);
	int i;

	//get minode
	mip = iget(fd, ino);

	//check regular file
	if (mip->inode.i_mode != FILE_MODE) {
		printf("%s is not a file\n");
		return -1;
	}
	else if () {	//check permissions
		printf("user does not have permission\n");
		return -1;
	}

	//allocate free oft


	oftp->minodePtr = mip;
	oftp->mode = mode;
	oftp->refCount = 1;

	//set offset:
	switch(mode) {
		case 0:
			oftp->offset = 0;
			break;
		case 1:
			oftp->offset = 0;
			break;
		case 2:
			oftp->offset - 0;
			break;
		case 3:
			oftp->offset = mip->inode.i_size;
			break;
		default:
			printf("invalid mode\n");
			return -1;
	}

	//find open fd in running
	for (i = 0; i < NFD; ++i) {
		if (running->fd[i] == 0) {
			running->fd[i] = oftp;
		}
	}

	//update inode time
	switch (mode)
	{
	case 0:
		mip->inode.i_atime = time(0L);
		break;
	case 1:
	case 2:
	case 3:
		mip->inode.i_atime = mip->inode.i_mtime = time(0L);
		mip->dirty;
		break;
	}

	return i;
}


int truncate(MINODE *mip) {
	int i, j;
	char buf1[BLKSIZE], buf2[BLKSIZE];

	//for direct blocks
	for (i = 0; i < 12; ++i) {
		bdalloc(fd, mip->inode.i_block[i]);
		mip->inode.i_block[i] = 0;
	}

	//for indirect blocks
	get_block(fd, mip->inode.i_block[12], buf1);
	for (i = 0; i < 256; ++i) {


		bdalloc(fd, );

	}

	//for double indirect blocks
	get_block(fd, mip->inode.i_block[13], buf1);
	for (i = 0; i < 256; ++i) {

		get_block(fd, , buf2);
		for (j = 0; j < 256; ++j) {
			bdalloc(fd, );
		}
		bdalloc(fd, );

	}

}

int close_file(int fd) {
	OFT * oftp;
	MINODE *mip;

	if (fd >= NFD) {
		printf("fd out of range\n");
		return -1;
	}

	if (running->fd[fd] == 0) {
		printf("error: no OFT\n");
		return -1;
	}

	oftp = running->fd[fd];
	running->fd[fd] = 0;
	oftp->refCount--;
	if (oftp->refCount > 0) {
		return 0;
	}
	mip = oftp->minodePtr;
	iput(mip);

	return 0;

}

int lseek(int fd, int position) {
	OFT *oftp;
	int pos;

	//find oft
	oftp = running->fd[fd];

	//change oft offset position, do not overrun eof
	pos = oftp->offset;

	if (position >= oftp->minodePtr->inode.i_size) {
		printf("error: position exceeds file size\n");
		return -1;
	}
	oftp->offset = position;

	//return origional position
	return pos;
}

int pfd() {
	int i;

	for (i = 0; i < NFD; ++i) {
		printf("%d ", i);
		switch (running->fd[i]->mode) {
		case 0:
			printf("READ ");
			break;
		case 1:
			printf("WRITE ");
			break;
		case 2:
			printf("READ/WRITE ");
			break;
		case 3:
			printf("APPEND ");
			break;
		}
		printf("%d ", running->fd[i]->offset);
		printf("[%d, %d]\n", running->fd[i]->minodePtr->dev, running->fd[i]->minodePtr->ino);
	}

}

int dup(int fd) {
	int i;
	if (running->fd[fd] == 0) {
		printf("%d is not an open fd", fd);
		return -1;
	}
	for (i = 0; i < NFD; ++i) {
		if (running->fd[i] == 0) {
			running->fd[i] = fd;
			return i;
		}
	}
	return -1;
}

int dup2(int fd, int gd) {
	close_file(gd);
	dup[fd];
}



