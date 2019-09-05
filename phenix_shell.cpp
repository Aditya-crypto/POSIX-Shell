#include<stdio.h>
#include<cstring>
#include<stdio.h>
#include<iostream>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<stack>
#include<sys/types.h>
#include<sys/wait.h>
#include<fcntl.h>
using namespace std;
int gflag=0;
char* get_p(char *in,char *fs)
{  char ri[]="/bin/";
   char *r=ri;
   char* cmd=strtok(in," ");
   strcpy(fs,r);
   strcat(fs,cmd);
   return fs;
}
char** get_p_arg(char * in,char**f)
{  char stp[]="STOP";  
   char* cmd=strtok(in," ");
   // char * argp[100];
   f[0]=cmd;
   int i=0;
while(cmd!= NULL)
{   if(strcmp(cmd,stp)==0)
           break;
    cmd=strtok(NULL," ");  
    f[++i]=cmd;
   
}
f[++i]=NULL;

return f;
}
void get_arg(char *in)
  {   
     char t_c[1024],t_temp[1024];
     char*t_o=t_c;
     strcat(in,"\0");   //perfect input for one command
     strcpy(t_o,in);
     char*tt_temp=t_temp;
     strcpy(tt_temp,in);
     char fs[1024];
    char *p=get_p(tt_temp,fs);//p and fs are storing process path
    char * argp[100];
    char**t_arg=argp;
    t_arg=get_p_arg(t_o,argp);  
    //cout<<p<<" "<<argp[0]<<" "<<argp[1]<<" t_o is "<<t_o<<"\n";                     
    execvp(fs,argp);
  }
void take_input(char* inp)
{
     int i=0;
    char ch;
    while(1)
    {
      ch=fgetc(stdin);
       if(ch==0x0A)
           break;
       else if(ch=='|')
             gflag=1;
       else
	{
             inp[i]=ch;
             i++;  
	}
		
    }
  inp[i]=NULL;
}
void pipe_handle(char**inp,int* pipeend,int i)
  {
         int f=0;
         
  }
int main()
{
          int pipeend1[2];
          pipe(pipeend1);
          pid_t pid=fork();
          char *ex="exit";
            if(pid<0)
            { 
               perror("fork(): error");
            }
         
           else if(pid==0)
           {
              while(1)
            { 
              char in[1024],p_c_[1024];
              char* p=in;
              char* p_c=p_c_;
              take_input(in);
              strcpy(p_c,p);
              char *fs_in[1024];
              char* cmd=strtok(p, " | ");
              fs_in[0]=cmd;
              if(strcmp(cmd,ex)==0)
                exit(1);
              int i=0;
              while(cmd!= NULL)
              {  
                 cmd=strtok(NULL, " | ");  
                 fs_in[++i]=cmd;
              }
              fs_in[++i]=NULL;
               //cout<<i<<endl;
              if(gflag==0)
              get_arg(p_c_);
              else 
              {
              pipe_handle(fs_in,pipeend1,i-1);
              }
            }
  
           }
            else{
             wait(NULL);
             
            }
   
   

  return 0;
}
