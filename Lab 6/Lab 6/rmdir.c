//Worked with 

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

char *t1 = "xwrxwrxwr-------";
char *t2 = "----------------";


int rm_child(MINODE *pmip, char *myname) {
	DIR *prev;
	int i, rec;
	char buf[BLKSIZE], *cp, *cp2;

	for (i = 0; i < 12; ++i) {
		get_block(pmip->dev, pmip->inode.i_block[i], buf);
		prev = dp = (DIR *)buf;
		cp = buf;

		while (cp < buf + BLKSIZE) {
			if (!strcmp(dp->name, myname)) {
				if (cp + dp->rec_len >= buf + BLKSIZE) {	//last entry
					prev->rec_len += dp->rec_len;
					put_block(pmip->dev, pmip->inode.i_block[i], buf);
					return 0;
				}
				else if (dp->rec_len == BLKSIZE) {	//only entry entry
					bdalloc(fd, pmip->inode.i_block[i]);
					
					for (i = i + 1; i < 12; ++i) {
						if (pmip->inode.i_block[i] == 0) {
							pmip->inode.i_block[i - 1] = 0;
							break;
						}
						pmip->inode.i_block[i - 1] = pmip->inode.i_block[i];
					}


					return 0;
				}
				else { //middle entry
					cp2 = cp + dp->rec_len;
					rec = dp->rec_len;
					int bytesToMove = (buf + BLKSIZE) - (cp + dp->rec_len);

					memmove(cp, cp2, bytesToMove);

					dp = (DIR *)cp;

					while ((cp + dp->rec_len) < (buf + BLKSIZE - rec)) {
						cp += dp->rec_len;
						dp = (DIR *)cp;
					}
					dp->rec_len += rec;

					put_block(fd, pmip->inode.i_block[i], buf);
					return 0;

					/*dp = (DIR *)cp2;
					while (cp2 < buf + BLKSIZE) {
						memcpy(cp, cp2, dp->rec_len);
						dp = (DIR *)cp;
						cp2 += dp->rec_len;
						cp += dp->rec_len;
						dp = (DIR *)cp;
					}
					dp->rec_len += rec;
					put_block(pmip->dev, pmip->inode.i_block[i], buf);
					return 0;*/
				}
			}
			
			cp += dp->rec_len;
			prev = dp;
			dp = (DIR *)cp;
		}
	}


}


int rm_dir() {
	int i, uid;
	int pino;
	MINODE *pmip;
	char myname[256], buf [BLKSIZE], *cp;
	//get in memory INODE
	int ino = getino(pathname);
	MINODE *mip = iget(fd, ino);


	//check user id
	uid = mip->inode.i_uid;
	if (running->uid != 0) {
		if (uid == running->uid) {
			printf("User does not have permission to remove %s\n", pathname);
			iput(mip);
			return -1;
		}
	}
	//verify INODE is a DIR
	if (mip->inode.i_mode != FILE_MODE) {
		printf("%s is not a dir\n", pathname);
		iput(mip);
		return -1;
	}

	//check if INODE is BUSY
	if (mip->refCount != 1) {
		printf("%s is busy\n", pathname);
		iput(mip);
		return - 1;
	}	

	//check if INODE is empty
	for (i = 0; i < 12; ++i) {
		if (mip->inode.i_block[i] == 0) {
			iput(mip);
			break;
		}
	}
	if (i > 1) {
		printf("%s is not empty\n", pathname);
		iput(mip);
		return -1;
	}
	else {
		get_block(fd, mip->inode.i_block[0], buf);
		dp = (DIR *)buf;
		cp = buf;
		i = 0;

		while (cp < buf + BLKSIZE) {
			cp += dp->rec_len;
			dp = (DIR *)cp;
			++i;
			if (i > 2) {
				printf("%s is not empty\n", pathname);
				iput(mip);
				return -1;
			}
		}
	}

	//check if pathname is root
	if (root->ino == ino) {
		printf("can't remove /");
		iput(mip);
		return -1;
	}

	if (!strcmp(pathname, ".") || !strcmp(pathname, "..") {
		printf("can't remove %s\n", pathname);
		iput(mip);
		return -1;
	}

	//get parents ino and INODE from .. entry
	pino = search(mip, "..");
	pmip = iget(mip->dev, pino);

	//get name from parent DIR's data block
	findmyname(pmip, ino, myname);

	//remove name from parent directory
	rm_child(pmip, myname);

	//decrement parent i_links_count
	pmip->inode.i_links_count -= 1;

	//mark parent dirty
	pmip->dirty = 1;

	pmip->inode.i_atime = pmip->inode.i_mtime = time(0L);

	//write parent to disc
	iput(pmip);

	//dealloc data blocks and INODE
	bdalloc(mip->dev, mip->inode.i_block[0]);
	idalloc(mip->dev, mip->ino);
	iput(mip);

	//

}