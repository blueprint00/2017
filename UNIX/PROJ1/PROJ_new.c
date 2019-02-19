#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <ftw.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>

void cat_m(char *s){
	int fd=0, n;
	char buf[512];
	char arr2[10]={0,};
	
	s =strtok(NULL, " ");
	fd = open(s, O_RDONLY);
	
	while( n = read(fd,buf,512)){
		write(1,buf,n);
	}

/*
        while(fd!=-1){
          	num=read(fd, arr2,sizeof(char));
        	if(num!=0)
               		printf("%s",arr2);
               	else
               	      	break;
      	 }
*/
	
}

void ls_m(char *s){
	DIR * dp;
	struct dirent * d; 
	dp = opendir(".");
	d = readdir(dp);

	while(d!=NULL){
		if(strcmp(d->d_name,".")) {
			if(strcmp(d->d_name,".."))
			printf("%s\n",d->d_name);
		}
		d = readdir(dp);
	}

}

void mkdir_m(char *s){
	s = strtok(NULL," ");
	mkdir(s,0700);
}

void exec_m(char *s){
	char name[20];
	int i;					
	static struct sigaction act;
	pid_t pid;
	int status;
//	s = strtok(NULL, " ");
	for(i=0; i<strlen(s); i++)
		name[i] = s[i+2];
	pid = fork();
	if(pid==0){
		act.sa_handler = SIG_DFL;
		sigaction(SIGINT,&act,NULL);
	execl(s, name, (char*)0);
	}
	wait(&status);
}

void vi_m(char *s){
	int fd;
 	int readit;
	char data[512] = {0};
	char jum = '\n';
	int n;
	s = strtok(NULL, " "); 
	fd = open(s,O_RDWR | O_CREAT | O_APPEND, 0666 );

	
	while((readit=read(fd,data,512))>0){
		write(1,data,readit);
	}


	/*//lseek = (fd, 0, SEEK_SET);
        while(1){
                readit = read(fd, data, 512);
                if(readit == 0) break;
                printf("%s",data);
        }
        //printf("\n");
*/
        while(1){
                gets(data);
		
		if(strcmp(data, "quit")==0) break;
                //printf("%s",input);
		//n = strlen(data);
		//data[n] = '\n';	
		//data[n+1] = '\0';
	        strcat(data,"\n");
		write(fd, data, strlen(data));
	
		//write(fd, jum, strlen(jum)); 
        }//while

}
void cd_m(char *s){
	s=strtok(NULL, " ");
	chdir(s);
}



int main() {
        char in[50], *res;
        char *inst[5]={"ls", "mkdir", "cat", "vi", "cd"};
        int i, flag;
        void (*f[6])(char *)={ls_m, mkdir_m, cat_m, vi_m, cd_m, exec_m};
	char buf[512]={0};
	static struct sigaction act;
//	static struct sigaction oct;
	sig_t set;
	pid_t pid;
	int status;	

	//sigemptyset(&set);
	//sigaddset(&set, SIGINT);
	//sigprocmask(SIG_SETMASK,&set,NULL);
	act.sa_handler=SIG_IGN;
	sigaction(SIGINT, &act,NULL);
	//sigaction(SIGINT, NULL, &oct);
//	printf("%d\n",flag);

        while(1){
		getcwd(buf,512);
		printf("%s", buf);

                printf("> ");
		gets(in);
        	res=strtok(in, " ");
        	if (strcmp(res, "exit")==0){
        		break;
        	}
        	else{
			printf("%s\n",in);
        		for (i=0;i<5;i++){
				flag=0;
        			if (!strcmp(res, inst[i])){
					flag=1;
					break;
				}
        		}
//			sigprocmask(SIG_UNBLOCK, &set, NULL);
			
			//printf("%s\n",res);
		//	res = strtok(NULL, " ");			
			//printf("Second : %s\n",res);
		
		//	pid = fork();
			if(flag==1 &&  strcmp(res,"cd")!=0){
			//	res = strtok(NULL," ");
				pid=fork();
				if(pid == 0){
					act.sa_handler = SIG_DFL;
					sigaction(SIGINT,&act,NULL);
					//sigprocmask(SIG_UNBLOCK, &set, NULL);
					f[i](res);
					exit(0);
				}
				else{
				wait(&status);
				}
			}
			else if( flag==0 &&  !(res[0]=='.' || res[1]=='/')){
					
			} 
			else{//exe
				//res = strtok(NULL , " ");
        			 f[i](res);
			}
			act.sa_handler = SIG_IGN;
			sigaction(SIGINT,&act,NULL);			
			//sigprocmask(SIG_SETMASK, &set, NULL);
        	}
        }

        exit(0);
}
