#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>


#define MAX 10000
#define BLKSIZE 4096
typedef struct {
    char *name;
    char *value;
} ENTRY;

ENTRY entry[MAX];

struct stat mystat, *sp;
char *t1 = "xwrxwrxwr-------";
char *t2 = "----------------";



int _cat1(char *dirname);
int _cat(char *dir1, char *dir2);

int _cp(char *dirname1, char *dirname2);

int _ls(char *dirname);



main(int argc, char *argv[]) {
	int i, m, r;
	char cwd[128];
	//char dirname[128];
	char cmd[128], param1[128], param2[128];
 

	m = getinputs();    // get user inputs name=value into entry[ ]
	getcwd(cwd, 128);   // get CWD pathname
	
	printf("Content-type: text/html\n\n");
	printf("<p>pid=%d uid=%d cwd=%s\n", getpid(), getuid(), cwd);

	printf("<H1>Echo Your Inputs</H1>");
	printf("You submitted the following name/value pairs:<p>");
 
	for(i=0; i <= m; i++){
		printf("%s = %s<p>", entry[i].name, entry[i].value);
		if (!strcmp(entry[i].name, "command")) {
			strcpy(cmd, entry[i].value);
		}
		else if (!strcmp(entry[i].name, "filename1")) {
			strcpy(param1, entry[i].value);
		}
		else if (!strcmp(entry[i].name, "filename2")) {
			strcpy(param2, entry[i].value);
		}
	}
	printf("<p>");


  /*****************************************************************
   Write YOUR C code here to processs the command
         mkdir dirname
         rmdir dirname
         rm    filename
         cat   filename
         cp    file1 file2
         ls    [dirname] <== ls CWD if no dirname
  *****************************************************************/
  //temp functions to be executed
	if (!strcmp(cmd, "mkdir")) {
		r = mkdir(param1, 0755);
	}
	else if (!strcmp(cmd, "rmdir")) {
		r = rmdir(param1);
	}
	else if (!strcmp(cmd, "rm")) {
		r = unlink(param1);
	}
	else if (!strcmp(cmd, "cat")) {
		if (m == 2) {
			r = _cat(param1, param2);
		}
		else {
			r = _cat1(param1);
		}
		
	}
	else if (!strcmp(cmd, "cp")) {
		if (m < 2) {
			printf("too few filenames for cp<p>");			
		}
		else {
			r = _cp(param1, param2);
		}
	}
	else if (!strcmp(cmd, "ls")) {
		if (m == 1) {
			strcpy(param1, cwd);
		}
		r = _ls(param1);
	}
	else {
		printf("No command found<p>");
	}

  


 
  // create a FORM webpage for user to submit again 
	printf("</title>");
	printf("</head>");
	printf("<body bgcolor=\"#FF0000\" link=\"#330033\" leftmargin=8 topmargin=8");
	printf("<p>------------------ DO IT AGAIN ----------------\n");
  
  //printf("<FORM METHOD=\"POST\" ACTION=\"http://cs360.eecs.wsu.edu/~kcw/cgi-bin/mycgi.bin\">");

  //------ NOTE : CHANGE ACTION to YOUR login name ----------------------------
	printf("<FORM METHOD=\"POST\" ACTION=\"http://cs360.eecs.wsu.edu/~gaull/cgi-bin/mycgi.bin\">");
  
	printf("Enter command : <INPUT NAME=\"command\"> <P>");
	printf("Enter filename1: <INPUT NAME=\"filename1\"> <P>");
	printf("Enter filename2: <INPUT NAME=\"filename2\"> <P>");
	printf("Submit command: <INPUT TYPE=\"submit\" VALUE=\"Click to Submit\"><P>");
	printf("</form>");
	printf("------------------------------------------------<p>");

	printf("</body>");
	printf("</html>");
}

int _cat1(char *dirname) {
	int fd, i, m, n;
	char buf[BLKSIZE], dummy;
	fd = open(dirname, O_RDONLY);
	if (fd < 0) {
		printf("failed to open %s<p>", dirname);
		return -1;
	}
	while (n = read(fd, buf, BLKSIZE)) {
		m = write(1, buf, n);
		printf("<p>");
	}
	
}

int _cat(char *dir1, char *dir2) {
	_cat1(dir1);
	_cat1(dir2);
	return 0;
}

int _cp(char *dirname1, char *dirname2) {
	int fd, gd, n, total = 0;
	char buf[BLKSIZE];
	if ((fd = open(dirname1, O_RDONLY)) < 0) {
		printf("failed to open %s<p>", dirname1);
		return -1;
	}
	if ((gd = open(dirname2, O_RDONLY)) < 0) {
		printf("failed to open %s<p>", dirname2);
		return -1;
	}
	while (n = read(fd, buf, BLKSIZE)) {
		write(gd, buf, n);
		total += n;
	}
	printf("total bytes copied = %d<p>", total);
	close(fd);
	close(gd);
	return 0;
}


int ls_file(char *fname) {
	struct stat fstat, *sp;
	int r, i;
	char ftime[64], linkname[128];
	sp = &fstat;
	if ((r = lstat(fname, &fstat)) < 0) {
		printf("can't stat %s<p>", fname);
		//exit(1);
		return -1;
	}
	if ((sp->st_mode & 0xF000) == 0x8000) {
		printf("%c", '-');
	}
	if ((sp->st_mode & 0xF000) == 0x4000) {
		printf("%c", 'd');
	}
	if ((sp->st_mode & 0xF000) == 0xA000) {
		printf("%c", 'l');
	}
	for (i = 8; i >= 0; --i) {
		if (sp->st_mode & (1 << i)) {
			printf("%c", t1[i]);
		}
		else {
			printf("%c", t2[i]);
		}
	}
	printf("%4d", sp->st_nlink);
	printf("%4d", sp->st_gid);
	printf("%4d", sp->st_uid);
	printf("%8d", sp->st_size);
	strcpy(ftime, ctime(&sp->st_ctime));
	ftime[strlen(ftime) - 1] = 0;
	printf("%s", ftime);
	printf("%s", basename(fname));
	if ((sp->st_mode & 0xF000) == 0xA000) {
		readlink(fname, linkname, 128);
		printf(" - > %s", linkname);
	}
	printf("<p>");
}

int ls_dir(char *dname) {
	struct dirent *ep;
	DIR *dp = opendir(dname);

	if (dp == 0) {
		printf("opendir %s failed\n", dname);
		return -1;
	}
	while (ep = readdir(dp)) {
		if (strcmp(ep->d_name, ".") && strcmp(ep->d_name, "..")) {
			ls_file(ep->d_name);
		}
	}

}

int _ls(char *dirname) {
	struct stat mystat, *sp = &mystat;
	int r;
	if (r = lstat(dirname, sp) < 0) {
		printf("%s does not exist<p>", dirname);
		return -1;
	}
	if (S_ISDIR(sp->st_mode)) {
		ls_dir(dirname);
	}
	else {
		ls_file(dirname);
	}
}

