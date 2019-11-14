#include <ext2fs/ext2_fs.h>

//constants
#define SUPERBLOCK 1
#define GDBLOCK 2
#define ROOT_INODE 2

#define DIR_MODE 0x41ED
#define FILE_MODE 0x81AE
#define LINK_MODE 0xA000	//this value is not provided, value taken from ls code
#define SUPER_MGAIC 0xEF53

#define SUPER_USER 0
#define FREE 0
#define BUSY 1
#define READY 2

#define NMINODE 64
#define NMTABLE 10
#define NPROC 2
#define NFD 10
#define NOFT 40

typedef struct ext2_group_desc GD;
typedef struct ext2_super_block SUPER;
typedef struct ext2_inode INODE;
typedef struct ext2_dir_entry_2 DIR;

#define BLKSIZE 1024
#define ISIZE              128

#define BITS_PER_BLOCK    (8*BLOCK_SIZE)
#define INODES_PER_BLOCK  (BLOCK_SIZE/sizeof(INODE))




typedef struct oft {
	int mode;
	int refCount;
	struct minode *minodePtr;
	int offset;
}OFT;

typedef struct proc {
	struct proc *next;
	int pid;
	int uid;
	int gid;
	int ppid;
	int status;
	struct minode *cwd;
	OFT *fd[NFD];
}PROC;

typedef struct minode {
	INODE inode;
	int dev, ino;
	int refCount;
	int dirty;
	int mounted;
	struct mount *mntPtr;

}MINODE;

typedef struct mount {
	int dev;
	int ninodes;
	int nblocks;
	int free_blocks;
	int free_inodes;
	int bmap;
	int imap;
	int iblock;
	MINODE *mntDirPtr;
	char devName[256];
	char mntName[64];
} MOUNT;
