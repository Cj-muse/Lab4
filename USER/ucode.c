// ucode.c file

char *cmd[]={"getpid", "ps", "chname", "kfork", "switch", "wait", "exit", 0};

#define LEN 64

int show_menu()
{
   printf("***************** Menu *******************\n\r");
   printf("*  ps  chname  kfork  switch  wait  exit *\n\r");
   printf("******************************************\n\r");
}

int find_cmd(char *name)
{
  int i=0; char *p=cmd[0];

  while (p){
    if (!strcmp(p, name))
    {
      printf("FindCmd: %d\n", i);
      return i;
    }
    i++;
    p = cmd[i];
  }
  return -1;
}

int getpid() { return syscall(0,0,0,0); }
int ps() { return syscall(1,0,0,0); }
int chname(char *s) { return syscall(2,s,0,0); }
int kfork() { return syscall(3,0,0,0); }
int kswitch() { return syscall(4,0,0,0); }
int wait(int *status) { return syscall(5,status,0,0); }
int exit(int exitValue) { syscall(6,exitValue,0,0); }

int geti()
{
  // return an input integer
}

int invalid(char *name)
{
    printf("Invalid command : %s\n", name);
}
