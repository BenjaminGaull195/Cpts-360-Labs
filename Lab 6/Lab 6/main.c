#include <ext2fs/ext2_fs.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>
#include <libgen.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>

#include "type.h"

//global
int fd, dev;
int ninodes, nblocks;
int bmap, imap, inode_start, iblock;

char line[256], cmd[32], pathname[256], pathname2[256];

char gpath[256];
char *name;
int n;

GD    *gp;
SUPER *sp;
INODE *ip;
DIR   *dp;

MINODE minode[NMINODE];
MINODE *root;
PROC   proc[NPROC], *running;
OFT fileTable[NOFT];

char *t1 = "xwrxwrxwr-------";
char *t2 = "----------------";

//typedefs
typedef void(*cmd_ptr)();


//function prototypes
int init();
int mount_root();
void ls(char *pathname);
void chdir(char *pathname);
void pwd(MINODE *wd);
extern int make_dir(char *pathname);
extern int creat_file(char *pathname);
extern int rm_dir();
extern int link();
extern int unlink();
extern int symlink();
extern int showlink();

void quit();

//int find_cmd(char *command);
void printCommands();


char *commands[32] = { "ls", "cd", "pwd", "quit", 0 };
cmd_ptr cmd_ptrs[32] = { &ls, &chdir, &pwd, &quit };


int main(int argc, char *argv[]) {
	init();
	mount_root();
	int i;

	/*printf("INIT DEBUG:\n");
	printf("Proc%d: uid=%d, gid=%d\n", running->pid, running->uid, running->gid);
	printf("Proc%d: cwd=%d\n", running->pid, running->cwd);
	printf("END INIT DEBUG\n");*/

	while (1) {
		//print available commands
		strcpy(pathname, "");
		strcpy(pathname2, "");
		printCommands();
		fgets(line, 256, stdin);
		i = sscanf(line, "%s %s %s", cmd, pathname, pathname2);

		printf("cmd = %s, pathname = %s\n", cmd, pathname);

		//cmd_ptrs[find_cmd(cmd)](pathname);
		if (strcmp(cmd, "ls") == 0) {
			ls(pathname);
		}
		else if (strcmp(cmd, "cd") == 0) {
			chdir(pathname);
		}
		else if (strcmp(cmd, "pwd") == 0) {
			pwd(running->cwd);
		}
		else if (strcmp(cmd, "quit") == 0) {
			quit();
		}
		else if (strcmp(cmd, "mkdir") == 0) {
			make_dir(pathname);
		}
		else if (strcmp(cmd, "creat") == 0) {
			creat_file(pathname);
		}
		else if (strcmp(cmd, "rmdir") == 0) {
			//rmdir();
			rm_dir();
		}
		else if (strcmp(cmd, "link") == 0) {
			link();
		}
		else if (strcmp(cmd, "unlink") == 0) {
			unlink();
		}
		else if (strcmp(cmd, "symlink") == 0) {
			symlink();
		}
		else if (strcmp(cmd, "showlink") == 0) {
			showlink();
		}
		else {
			printf("%s is not an available command\n");
		}


	}
	   	  
	return 0;
}

void printCommands() {
	printf("================ commands ================\n");
	printf("|  ls | cd | pwd | mkdir | creat | quit   |\n");
	printf("| rmdir | link| unlink| symlink| showlink |\n");
	printf("==========================================\n\n");

	printf("Enter a command: ");
}



int init() {
	int i, j;
	for (i = 0; i < NMINODE; ++i) {
		minode[i].refCount = 0;
	}
	for (i = 0; i < NPROC; ++i) {
		proc[i].status = READY;
		proc[i].pid = i;
		proc[i].uid = i;
		for (j = 0; j < NFD; ++j) {
			proc[i].fd[j] = 0;
		}
		proc[i].next = &proc[i + 1];
	}
	proc[NPROC - 1].next = &proc[0];
	running = &proc[0];
	printf("debug: &proc=%d\n", &proc);
	printf("debug: &proc0=%d\n", &proc[0]);
	printf("debug: &proc1=%d\n", &proc[1]);
	printf("debug: running=%d\n", running);
	//root = 0;

}


