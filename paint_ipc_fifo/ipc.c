#include "header.h"
#include <fcntl.h>
#include <errno.h>

#define YO = 100

extern int fdDrawing;
extern int fdAnswer;
extern int nwrite;

static char *strDrawing = "Drawing";
static char *strAnswer = "Answer";

static int wroteNumber;
static int fdDrawing;
static int fdAnswer;


char answerStr[100];

void fatal(char *err){
	perror(err);
	exit(0);
}

static void InitFifoFdDrawing() { // for reader
	// ... .. .. ..
	wroteNumber = 0;
	if(isDrawer){
		if ((fdDrawing = open(strDrawing, O_WRONLY | O_NONBLOCK)) <0) {
			fatal("fifo open failed");		
		}
	}else{ 
		if (mkfifo(strDrawing, 0666) == -1){
			if(errno != EEXIST) {
				fatal("fifo exist");
			 }
		}
		if ((fdDrawing = open(strDrawing, O_RDWR))< 0) {
			fatal("fifo open failed");
		}
	}
}
static void InitFifoFdAnswer(){
	if(isDrawer){
		if ((fdAnswer = open(strAnswer,  O_WRONLY | O_NONBLOCK)) <0) {
			fatal("fifo open failed");
		}
	}else{
		if (mkfifo(strAnswer, 0666) ==-1){
			if(errno != EEXIST) {
				fatal("fifo exist");
			}
		}
		if ((fdAnswer = open(strAnswer, O_RDWR))< 0) {
			fatal("fifo poen failed");
		}
	}
}

extern void SndGameOver() {
	int nwrite;
	int yo=100;
	char yooo[4];

	sprintf(yooo,"%d",yo);
    if ((nwrite=write(fdAnswer,yooo,sizeof(yooo)))==-1)
		fatal("failed to send a signal");

  //read fifo gameover fifo
}

extern void SndAnswerCorrect(char *strAnswer) {
 	char bufAnswer[100];
	int nwrite;

	sprintf(bufAnswer,"%s",strAnswer);
	
	printf("%s \n", bufAnswer);
	if((nwrite=write(fdAnswer,bufAnswer,sizeof(bufAnswer)))==-1)
		fatal("answer write failed");

  //SendFifo
}
  
static void IpcLoopWriter() {
//	char yooo[4];
	char buf[10];
	int nwrite;
	int i=0;
//	int yo;

	for (;;) {
		for(i=wroteNumber; i<indexPath; i++){
		    	sprintf(buf, "%d", path[i].x);
			    if ((nwrite=write(fdDrawing,buf,10))==-1){
			      fatal("message write failed");
			    }
//				printf("x : %d ",path[i].x);
	
			    sprintf(buf, "%d", path[i].y);
			    if ((nwrite=write(fdDrawing,buf,10))==-1){
			      fatal("message write failed");
			    }
//				printf("y : %d ",path[i].y);
	
			    sprintf(buf, "%ld", pathColor[i]);
			    if ((nwrite=write(fdDrawing,buf,10))==-1){
			      fatal("message write failed");
			    }
//				printf("color : %ld ",pathColor[i]);
		
			    sprintf(buf, "%d", pathWidth[i]);
			    if ((nwrite=write(fdDrawing,buf,10))==-1){
			      fatal("message write failed");
			    }
	
//				printf("width : %d\n",pathWidth[i]);
				//read gameover signal
//				if(read(fdAnswer, yooo, sizeof(yooo))>0){
//					yo = atoi(yooo);
//					if(yo==100){
//						bGameOver = true;
//					}
//				}
		}
		wroteNumber = i;	
		//sleep(1);

	}
}
static void IpcLoopReader() {
//	char answerBuf[100];
	char buf[10];
	//	int i=1;

	printf("defug: ans: %s", strAnswerCorrect);
	for(;;){
//					printf("%s",strAnswerCorrect);

//			if(read(fdAnswer,answerBuf, sizeof(answerBuf))<0)
//				fatal("tlqkf");
//			strcpy(strAnswerCorrect, answerBuf);


			if(read(fdDrawing,buf,10)<0) fatal("message read failed");
			path[indexPath].x = atoi(buf);
//			printf("x : %d",path[indexPath].x);	

			if(read(fdDrawing,buf,10)<0) fatal("message read failed");
			path[indexPath].y = atoi(buf);
//			printf("y : %d",path[indexPath].y);
			
			if(read(fdDrawing,buf,10)<=0) fatal("message read failed");
			pathColor[indexPath] = atoi(buf);
//			printf("color:%d",pathColor[indexPath]);
		
			if(read(fdDrawing,buf,10)<=0) fatal("message read failed");
			pathWidth[indexPath] = atoi(buf);
//			printf("width:%d\n",pathWidth[indexPath]);
	
			indexPath++;

			RepaintPath();
	}
}


extern void *Thread2Writer() {
	InitFifoFdDrawing();
	InitFifoFdAnswer();
	IpcLoopWriter();
	pthread_exit(NULL);
}


extern void *Thread2Reader() {
	InitFifoFdDrawing();
	InitFifoFdAnswer();
	IpcLoopReader();
	pthread_exit(NULL);
}