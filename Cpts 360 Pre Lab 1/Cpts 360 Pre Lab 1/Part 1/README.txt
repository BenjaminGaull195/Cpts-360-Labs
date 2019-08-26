PART 1:

1. A binary executable file, a.out, consists of

           |header| Code | Data |<-BSS->|

where BSS is for uninitialized globals and uninitialized static locals.

The Unix command   size a.out  shows the size of TEXT, DATA, BSS of a.out.

Use the following C program, t1.c, to generate t2.c, t3.c,.. t6.c as specified
below. 
        //********** t1.c file ************
          #include <stdio.h>
          int g;                              
          main()                              
          {                                   
             int a,b,c; 
             a = 1; b = 2; 
             c = a + b;
             printf("c=%d\n", c);
          }                                

t2.c: Change the global variable  g  to  int g=3;
t3.c  Change the global variable  g  to  int g[10000];
t4.c  Change the global variable  g  to  int g[10000] = {4};
t5.c  Change the local variables of main() to
                 int a,b,c, d[10000];
t6.c. Change the local variables of main() to
          static int a,b,c, d[10000];

(A). For each case, use cc -m32 t.c to generate a.out. 
     Then use  ls -l a.out to get a.out size.
          run  size a.out  to get its section sizes.

     Record the observed sizes in a table:

          Case  | a.out |  TEXT  |  DATA  |  BSS   |
        --------------------------------------------
          (1)   |       |        |        |        |
        --------------------------------------------  
          (2)
        --------------------------------------------
          (3)
        --------------------------------------------
          (4)
        --------------------------------------------
          (5)
        --------------------------------------------        
          (6)
        --------------------------------------------

                  ANSWER THESE QUESTIONS:

   1. Variables in C may be classified as

          globals ---|--- UNINITIALIZED  globals;
                     |---   INITIALIZED  globals;

          locals  ---|--- AUTOMATIC locals;
                     |--- STATIC    locals;

   In terms of the above classification and the variables g, a, b, c, d,

      Which variables are in DATA? ______________________________________
      Which variables are in BSS ? ______________________________________

   2. In terms of the TEXT, DATA and BSS sections,
      Which sections are in a.out, which section is NOT in a.out?
      WHY?______________________________________________________

  (B). For each case, use   cc -m32 -static t.c   to generate a.out.
       Record the sizes again and compare them with the sizes in (A).
       WHAT DO YOU SEE?_________________________________________________

       WHY?______________________________________________________________
