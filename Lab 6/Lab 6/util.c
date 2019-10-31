/*********** util.c file ****************/
#include "type.h"
#include <string.h>

/**** globals defined in main.c file ****/
extern MINODE minode[NMINODE];
extern MINODE *root;
extern PROC   proc[NPROC], *running;

extern char   gpath[256];
extern char   *name[64];
extern int    n;

extern int    fd, dev;
extern int    nblocks, ninodes, bmap, imap, inode_start;
extern char   line[256], cmd[32], pathname[256];

extern GD    *gp;
extern SUPER *sp;
extern INODE *ip;
extern DIR   *dp;

int get_block(int dev, int blk, char *buf)
{
	lseek(dev, (long)blk*BLKSIZE, 0);
	read(dev, buf, BLKSIZE);
}

int put_block(int dev, int blk, char *buf)
{
	lseek(dev, (long)blk*BLKSIZE, 0);
	write(dev, buf, BLKSIZE);
}

int tokenize(char *pathname)
{
	char *s;
	strcpy(gpath, pathname);
	// YOUR tokenize() code: strtok(gpath)
	n = 0;
	s = strtok(gpath, "/");
	while (s) {
		name[n++] = s;
		s = strtok(NULL, "/");
	}
}

// return minode pointer to loaded INODE
MINODE *iget(int dev, int ino)
{
	int i;
	MINODE *mip;
	char buf[BLKSIZE];
	int blk, disp;
	INODE *ip;

	for (i = 0; i < NMINODE; i++) {
		mip = &minode[i];

		if (mip->refCount && mip->dev == dev && mip->ino == ino) {
			mip->refCount++;
			printf("found [%d %d] as minode[%d] in core\n", dev, ino, i);
			return mip;
		}
	}

	for (i = 0; i < NMINODE; i++) {
		mip = &minode[i];
		if (mip->refCount == 0) {
			//printf("allocating NEW minode[%d] for [%d %d]\n", i, dev, ino);
			mip->refCount = 1;
			mip->dev = dev;
			mip->ino = ino;

			// get INODE of ino to buf    
			blk = (ino - 1) / 8 + inode_start;
			disp = (ino - 1) % 8;

			//printf("iget: ino=%d blk=%d disp=%d\n", ino, blk, disp);

			get_block(dev, blk, buf);
			ip = (INODE *)buf + disp;
			// copy INODE to mp->INODE
			mip->INODE = *ip;

			return mip;
		}
	}
	printf("PANIC: no more free minodes\n");
	return 0;
}

int iput(MINODE *mip)
{
	int i, block, offset;
	char buf[BLKSIZE];
	INODE *ip;

	if (mip == 0)
		return;

	mip->refCount--;

	if (mip->refCount > 0) return;
	if (!mip->dirty)       return;

	/* write back */
	//printf("iput: dev=%d ino=%d\n", mip->dev, mip->ino); 

	block = ((mip->ino - 1) / 8) + inode_start;
	offset = (mip->ino - 1) % 8;

	/* first get the block containing this inode */
	get_block(mip->dev, block, buf);

	ip = (INODE *)buf + offset;
	*ip = mip->INODE;

	put_block(mip->dev, block, buf);

}

int search(MINODE *mip, char *name)
{
	// YOUR search function: return ino if found name; else return 0;
	int i;
	char *cp, temp[256], sbuf[BLKSIZE];
	DIR *dp;

	for (i = 0; i < 12; ++i) {
		if (mip->inode.i_block[i] == 0) {
			return 0;
		}

		get_block(mip->dev, mip->inode.i_block[i], sbuf);
		dp = (DIR *)sbuf;
		cp = sbuf;

		while (cp < sbuf + BLKSIZE) {
			strncpy(temp, dp->name, dp->name_len);
			temp[dp->name_len] = 0;
			print("%8d%8d%8u %s", dp->inode, dp->rec_len, dp->name_len, temp);

			if (strcmp(name, temp) == 0) {
				printf("found %s : inum = %d\n", name, dp->inode);
				cp += dp->rec_len;
				dp = (DIR *)cp;
			}
		}
	}
}

int getino(char *pathname)
{
	int i, ino, blk, disp;
	INODE *ip;
	MINODE *mip;

	printf("getino: pathname=%s\n", pathname);
	if (strcmp(pathname, "/") == 0)
		return 2;

	if (pathname[0] == '/')
		mip = iget(dev, 2);
	else
		mip = iget(running->cwd->dev, running->cwd->ino);

	tokenize(pathname);

	for (i = 0; i < n; i++) {
		printf("===========================================\n");
		ino = search(mip, name[i]);

		if (ino == 0) {
			iput(mip);
			printf("name %s does not exist\n", name[i]);
			return 0;
		}
		iput(mip);
		mip = iget(dev, ino);
	}
	iput(mip);
	return ino;
}

int findmyname(MINODE *parent, uint32_t myino, char myname[])
{
	char buf[BLKSIZE];
	// find mynio in parent data block; copy name string to myname[ ];
	get_block(fd, parent->inode.i_block[0], buf);

}


int findino(MINODE *mip, uint32_t *myino) // return ino of parent and myino of .
{
	char buf[BLKSIZE], *cp;
	DIR *dp;

	get_block(mip->dev, mip->INODE.i_block[0], buf);
	cp = buf;
	dp = (DIR *)buf;
	*myino = dp->inode;
	cp += dp->rec_len;
	dp = (DIR *)cp;
	return dp->inode;
}



int tst_bit(char *buf, int bit)
{
	int i, j;
	i = bit / 8; j = bit % 8;
	if (buf[i] & (1 << j))
		return 1;
	return 0;
}

int set_bit(char *buf, int bit)
{
	int i, j;
	i = bit / 8; j = bit % 8;
	buf[i] |= (1 << j);
}

int clr_bit(char *buf, int bit)
{
	int i, j;
	i = bit / 8; j = bit % 8;
	buf[i] &= ~(1 << j);
}

int decFreeInodes(int dev)
{
	char buf[BLKSIZE];
	// dec free inodes count by 1 in SUPER and GD
	get_block(dev, 1, buf);
	sp = (SUPER *)buf;
	sp->s_free_inodes_count--;
	put_block(dev, 1, buf);

	get_block(dev, 2, buf);
	gp = (GD *)buf;
	gp->bg_free_inodes_count--;
	put_block(dev, 2, buf);
}

int ialloc(int dev)  // allocate an inode number
{
	int  i;
	char buf[BLKSIZE];

	// read inode_bitmap block
	get_block(dev, imap, buf);

	for (i = 0; i < ninodes; i++) {
		if (tst_bit(buf, i) == 0) {
			set_bit(buf, i);
			put_block(dev, imap, buf);
			decFreeInodes(fd);
			return i + 1;
		}
	}
	return 0;
}

int balloc(int dev)  // allocate an inode number
{
	int  i;
	char buf[BLKSIZE];

	// read inode_bitmap block
	get_block(dev, bmap, buf);

	for (i = 0; i < nblocks; i++) {
		if (tst_bit(buf, i) == 0) {
			set_bit(buf, i);
			put_block(dev, bmap, buf);
			decFreeInodes(fd);
			return i + 1;
		}
	}
	return 0;
}







