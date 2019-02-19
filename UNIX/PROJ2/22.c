#include <sys/types.h>
#include <sys/stat.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
struct q_entry{
	long mtype;
	char buf[512];
	int id;
	int seq;
	int cnt;
};
int id, seq, cnt;
void writer(int qid){//, struct q_entry msg){//, int id, int seq, int cnt){
	int i;
	char len[512];
	struct q_entry msg1, msg2;

	while(1){	
		scanf("%s", len);
		if(!strcmp(len, "talk_quit")){
			//msgsnd(qid, &msg, sizeof(struct q_entry), 0);
			//msgrcv(qid, &msg, sizeof(struct q_entry), ,0);
			msg.cnt = --cnt;
			msg.seq = ++seq;
			msg.mtype = seq;
			for(i = 0; i < cnt; i ++) msgsnd(qid, &msg, sizeof(struct q_entry), 0);
			
			exit(0);
		} else {
			msgrcv(qid, &msg, sizeof(struct q_entry), seq, 0);
			cnt = msg.cnt;
			seq = msg.seq;
			strcpy(msg.buf, len);
			for(i = 0; i < cnt; i ++){
				msg.mtype = seq;
				msgsnd(qid, &msg, sizeof(struct q_entry), 0);
			}
		}	
	}
}
void reader(int qid){//, struct q_entry msg){//, int id, int seq, int cnt){
	struct q_entry msg1, msg2;

	while(1){
		msgrcv(qid, &msg1, sizeof(struct q_entry), seq, 0);
		seq ++;
		if(!strcmp(msg.buf, "loginMessage")){
			if(msg.cnt == 1) {
				printf("talk_wait...1\n"); sleep(10);
			}
			else if(msg.cnt == 2){ printf("talk_start1\n"); sleep(10);}
			else if(msg.id == id){ printf("talk_start2\n"); sleep(10);}
		} else {	
			if(!strcmp(msg.buf,"talk_quit")){
				if(msg.id == id) exit(0);
			}
			if(msg.id != id){
				printf("%d : %s\n", msg.id, msg.buf);
				printf("seq : %d\n", seq);
				printf("cnt : %d\n", msg.cnt);
				printf("mtype : %d\n\n", msg.mtype);
				sleep(20);
			} else {
				if(msg.cnt == 1) printf("talk_wait...2\n");
			}
		}
	}
}
int main(){
	int qid, i;
	char len[512];
	struct q_entry msg;

	qid = msgget(7351, 0600|IPC_CREAT|IPC_EXCL);

	id = 1;
	seq = 2;
	cnt = 1;
	
	msg.id = id;
	msg.seq = seq;
	msg.cnt = cnt;
	msg.mtype = 1;
	strcpy(msg.buf, "loginMessage");
	if(qid == -1){
		msgrcv(qid, &msg, sizeof(struct q_entry), 1, 0);
		id = ++msg.id;
		cnt = ++msg.cnt;
		seq = msg.seq;
		msg.mtype = seq;
		//strcpy(msg.buf, "loginMessage");
		printf("cnt = %d seq = %d mtype = %d\n", cnt, seq, msg.mtype);
		sleep(10);
		for(i = 0; i <= cnt; i ++){
			msgsnd(qid, &msg, sizeof(struct q_entry), 0);
		}
	}
	else {
		msgsnd(qid, &msg, sizeof(struct q_entry), 0);
		msg.mtype = seq; msgsnd(qid, &msg, sizeof(struct q_entry), 0);
	}

	printf("입장 완료 id = %d\n", id);
	if(fork() == 0) writer(qid, msg);
	else reader(qid, msg);

	exit(0);
}

