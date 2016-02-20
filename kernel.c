#include "header.h"

int ksleep(int event)
{
	//printf("ksleep():\n\r");
	running->event = event; // record event in PROC.event
	running->status = SLEEP; // change status to SLEEP

	//add proc to sleepList
	put_proc(&sleepList, running);

	//printf("after put_proc: pid %d status %d \n\r", running->pid, running->status);
	// give up CPU
	tswitch();
}

int kwakeup(int event)
{
	int i; PROC *p;
	for (i=1; i<NPROC; i++) // not applicable to P0
	{
		p = &proc[i];
	   if (p->status == SLEEP && p->event == event)
		{
			//remove from sleepList
			get_proc(&sleepList, SLEEP);

			p->event = 0; // cancel PROC’s event
		  	p->status = READY; // make it ready to run again
		   enqueue(&readyQueue, p);
		}
	}
}

int kexit(int exitvalue)
{
	int i, wakeupP1 = 0;
	PROC *p = 0;

 	if (running->pid == 1 && nproc > 2) // nproc = number of active PROCs
	{
  		printf("other procs stil exist, P1 can't die yet\n\r");
   	return -1;
	}

	/* send children (dead or alive) to P1's orphanage */
 	for (i = 1; i < NPROC; i++)
	{
  		p = &proc[i];
   	if (p->status != FREE && p->ppid == running->pid)
		{
	 		p->ppid = 1;
	  		p->parent = &proc[1];
	   	wakeupP1++;
		}
	}

	// record exitValue in PROC.exitCode for parent to get
	running->exitCode = exitvalue;

	// become a ZOMBIE (but do not free the PROC)
	running->status = ZOMBIE;

	/* wakeup parent and also P1 if necessary */
  	kwakeup(running->parent); // parent sleeps on its PROC address
   if (wakeupP1)
	{
		kwakeup(&proc[1]);
	}
	tswitch(); // give up CPU
}

int kwait(int *status)
{
	PROC *p; int i, hasChild = 0;
 	while(1) // search PROCs for a child
	{
  		for (i=1; i < NPROC; i++) // exclude P0
		{
   		p = &proc[i];
	 		if (p->status != FREE && p->ppid == running->pid)
			{
		  		hasChild = 1; // has child flag
		   	if (p->status == ZOMBIE) // lay the dead child to rest
				{
			 		*status = p->exitCode; // collect its exitCode
			  		p->status = FREE; // free its PROC
			   	put_proc(&freeList, p); // to freeList
				 	nproc--; // once less processes
				  	return(p->pid); // return its pid
				}
			}
		}
		if (!hasChild) return -1; // no child, return ERROR
		ksleep(running); // still has kids alive: sleep on PROC address
	}
}

PROC *kfork(char *filename)
{
	 int i, segment;
   PROC *p = get_proc(&freeList, FREE);

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

	if(filename)
	{
		segment = ((p->pid+1)*0x1000);
		p->uss = segment;

		//init stack to empty
		for (i=1; i<=12; i++){         // write 0's to ALL of them
			put_word(0, p->uss, -2*i);
		}
		load(filename, p->uss);
		// Fill in uDS, uES, uCS
		put_word(0x0200,  p->uss, -2*1);   /* flag */
		put_word(p->uss,  p->uss, -2*2);   /* uCS */
		put_word(p->uss,  p->uss, -2*11);   /* uES */
		put_word(p->uss,  p->uss, -2*12);   /* uDS */

		/* initial USP relative to USS */
		p->usp = -2*12; //-24
	}
	else
	{
		p->uss = 0;
	}

	printf("\rp->uss = %d p->usp = %d\n\r", p->uss, p->usp);
   enqueue(&readyQueue, p); // enter p into readyQueue by priority
   printf("kfork(): success\n\r");
   //printList("readyqueue", readyQueue);
   //printList("freeList", freeList);

   return p;
}

int kgetpid()
{
	return running->pid;
}

int kprintstatus()
{
	printf("_______________________________\n\r");
	printf("PROC %d Status. \n\r", running->pid);
	printf("ksp:          %d\n\r", running->ksp);
	printf("usp:          %d\n\r", running->usp);
	printf("uss:          %d\n\r", running->uss);
	printf("inkmode:      %d\n\r", running->inkmode);
	printf("pid:          %d\n\r", running->pid);
	printf("status:       %d\n\r", running->status);
	printf("ppid          %d\n\r", running->ppid);
	printf("priority:     %d\n\r", running->priority);
	printf("event:        %d\n\r", running->event);
	printf("exitcode:     %d\n\r", running->exitCode);
	printf("name:         %s\n\r", running->name);
	printf("_______________________________\n\r");
}

int kchname(char name[32])
{
	char str[64];
	printf("Please enter name: ");\
	gets(str);

	//change the name
}

/////////////////
/* byte movement
u8 get_byte(u16 segment, u16 offset) {
	u8 byte;
	setds(segment);
	byte = *(u8 *)offset;
	setds(MTXSEG);
	return byte;
}

int put_byte(u8 byte, u16 segment, u16 offset) {
	setds(segment);
	*(u8 *)offset = byte;
	setds(MTXSEG);
}

u16 get_word(u16 segment, u16 offset) {
	u16 word;
	setds(segment);
	word = *(u16 *)offset;
	setds(MTXSEG);
	return word;
}

int put_word(u16 word, u16 segment, u16 offset) {
	setds(segment);
	*(u16 *)offset = word;
	setds(MTXSEG);
}
*/
