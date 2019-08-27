



typedef unsigned int u32;

char *ctable = "0123456789ABCDEF";
int  BASE = 10;

int rpu(u32 x)
{
	char c;
	if (x) {
		c = ctable[x % BASE];
		rpu(x / BASE);
		putchar(c);
	}
}

/*
char                      by %c
		 string                    by %s
		 unsigned integer          by %u
		 integer                   by %d
		 unsigned integer in OCT   by %o
		 unsigned integer in HEX   by %x
*/

int printu(u32 x)
{
	(x == 0) ? putchar('0') : rpu(x);
	//putchar(' ');
	return 0;
}

int prints(u32 x) {

	return 0;
}

/*
int printc(u32 x) {

	return 0;
}
*/

int printd(int x) {
	(x < 0) ? (-1 * rpu(-1 * x)) : rpu(x);
	return 0;
}
int printo(u32 x) {
	BASE = 8;

	rpu(x);

	BASE = 10;
	return 0;
}

int printx(u32 x) {
	BASE = 16;

	rpu(x);

	BASE = 10;
	return 0;
}


int myprintf(char *fmt, ... ) {
	char *cp = (char *)getebp();
	int *ip = (int *)getebp();		//increment by 1 for each % encountered
	int count = 0;
	char temp;

	while (fmt[count] != '\n') {
		temp = fmt[count];

		if (temp == '%') {
			++count;
			temp = fmt[count];
			++ip;

			switch (temp)
			{
			case 's':
				prints(*(ip));
				break;
			case 'c':
				putchar(temp);
				break;
			case 'd':
				printd(*(ip));
				break;
			case 'o':
				printo(*(ip));
				break;
			case 'x':
				printx(*(ip));
				break;
			case 'u':
				printu(*(ip));
				break;

			default:
				break;
			}

		}
		else {
			putchar(temp);
		}


		++count;
	}
	putchar('\r');

}
