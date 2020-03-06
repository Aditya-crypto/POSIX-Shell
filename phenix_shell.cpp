
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include<sstream>
#include <sys/wait.h>
#include<unordered_map>
#include <dirent.h>
#include <stdlib.h>
#include <termios.h>
#include <ctype.h>
#include <stdio.h>
#include <errno.h>
#include <iostream>
#include <string>
#include <fstream>
#include<vector>
#define clear() printf("\033[H\033[J")# define WRITE 1# define READ 0

using namespace std;
map < string, string > mp;
//////////////////////////////////////////////////////////////////////////////////////////////////////////Trie Datastructure
class Trie {
  public:
      unordered_map<char,Trie*> map = {};
      bool isleaf = false;

  public:

      Trie(){}   
      void insert(string word) {
          Trie* node = this;
          
          for(char ch : word){  
              if(node->map.find(ch) == node->map.end()){
                  node->map[ch] = new Trie();
              }
              node=node->map[ch];
          }
          node->isleaf = true;
      }
      
      bool search(string word) {
          Trie* node= this;
          for(char ch:word){
             if(!node->map.count(ch)) return false;
              node=node->map[ch];
          }
          return node->isleaf;
      }

      
  };

   void printsuggestion(Trie *node,string word,vector<string> &ss){
    if(node == NULL) return;
    if(node->isleaf) ss.push_back(word);
    for(auto i = node->map.begin();i!=node->map.end();i++) {printsuggestion(i->second , word + (i->first),ss);}

   }

   vector<string> printstring(Trie *node , string word){
        vector<string> ss;
        int len = word.length();
        for(int i=0;i<len;i++)
        {
          if(node->map.find(word[i]) == node->map.end()) return {};
          node = node->map[word[i]];
           }
        if(node->isleaf && (node->map.size()==0)){
          ss.push_back(word); return ss;}

          if(node->map.size() != 0){
            printsuggestion(node,word,ss);
          }

          return ss;
      }
