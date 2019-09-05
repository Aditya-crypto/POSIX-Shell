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
              char in[1024];
              char* p=in;
              take_input(in);
              char *fs_in[1024];
              char* cmd=strtok(in, " | ");
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
               cout<<i<<endl;
            }
  
           }
            else{
             wait(NULL);
             
            }
   
   

  return 0;
}
