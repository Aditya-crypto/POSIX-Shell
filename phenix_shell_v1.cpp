#include<stdio.h>
#include<cstring>
#include<string.h>
#include<stack>
#include<sys/wait.h>
#include<iostream>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<pwd.h>
#include<fcntl.h>
#include<map>
#include<sys/stat.h>
#define clear() printf("\033[H\033[J")
#define WRITE 1
#define READ 0

using namespace std;
//map<string,string> mp;
void mybash()
{   char buf[1024];
   uid_t uid=getuid();
   struct passwd * mypwdstr=getpwuid(uid);
   FILE* fd;
   fd=fopen("myrc.txt","w");
   string user=mypwdstr->pw_name;
   string home=mypwdstr->pw_dir;
   if(gethostname(buf,sizeof(buf))<0)
      perror("hostname access permission denied");
  fprintf(fd,"%s\n",mypwdstr->pw_name);
  fprintf(fd,"%s\n",mypwdstr->pw_dir);
  fprintf(fd,"%s\n",buf);
   fclose(fd);
   cout<<user<<"@"<<buf<<":~$";
}

int exe_pr(char *in)
{  char dir[]="cd";
   char hstry[]="history";
   
   char *arg[1024];
   char*arg_p=in;
   char* cmd=strtok(arg_p," ");
   arg[0]=cmd;
  //cout<<arg[0];
 
 
  int i=0;
  while(cmd!= NULL)
  {  
      cmd=strtok(NULL, " ");  
      arg[++i]=cmd;
  }
   arg[++i]=NULL;
    if(strcmp(arg[0],dir)==0)
       {
         chdir(arg[1]);
         return 1;
       }
   if(execvp(arg[0],arg)<0)
   {
    perror("command not found");
    exit(1);
    }
}
void pipe_handle(char**inp,int i)
  {      //cout<<"hello";
          int pipe1[2];
          pid_t pi;
         int k=0;
         int f=0;
         int ii = 0;
         while(ii < i)
        {   pipe(pipe1);
           pi=fork();
          if(pi<0)
            perror("fork(): error");
          else if(pi==0)
          {  dup2(f,READ);
             if(i-1 != ii)
                dup2(pipe1[WRITE],WRITE);
             close(pipe1[WRITE]);
             close(pipe1[READ]);
             exe_pr(inp[ii]);
          }
          else
          {
            wait(NULL);
            ii++;
            //close(pipe1[READ]);
            close(pipe1[WRITE]);
            f=pipe1[READ];
            //close(pipe1[READ]);
          }
       }
  }


int main()
{   char ex[]="exit";
    char clr[]="clear";
    clear();
    
while(1)
{
  mybash();
  string str;
  getline(cin,str);
  //add_hstry(str);
  char in[1024],in_d[1024];
  char *p=in;
  char *p1=in_d;
 /* if(mp.find(str))
      { string temp=mp[str];
        strcpy(in,temp.c_str());
        strcpy(in_d,temp.c_str());
      }
   else{*/
  strcpy(in,str.c_str());
  strcpy(in_d,in);
  //}
  char *fs_in[1024];   
  char* cmd=strtok(p,"|");
  if(strcmp(cmd,ex)==0)
       exit(1);
  if(strcmp(cmd,clr)==0)
       clear();   
  fs_in[0]=cmd;
  //if(!add_alias(fs_in[0]))
  // { //cout<<fs_in[0];
      int i=0;
      while(cmd!= NULL)
      {  
        cmd=strtok(NULL, "|");  
        fs_in[++i]=cmd;
      }
      fs_in[++i]=NULL;
  
      pid_t pid=fork();
      if(pid<0)
         { 
            perror("fork(): error");
         }
      else if(pid==0)
           {  
               
               if(!strstr(in_d,"|"))
                   {  
                      exe_pr(fs_in[0]);
                   }
               else 
                  {
                   //cout << "Entering pipe \n" ;
                    pipe_handle(fs_in,i-1);
                  }
           }
      else
          {
             wait(NULL);
             
          }
          //break;
    }
 //}
    return 0;
}
