#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <dirent.h>
#include <ftw.h>
#include <signal.h>
#include <sys/mman.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <sys/shm.h>

struct q_entry{
	long mtype;
	char buf[512];
	int id;
	int cnt;
	int order;
};

int main(void){
	int qid, cnt, order, id, i;
	struct q_entry msg;
	pid_t pid;

	qid = msgget(8198, 0600|IPC_CREAT);

	msg.mtype = 1;
	msgsnd(qid, &msg, sizeof(struct q_entry), 0);
	msgrcv(qid, &msg, sizeof(struct q_entry), 2, 0);
	printf("id=%d\n", msg.id);
	cnt = msg.cnt;
	order = msg.order;
	id = msg.id;
	for(i=0;i<cnt;i++){
		msg.mtype = order;
		strcpy(msg.buf,"loginMessage");
		msgsnd(qid, &msg, sizeof(struct q_entry), 0);
	}

	pid = fork();
	if(pid == 0){
		while(1){
			scanf("%s", msg.buf);
			if(strcmp(msg.buf,"talk_quit")==0){
				msg.mtype = 4;
				msgsnd(qid, &msg, sizeof(struct q_entry), 0);
				msgrcv(qid, &msg, sizeof(struct q_entry), 5, 0);
				cnt = msg.cnt;
				order = msg.order;
				for(i=0;i<cnt;i++){
					msg.mtype = order;
					msgsnd(qid, &msg, sizeof(struct q_entry), 0);
				}
				exit(0);
			}else{
				msg.mtype = 3;
				msgsnd(qid, &msg, sizeof(struct q_entry), 0);
				msgrcv(qid, &msg, sizeof(struct q_entry), 3, 0);
				cnt = msg.cnt;
				order = msg.order;
				for(i=0;i<cnt;i++){
					msg.mtype = order;
					//printf("현재 order: %d\n", order);
					msgsnd(qid, &msg, sizeof(struct q_entry), 0);
				}
			}
		}
	}else{
		while(1){
			msgrcv(qid, &msg, sizeof(struct q_entry), order, 0);
			order++;
			if(strcmp(msg.buf,"loginMessage")==0){
				if(msg.cnt==1){
					printf("talk_wait...\n");
				}else if(msg.cnt==2){
					printf("talk_start\n");
				}else if(msg.id==id){
					printf("talk_start\n");
				}
			}else{
				if(strcmp(msg.buf,"talk_quit")==0){
					if(msg.id==id){
						exit(0);
					}
				}
				if(msg.id!=id){
					printf("%d : %s\n",msg.id,msg.buf);
					//printf("받은 order: %d\n", order);
					//printf("받은 mtype: %d\n\n", msg.mtype);
				}else{
					if(msg.cnt==1){
						printf("talk_wait...\n");
					}
				}
			}
		}
	}
}
