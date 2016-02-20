#include "header.h"
/**************************************************
  bio.o, queue.o loader.o are in mtxlib
**************************************************/
/* #include "bio.c" */
/* #include "queue.c" */
/* #include "loader.c" */

//#include "kernel.c"           // YOUR kernel.c file
//#include "int.c"              // YOUR int.c    file


int body(void)
{
   char c, str[64];

   printf("proc %d resumes to body()\n\r", running->pid);
   while(1)
   {
      //color = running->pid + 7;
      printList("FreeList",freeList);
      printList("ReadyQueue",readyQueue);
      printList("SleepList",sleepList);

      printf("\rproc %d running : enter a key [s|f|z|a|w|q|u|p]: ", running->pid);
      c = getc();
      printf("%c\n\r", c);
      switch(c)
      {
         case 's': tswitch();  break;
         case 'q': do_exit();  break;
         case 'f': kfork("/bin/u1");    break;
         case 'z': do_sleep(); break;
         case 'a': do_wake();  break;
         case 'w': do_wait();  break;
         case 'u': goUmode();  break;
         case 'p': do_ps();    break;
         default: break;
      }
   }
}

int init()
{
    PROC *p; int i;
    color = 0x0C;
    printf("init ....");
    for (i=0; i<NPROC; i++){   // initialize all procs
        p = &proc[i];
        p->pid = i;
        p->status = FREE;
        p->priority = 0;
        strcpy(proc[i].name, pname[i]);
        p->next = &proc[i+1];
    }
    freeList = &proc[0];      // all procs are in freeList
    proc[NPROC-1].next = 0;
    readyQueue = sleepList = 0;

    /**** create P0 as running ******/
    p = get_proc(&freeList, FREE);
    p->status = RUNNING;
    p->ppid   = 0;
    p->parent = p;
    running = p;
    nproc = 1;
    running->status = READY;
    printf("done\n\r");
}

int scheduler()
{
  /*printf("Schedualer: running->status = %d\n\r", running->status);
  printf("Schedualer: running->pid = %d\n\r", running->pid);
  printList("freeList", freeList);
  printList("readyQueue", readyQueue);
  printList("sleepList", sleepList);*/
   if (running->status == READY)
	{
   	enqueue(&readyQueue, running);
	}
   running = dequeue(&readyQueue);
   color = running->pid + 0x0A;
   //body();
}

int int80h();
int set_vector(u16 vector , u16 handler)
{
	put_word(handler, 0x0000, vector<<2);
   put_word(0x1000,  0x0000,(vector<<2) + 2);
}

main()
{
    printf("MTX starts in main()\n\r");
    init();      // initialize and create P0 as running
    set_vector(80, int80h);

    kfork("/bin/u1");     // P0 kfork() P1

    while(1){
      printf("P0 running\n\r");
      while(!readyQueue);
      printf("P0 switch process\n\r");
      tswitch();         // P0 switch to run P1
   }
}
