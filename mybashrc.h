#include<iostream>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<pwd.h>
#include<fcntl.h>
#include<stdio.h>
using namespace std;
void mybash()
{   char buf[1024];
   uid_t uid=getuid();
   struct passwd * mypwdstr=getpwuid(uid);
   FILE* fd;
   fd=fopen("bashrc.txt","w");
   string user=mypwdstr->pw_name;
   string home=mypwdstr->pw_dir;
   if(gethostname(buf,sizeof(buf))<0)
      perror("hostname access permission denied");
  fprintf(fd,"%s\n",mypwdstr->pw_name);
  fprintf(fd,"%s\n",mypwdstr->pw_dir);
  fprintf(fd,"%s\n",buf);
   
   cout<<user<<"@"<<buf<<":~$";
}

