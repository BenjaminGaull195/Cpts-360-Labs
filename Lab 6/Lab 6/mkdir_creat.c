#include "type.h"
#include <string.h>
#include <time.h>


extern int fd, dev;
extern int ninodes, nblocks;
extern int bmap, imap, inode_start, iblock;

extern char line[256], cmd[32], pathname[256];

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




int enter_name(MINODE *pip, int myino, char *myname) {
	int i, remain, ideal_len, bno;
	char buf[BLKSIZE], *cp;

	for (i = 0; i < 12; ++i) {
		if (pip->inode.i_block[i] == 0) {
			break;
		}
	}
	get_block(fd, pip->inode.i_block[i], buf);

	dp = (DIR *)buf;
	cp = buf;

	printf("step to LAST entry in data block %d\n", pip->inode.i_block[i]);
	while (cp + dp->rec_len < buf + BLKSIZE) {

		printf("ino = %d, name = %s \n", dp->inode, dp->name);
		cp += dp->rec_len;
		dp = (DIR *)cp;
	}
	ideal_len = 4 * ((8 + dp->name_len + 3) / 4);
	remain = dp->rec_len - ideal_len;

	if (remain < dp->rec_len) {
		dp->rec_len = ideal_len;
		cp += dp->rec_len;
		dp = (DIR *)cp;

		dp->inode = myino;
		dp->rec_len = remain;
		strcpy(dp->name, myname);
		dp->name_len = strlen(myname);
		put_block(fd, pip->inode.i_block[i], buf);

	}
	else {
		bno = balloc(fd);
		ip = &pip->inode;
		ip->i_size += BLKSIZE;
		ip->i_block[i + 1] = bno;
		enter_name(pip, myino, myname);
	}
	put_block(pip, bno, buf);

}

int mymkdir(MINODE *pip, char *name) {
	MINODE *mip;
	int ino, bno, i;
	ino = ialloc(fd);
	bno = balloc(fd);
	printf("ino = %d, bno = %d\n", ino, bno);

	mip = iget(fd, ino);
	ip = &mip->inode;

	ip->i_mode = DIR_MODE;
	ip->i_uid = running->uid;
	ip->i_gid = running->gid;
	ip->i_size = BLKSIZE;
	ip->i_links_count = 2;
	ip->i_atime = ip->i_ctime = ip->i_mtime = time(0L);
	ip->i_blocks = 2;
	ip->i_block[0] = bno;
	for (i = 1; i < 14; ++i) {
		ip->i_block[i] = 0;
	}
	mip->dirty = 1;
	iput(mip);

	enter_name(mip, ino, ".");
	enter_name(mip, pip->ino, "..");
	enter_name(pip, ino, name);

	return 0;
}

int make_dir(char *pathname) {
	MINODE *start;
	int dev, pino;
	char *parent, *child, temp[256];
	MINODE *pmip;

	printf("%s\n", pathname);
	strcpy(temp, pathname);
	printf("pathname copied\n");

	if (temp[0] == '/') {
		//printf("path is relative to root\n");
		start = root;
		dev = root->dev;
		//printf("start = root, dev = %d\n", dev);
	}
	else {
		//printf("path is relative to cwd\n");
		start = running->cwd;
		//printf("Debug: assign start\n");
		dev = running->cwd->dev;
		//printf("Debug: assign dev\n");
		//printf("start = %d, dev = %d\n", start->ino, dev);
	}
	parent = dirname(temp);
	child = basename(temp);
	printf("parent = %s, child = %s\n", parent, child);

	pino = getino(parent);
	pmip = iget(dev, pino);
	printf("pino = %d, pmip = %d\n", pino, pmip->ino);
	if (pmip->inode.i_mode != DIR_MODE) {
		printf("Error: %s is not a valid pathname\n", pathname);
		return -1;
	}
	if (search(pmip, child) == 0) {
		printf("Error: %s already exists\n", pathname);
	}

	mymkdir(pmip, child);

	//update i_links_count
	pmip->inode.i_links_count += 1;
	//touch atime
	pmip->inode.i_atime = time(0L);

	pmip->dirty = 1;

	iput(pmip);
	return 0;
}


int my_creat(MINODE *pip, char *name) {
	MINODE *mip;
	int ino, bno, i;
	ino = ialloc(fd);
	//bno = balloc(fd);
	printf("ino = %d, bno = %d\n", ino, bno);

	mip = iget(fd, ino);
	ip = &mip->inode;

	ip->i_mode = FILE_MODE;
	ip->i_uid = running->uid;
	ip->i_gid = running->gid;
	ip->i_size = 0;
	ip->i_links_count = 1;
	ip->i_atime = ip->i_ctime = ip->i_mtime = time(0L);
	ip->i_blocks = 2;
	//ip->i_block[0] = bno;
	for (i = 0; i < 14; ++i) {
		ip->i_block[i] = 0;
	}
	mip->dirty = 1;
	iput(mip);

	//enter_name(mip, ino, ".");
	//enter_name(mip, pip->ino, "..");
	enter_name(pip, ino, name);

	return 0;
}

int creat_file(char *pathname) {
	MINODE *start;
	int dev, pino;
	char *parent, *child, temp[256];
	MINODE *pmip;
	strcpy(temp, pathname);

	if (pathname[0] == '/') {
		start = root;
		dev = root->dev;
	}
	else {
		start = running->cwd;
		dev = running->cwd->dev;
	}
	parent = dirname(temp);
	child = basename(temp);

	pino = getino(parent);
	pmip = iget(dev, pino);
	if (pmip->inode.i_mode != DIR_MODE) {
		printf("Error: %s is not a valid pathname\n", pathname);
		return -1;
	}
	if (search(pmip, child) == 0) {
		printf("Error: %s already exists\n", pathname);
	}

	my_creat(pmip, child);

	//update i_links_count
	//pmip->inode.i_links_count += 1;
	//touch atime
	pmip->inode.i_atime = time(0L);

	pmip->dirty = 1;

	iput(pmip);
	return 0;
}


