// ucode.c file

char *cmd[]={"getpid", "ps", "chname", "kfork",
  "switch", "wait", "exit", "getc", "putc",0};

#define LEN 64

int show_menu()
{
   printf("***************** Menu *******************\n\r");
   printf("*  ps  chname  kfork  switch  wait  exit *\n\r");
   printf("*   getc  putc                    \n");
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
int cmgetc(){return syscall(7,0,0,0);}
int cmputc(char c){return syscall(8,c,0,0);}


int geti()
{
  // return an input integer
  char c = getc();

  switch(c)
  {
    case '0': return 0; break;
    case '1': return 1; break;
    case '2': return 2; break;
    case '3': return 3; break;
    case '4': return 4; break;
    case '5': return 5; break;
    case '6': return 6; break;
    case '7': return 7; break;
    case '8': return 8; break;
    case '9': return 9; break;
    default: printf("Not an int\n");return -1;
  }
}

int invalid(char *name)
{
    printf("Invalid command : %s\n", name);
}
