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

int main(int argc, char **argv){
	int qid, cnt, id, i, order;
	char len[512];
	struct q_entry msg;
	pid_t pid;

	qid=msgget(8198, 0600|IPC_CREAT);

	cnt = 0;
	id = 0;
	order = 10;

	while(1){
		msgrcv(qid, &msg, sizeof(struct q_entry), -4, 0);
		if(msg.mtype == 1){
			id++;
			cnt++;
			msg.id = id;
			msg.cnt = cnt;
			msg.order = order;
			msg.mtype = 2;
			msgsnd(qid, &msg, sizeof(struct q_entry), 0);
			order++;
		}else if(msg.mtype == 3){
			msg.order = order;
			msg.cnt = cnt;
			msgsnd(qid, &msg, sizeof(struct q_entry), 0);
			order++;
		}else if(msg.mtype == 4){
			msg.mtype = 5;
			msg.cnt = cnt;
			msg.order = order;
			msgsnd(qid, &msg, sizeof(struct q_entry), 0);
			order++;
			cnt--;
			if(cnt == 0){
				exit(0);
			}
		}
	}
}