int mount_root() {
	char buf[BLKSIZE];
	MINODE *mip;
	//SUPER *sp;
	//GD * gp;
	fd = dev = open("disk", O_RDWR);
	if (fd < 0) {
		printf("Failed to open disk\n");
		exit(1);
	}
	
	get_block(fd, SUPERBLOCK, buf);
	sp = (SUPER *)buf;

	if (sp->s_magic != SUPER_MGAIC) {
		printf("diskImage is not ext2\n");
		exit(1);
	}

	ninodes = sp->s_inodes_count;
	nblocks = sp->s_blocks_count;

	printf("ninodes = %d nblocks = %d\n", ninodes, nblocks);

	get_block(fd, GDBLOCK, buf);
	gp = (GD *)buf;

	bmap = gp->bg_block_bitmap;
	imap = gp->bg_inode_bitmap;
	inode_start = iblock = gp->bg_inode_table;

	printf("bmp=%d imap=%d iblock = %d\n", bmap, imap, iblock);

	mip = iget(fd, 2);
	//printf("debug: root=%d\n", mip);
	root = mip;

	proc[0].cwd = mip;
	proc[1].cwd = mip;

	//running = &proc[0];
}

//helper functions

int ls_file(int ino, char *fname) {
	MINODE *mip = iget(fd, ino);
	ip = &mip->inode;
	char ftime[64], linkname[128];
	int i;

	//print file mode
	if ((ip->i_mode & 0xF000) == 0x8000) {
		printf("%c", '-');
	}
	if ((ip->i_mode & 0xF000) == 0x4000) {
		printf("%c", 'd');
	}
	if ((ip->i_mode & 0xF000) == 0xA000) {
		printf("%c", 'l');
	}
	for (i = 8; i >= 0; --i) {	//print file permissions
		if (ip->i_mode & (1 << i)) {
			printf("%c", t1[i]);
		}
		else {
			printf("%c", t2[i]);
		}
	}	//print file info
	printf("%4d", ip->i_links_count);
	printf("%4d", ip->i_gid);
	printf("%4d", ip->i_uid);
	printf("%8d", ip->i_size);
	strcpy(ftime, ctime(&ip->i_ctime));
	ftime[strlen(ftime) - 1] = 0;
	printf("%s", ftime);
	printf("%s", basename(fname));
	if ((ip->i_mode & 0xF000) == 0xA000) { //print symbolic link
		readlink(fname, linkname, 128);
		printf(" - > %s", linkname);
	}
	printf("\n");
}

int ls_dir(char *dname) {
	int ino = getino(pathname);
	MINODE *mip = iget(fd, ino);
	char buf[BLKSIZE], *cp;
	//DIR *dp;
	int i = 0;

	get_block(fd, mip->inode.i_block[0], buf);
	dp = (DIR *)buf;

	while (cp < buf + BLKSIZE) {
		mip = iget(fd, dp->inode);
		ls_file(mip->ino, dp->name);
		dp = (DIR *)buf;
		cp = buf;
	}
}


//command functions
//int find_cmd(char *command) {
//	int i = 0;
//	for (i = 0; i < 32; ++i) {
//		if (strcmp(command, commands[i]) == 0) {
//			return i;
//		}
//	}
//}


void ls(char *pathname) {
	char *name, buf;
	//DIR *dp;
	if (strcmp(pathname, "") == 0) {	//ls for cwd
		get_block(fd, running->cwd->inode.i_block[0], buf);
		dp = (DIR *)buf;
		name = dp->name;
		ls_dir(name);
	}
	else {

	}

}

void chdir(char *pathname) {
	MINODE *mip;
	if (strcmp(pathname, "") == 0) {
		running->cwd = root;
	}
	else {
		int ino = getino(pathname);
		if (ino == 0) {
			printf("Error: failed to get ino\n");
			return;
		}
		mip = iget(fd, ino);
		if (mip->inode.i_mode != FILE_MODE) {
			printf("pathname is not a DIR\n");
			return;
		}
		iput(running->cwd);
		running->cwd = mip;
	}

}

void rpwd(MINODE *wd) {
	int ino, pino;
	INODE *pip;
	char buf[BLKSIZE], myname[128];


	if (wd == root) {
		return;
	}
	pino = findino(wd, &ino);
	pip = iget(fd, pino);
	findmyname(pip, ino, myname);

	rpwd(pip);
	printf("/%s", myname);

}

void pwd(MINODE *wd) {
	if (wd == root) {
		printf("/\n");
	}
	else {
		rpwd(wd);
	}
}

void quit() {
	//write minodes to disk
	int i;
	MINODE *mip;
	for (i - 0; i < NMINODE; ++i) {
		mip = &minode[i];
		if (mip->refCount && mip->dirty) {
			mip->refCount = 1;
			iput(mip);
		}
	}

	//exit
	exit(0);
}





