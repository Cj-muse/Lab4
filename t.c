#include "header.h"
#include "queue.c"   // YOUR queue.c file: get_proc/put_proc, enqueue/dequeue
#include "kernel.c"  // YOUR kernel functions
int myExit()
{
   if (running->pid) // don't ever kill P0
   {
      running->status = ZOMBIE;
      tswitch();
      printf("Running: P%d\n\r", running->pid);
   }
}
int body(void)
{ 
	char c, str[64];
	int test = 0;

   printf("proc %d resumes to body()\n\r", running->pid);
   while(1)
   {
      color = running->pid + 7;
		printList("FreeList",freeList);
		printList("ReadyQueue",readyQueue);
		printList("SleepList",sleepList);

      printf("\rproc %d running : enter a key : ", running->pid);
      c = getc(); 
      printf("%c\n\n\r", c);
		switch(c)
		{
			case 's': tswitch();	 break;
			case 'q': do_exit();	 break;
			case 'f': kfork();	 break;
			case 'z': do_sleep(); break;
			case 'a': do_wake();  break;
			case 'w': do_wait();  break;
         default: break;
		}
   }
}
PROC *kfork()
{
	int i;
	PROC *p = get_proc(&freeList, FREE);
	printf("FREE = %d \n\r", FREE);

	if (!p)
	{
		printf("no more PROC, kfork() failed\n\r");
		return 0;
	}
	
	p->status = READY;
	p->priority = 1; // priority = 1 for all proc except P0
	p->ppid = running->pid; // parent = running
	p->parent = running;

   /* initialize new proc's kstack[ ] */
	for (i=1; i<10; i++) // saved CPU registers
	{
		p->kstack[SSIZE-i]= 0 ; // all 0's
	}
	
	p->kstack[SSIZE-1] = (int)body; // resume point=address of body()
	p->ksp = &p->kstack[SSIZE-9]; // proc saved sp

	enqueue(&readyQueue, p); // enter p into readyQueue by priority
	printf("kfork(): success\n\r");
	//printList("readyqueue", readyQueue);
	//printList("freeList", freeList);

	return p; 
}
int init()
{
    PROC *p;
    int i;

    printf("init ....");
    for (i=0; i<NPROC; i++){   // initialize all procs
         p = &proc[i];
         p->pid = i;
         p->status = FREE;
         p->priority = 0;     
         p->next = &proc[i+1];
    }
    freeList = &proc[0];      // all procs are in freeList
    proc[NPROC-1].next = 0;
    readyQueue = sleepList = 0;

    /**** create P0 as running ******/
    p = get_proc(&freeList, FREE);
    p->status = READY;
    running = p;
    nproc++;                 // number of active PROCs 
    printf("done\n\r");
} 

int scheduler()
{
//	printf("schedualer(): r->pid:%d r->staus: %d \n\r", running->pid, running->status);
   
  // printList("readyqueue", readyQueue);

  	if (running->status == READY)
  	{
  		enqueue(&readyQueue, running);
  	}
 	running = dequeue(&readyQueue);
}
            
main()
{
    printf("MTX starts in main()\n\r");
    init();      // initialize and create P0 as running
    kfork();     // P0 kfork() P1
    while(1)
   {
      printf("P0 running\n\r");
      if (nproc==2 && proc[1].status != READY)
	      printf("no runable process, system halts\n\r");
      while(!readyQueue);
      printf("P0 switch process\n\r");
      tswitch();   // P0 switch to run P1
   }
}
