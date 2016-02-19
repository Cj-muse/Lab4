#include "header.h"
/*************************************************************************
  usp  1   2   3   4   5   6   7   8   9  10   11   12    13  14  15  16
----------------------------------------------------------------------------
 |uds|ues|udi|usi|ubp|udx|ucx|ubx|uax|upc|ucs|uflag|retPC| a | b | c | d |
----------------------------------------------------------------------------
***************************************************************************/

int kcinth()
{
  char str[64];
  u16 segment, offset; int a, b, c, d, r;
  segment = running->uss; offset = running->usp;
  
  printf("kcinth()\n\r");
  /* get syscall parameters from ustack */
  a = get_word(segment, offset + 2*PA);
  b = get_word(segment, offset + 2*(PA+1));
  c = get_word(segment, offset + 2*(PA+2));
  d = get_word(segment, offset + 2*(PA+3));
  /* route syscall call to kernel functions by call# a */

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

	 put_word(r, segment, offset + 2*AX); // return value in uax

}

//============= WRITE C CODE FOR syscall functions ======================
// any system calls not seen here are in kernel.c

int kkfork()
{
   //use you kfork() in kernel;
   kfork();
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
