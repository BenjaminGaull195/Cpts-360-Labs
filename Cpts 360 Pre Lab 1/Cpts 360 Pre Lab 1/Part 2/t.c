#include <stdio.h>
#include <stdlib.h>

int *FP;

int main(int argc, char *argv[], char *env[])
{
	int a, b, c, i;
	printf("enter main\n");

	printf("&argc=%x argv=%x env=%x\n", &argc, argv, env);
	printf("&a=%8x &b=%8x &c=%8x\n", &a, &b, &c);

	//(1).Write C code to print values of argc and argv[] entries
	printf("argc = %d\n", argc);
	for (i = 0; i < argc; ++i) {
		printf("argv[%d] = %s\n", i, argv[i]);
	}

		a = 1; b = 2; c = 3;
	A(a, b);
	printf("exit main\n");
}

int A(int x, int y)
{
	int d, e, f;
	printf("enter A\n");
	// write C code to PRINT ADDRESS OF d, e, f
	printf("&d = %x, &e = %x, &f = %x\n", &d, &e, &f);

	d = 4; e = 5; f = 6;
	B(d, e);
	printf("exit A\n");
}

int B(int x, int y)
{
	int g, h, i;
	printf("enter B\n");
	// write C code to PRINT ADDRESS OF g,h,i
	printf("&g = %x, &h = %x, &i = %x\n", &g, &h, &i);

	g = 7; h = 8; i = 9;
	C(g, h);
	printf("exit B\n");
}

int C(int x, int y)
{
	int u, v, w, i, *p;

	printf("enter C\n");
	// write C cdoe to PRINT ADDRESS OF u,v,w,i,p;
	u = 10; v = 11; w = 12; i = 13;
	printf("&u = %x, &v = %x, &w = %x, &i = %x, &p = %x\n\n", &u, &v, &w, &i, p);


	FP = (int *)getebp();  // FP = stack frame pointer of the C() function


	//(2).Write C code to print the stack frame link list.
	printf("Initial FP = %x\n", FP);

	//C Function Frame
	
	printf("x = %x\n", *(FP + 12));
	printf("y = %x\n", *(FP + 8));
	printf("PC = %x\n", *(FP + 4));
	printf("FP = %x\n", *(FP));
	printf("u = %x\n", *(FP - 4));
	printf("v = %x\n", *(FP - 8));
	printf("w = %x\n", *(FP - 12));
	printf("i = %x\n", *(FP - 16));
	printf("p = %x\n", *(FP - 20));

	//B Function Frame
	FP = *(FP);

	printf("x = %x\n", *(FP + 12));
	printf("y = %x\n", *(FP + 8));
	printf("PC = %x\n", *(FP + 4));
	printf("FP = %x\n", *(FP));
	printf("g = %x\n", *(FP - 4));
	printf("h = %x\n", *(FP - 8));
	printf("i = %x\n", *(FP - 12));

	//A Function Frame
	FP = *(FP);

	printf("x = %x\n", *(FP + 12));
	printf("y = %x\n", *(FP + 8));
	printf("PC = %x\n", *(FP + 4));
	printf("FP = %x\n", *(FP));
	printf("d = %x\n", *(FP - 4));
	printf("e = %x\n", *(FP - 8));
	printf("f = %x\n", *(FP - 12));

	//Main
	FP = *(FP);

	printf("env = %x\n", *(FP + 16));
	printf("argv = %x\n", *(FP + 12));
	printf("argc = %x\n", *(FP + 8));
	printf("PC = %x\n", *(FP + 4));
	printf("FP = %x\n", *(FP));
	printf("a = %x\n", *(FP - 4));
	printf("b = %x\n", *(FP - 8));
	printf("c = %x\n", *(FP - 16));

	//crt0.o
	//FP = *(FP);
	
	//printf("argc = %d\n", (FP + 8));
	//printf("PC = %d\n", *(FP + 4));
	//printf("FP = %x\n", *FP);
	//printf("a = %d\n", (FP - 4));

	p = (int *)&p;

	printf("exit C\n");

	/*
	(3).Print the stack contents from p to the frame of main()
	YOU MAY JUST PRINT 128 entries of the stack contents.

	(4).On a hard copy of the print out, identify the stack contents
	*/

}
