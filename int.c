#include "header.h"
/*************************************************************************
  usp  1   2   3   4   5   6   7   8   9  10   11   12    13  14  15  16
----------------------------------------------------------------------------
 |uds|ues|udi|usi|ubp|udx|ucx|ubx|uax|upc|ucs|uflag|retPC| a | b | c | d |
----------------------------------------------------------------------------
***************************************************************************/

/****************** syscall handler in C ***************************/
int kcinth()
{
   int a,b,c,d, r;
	char str[32];

	//==> WRITE CODE TO GET get syscall parameters a,b,c,d from ustack 

   switch(a){
       case 0 : r = kgetpid();        break;
       case 1 : r = kprintstatus();   break;
       case 2 : r = kchname(str);     break;
       case 3 : r = kkfork();         break;
       case 4 : r = ktswitch();       break;
       case 5 : r = kkwait(b);        break;
       case 6 : r = kkexit(b);        break;

       case 99: kkexit(b);            break;
       default: printf("invalid syscall # : %d\n", a); 
   }

	//==> WRITE CODE to let r be the return value to Umode

}

//============= WRITE C CODE FOR syscall functions ======================
// any system calls not seen here are in kernel.c

int kkfork()
{
  //use you kfork() in kernel;
  //return child pid or -1 to Umode!!!
}

int ktswitch()
{
    return tswitch();
}

int kkwait(int *status)
{

  //use YOUR kwait() in LAB3;
  //return values to Umode!!!
}

int kkexit(int value)
{
    //use your kexit() in LAB3
    //do NOT let P1 die
}
