#include "header.h"
/**************************************************
  bio.o, queue.o loader.o are in mtxlib
**************************************************/
/* #include "bio.c" */
/* #include "queue.c" */
/* #include "loader.c" */

#include "kernel.c"           // YOUR kernel.c file
#include "int.c"              // YOUR int.c    file

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
    p = get_proc(&freeList);
    p->status = RUNNING;
    p->ppid   = 0;
    p->parent = p;
    running = p;
    nproc = 1;
    printf("done\n\r");
} 

int scheduler()
{
    if (running->status == READY)
        enqueue(&readyQueue, running);
     running = dequeue(&readyQueue);
     color = running->pid + 0x0A;
}

int int80h();
int set_vector(u16 segment, u16 handler)
{
     // put_word(word, segment, offset)

	  // don't know what to do with vector...
     // put_word(handler, 0, vector<<2);
     // put_word(0x1000,  0,(vector<<2) + 2);
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