/////////////////////////////////////////////////////////////////////////////////////////////////////// MYBASHRC.txt File
void mybash() {
  char buf[1024];
  uid_t uid = getuid();
  struct passwd * mypwdstr = getpwuid(uid);
  FILE * fd;
  fd = fopen("myrc.txt", "w");
  string user = mypwdstr - > pw_name;
  string home = mypwdstr - > pw_dir;
  if (gethostname(buf, sizeof(buf)) < 0)
    perror("hostname access permission denied");
  string path = getenv("PATH");
  char pa[1024];
  strcpy(pa, path.c_str());
  fprintf(fd, "%s\n", mypwdstr - > pw_name);
  fprintf(fd, "%s\n", mypwdstr - > pw_dir);
  fprintf(fd, "%s\n", buf);
  fprintf(fd, "%s\n", pa);
  fclose(fd);
  cout << user << "@" << buf << ":~$";
}
////////////////////////////////////////////////////////////////////////////////////////////////// read history.txt file
void show_history() {
  char buf[1024];

  FILE * fd;
  char final_path[] = "/home/phenix-fire/Documents/OS_Shell/hstry.txt";
  fd = fopen(final_path, "r");
  while (!feof(fd)) {

    fread(buf, sizeof(buf), 1, fd);
    printf("%s", buf);
  }
  fclose(fd);
}
///////////////////////////////////////////////////////////////////////////////////////////////// execute commands
int exe_pr(char * in ) {
  char dir[] = "cd";
  char hstry[] = "history";
  char
  char * arg[1024];
  char * arg_p = in ;
  char * cmd = strtok(arg_p, " ");
  arg[0] = cmd;
  //cout<<arg[0];
  if (strcmp(arg[0], hstry) == 0) {
    show_history();
    exit(1);
  }

  int i = 0;
  while (cmd != NULL) {
    cmd = strtok(NULL, " ");
    arg[++i] = cmd;
  }
  arg[++i] = NULL;
  if (strcmp(arg[0], dir) == 0) {
    chdir(arg[1]);
    return 1;
  }
  if (execvp(arg[0], arg) < 0) {
    perror("command not found");
    exit(1);
  }
}
///////////////////////////////////////////////////////////////////////////////////////////// handling with pipes
void pipe_handle(char ** inp, int i) {
  int pipe1[2];
  pid_t pi;
  int k = 0;
  int f = 0;
  int ii = 0;
  while (ii < i) {
    pipe(pipe1);
    pi = fork();
    if (pi < 0)
      perror("fork(): error");
    else if (pi == 0) {
      dup2(f, READ);
      if (i - 1 != ii)
        dup2(pipe1[WRITE], WRITE);
      close(pipe1[WRITE]);
      close(pipe1[READ]);
      exe_pr(inp[ii]);
    } else {
      wait(NULL);
      ii++;
      close(pipe1[WRITE]);
      f = pipe1[READ];
    }
  }
}
///////////////////////////////////////////////////////////////////////////////////////////////////////// add history
void add_hstry(string str) {
  char buf[1024];
  strcpy(buf, str.c_str());
  char c = 'a';
  //int fd;
  FILE * fd;
  char final_path[] = "/home/phenix-fire/Documents/OS_Shell/hstry.txt";
  fd = fopen(final_path, "a+");
  //fd=open(final_path,O_CREAT | O_WRONLY,0600);
  fprintf(fd, "%s\n", buf);
  //write(fd,&c,sizeof(c));
  fclose(fd);
}
void temp(int flag, char * second) {

  int fd;
  if (flag == 1)
    fd = open(second, O_CREAT | O_WRONLY | O_TRUNC, 0777);
  else
    fd = open(second, O_CREAT | O_WRONLY | O_APPEND, 0777);
  dup2(fd, 1);
  close(fd);
}
////////////////////////////////////////////////////////////////////////////////////////////////////// execute redirection
void exe_ind(char * first, char * second, int flag) {
  char cpy[1024];
  char * cpy_d = cpy;
  strcpy(cpy_d, cpy);
  char * fs_in[1024];
  char * cmd = strtok(first, "|");
  fs_in[0] = cmd;
  int i = 0;
  while (cmd != NULL) {
    cmd = strtok(NULL, "|");
    fs_in[++i] = cmd;
  }
  fs_in[++i] = NULL;
  pid_t pid = fork();
  if (pid < 0) {
    perror("fork(): error");
  } else if (pid == 0) {
    temp(flag, second);
    if (!strstr(cpy, "|")) {
      exe_pr(fs_in[0]);
    } else {
      pipe_handle(fs_in, i - 1);
    }
  } else {
    wait(NULL);

  }
}
bool ind(char * inp) {
  int flag = 0;
  if (strstr(inp, ">")) {
    flag = 1;
    char * arg[1024];
    char * arg_p = inp;
    char * cmd = strtok(arg_p, ">");
    arg[0] = cmd;
    int i = 0;
    while (cmd != NULL) {
      cmd = strtok(NULL, ">");
      arg[++i] = cmd;
    }
    arg[++i] = NULL;
    //cout<<arg[0];
    exe_ind(arg[0], arg[1], flag);
    return true;
  }
  if (strstr(inp, ">>")) {
    char * arg[1024];
    char * arg_p = inp;
    char * cmd = strtok(arg_p, ">>");
    arg[0] = cmd;
    int i = 0;
    while (cmd != NULL) {
      cmd = strtok(NULL, ">>");
      arg[++i] = cmd;
    }
    arg[++i] = NULL;
    exe_ind(arg[0], arg[1], flag);
    return true;
  }
  return false;
}
void insert_m(char * inp) {
  char * arg[1024];
  char * cmd = strtok(inp, "=");
  arg[0] = cmd;
  int i = 0;
  while (cmd != NULL) {
    cmd = strtok(NULL, "=");
    arg[++i] = cmd;
  }
  arg[++i] = NULL;
  string s1(arg[0]);
  string s2(arg[1]);
  mp[s1] = s2;
  //cout<<mp[s1];
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////adding aliases
int add_alias(char * inp) {
  char alsi[] = "alias";
  char * arg[1024];
  char inp_cp[1024];
  char * in = inp_cp;
  strcpy( in , inp);
  char * arg_p = in ;
  char * cmd = strtok(arg_p, " ");
  arg[0] = cmd;
  if (strcmp(arg[0], alsi) == 0) {
    for (int i = 0;; i++) {
      if (inp[i] == ' ') {
        inp[i] = '%';
        break;
      }
    }
    int i = 0;
    char * arg1[1024];
    char * cmd1 = strtok(inp, "%");
    arg1[0] = cmd1;
    while (cmd1 != NULL) {
      cmd1 = strtok(NULL, "%");
      arg1[++i] = cmd1;
    }
    arg1[++i] = NULL;
    insert_m(arg1[1]);
    //cout<<arg1[1];
    return 1;
  }
  return 0;
}
///////////////////////////////////////////////////////////////////////////////////////////////////// shell driver program
int main() {
  char ex[] = "exit";
  char clr[] = "clear";
  clear();

  while (1) {
    mybash();
    string str;
    getline(cin, str);
    add_hstry(str);
    char in [1024], in_d[1024];
    char * p = in ;
    char * p1 = in_d;
    if (mp.find(str) != mp.end()) {
      string temp = mp[str];
      strcpy( in , temp.c_str());
      strcpy(in_d, temp.c_str());
    } else {
      strcpy( in , str.c_str());
      strcpy(in_d, in );
    }
    char * fs_in[1024];
    char * cmd = strtok(p, "|");
    if (strcmp(cmd, ex) == 0)
      exit(1);
    if (strcmp(cmd, clr) == 0)
      clear();
    fs_in[0] = cmd;
    int i = 0;
    while (cmd != NULL) {
      cmd = strtok(NULL, "|");
      fs_in[++i] = cmd;
    }
    fs_in[++i] = NULL;
    if (!ind(in_d) && !add_alias(in_d)) { //cout<<fs_in[0];

      pid_t pid = fork();
      if (pid < 0) {
        perror("fork(): error");
      } else if (pid == 0) {

        if (!strstr(in_d, "|")) {
          exe_pr(fs_in[0]);
        } else {
          // cout << "Entering pipe \n" ;
          // cout<<fs_in[0]<<" "<<fs_in[1]<<" "<<i;
          pipe_handle(fs_in, i - 1);
        }
      } else {
        wait(NULL);

      }
    }
    //break;
  }

  return 0;
}
