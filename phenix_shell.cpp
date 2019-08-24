#include<stdio.h>
#include<cstring>
#include<string.h>
#include<stack>
#include<sys/wait.h>
#include "mybashrc.h"
#define WRITE 1
#define READ 0
using namespace std;
void exe_pr(char *in)
{
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
   execvp(arg[0],arg);
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
while(1){
  mybash();
  string str;
  getline(cin,str);
  char in[1024],in_d[1024];
  char *p=in;
  strcpy(in,str.c_str());
  strcpy(in_d,in);
  char *fs_in[1024];   
  char* cmd=strtok(p,"|");
  if(strcmp(cmd,ex)==0)
       exit(1);
  fs_in[0]=cmd;
  //cout<<fs_in[0];
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
                   exe_pr(fs_in[0]);
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
 }
    return 0;
}
